// base64 encoding and decoding - method cribbed from https://en.wikipedia.org/wiki/Base64

#include <iterator>
#include <cassert>
#include <stdexcept>

// #include <iostream>

class Base64Decoder {
public:
	Base64Decoder () noexcept : finish_called(false), num_chars(0), eq_count(0), chars(0) {};
	~Base64Decoder () { /* assert(finish_called); */ }
	
	template <typename OutputIterator>
	OutputIterator finish (OutputIterator out)
	{
		assert(num_chars < 4);
		if (eq_count > 0 && (num_chars + eq_count) != 4)
			throw std::runtime_error("Base64 decode: wrong # of '='");
		if (num_chars == 1)
			throw std::runtime_error("Base64 decode: wrong # characters");

	//	Output the last few chars
		if (num_chars == 2)
		{
			if (chars & 0x0F) throw std::runtime_error("Base64 decode: zero bits should be zero(2)");
			*out++ = static_cast<char>(chars >> 4);
		}
		else if (num_chars == 3)
		{
			if (chars & 0x03) throw std::runtime_error("Base64 decode: zero bits should be zero(3)");
			*out++ = static_cast<char>(chars >> 10);
			*out++ = static_cast<char>(chars >>  2);
		}

		num_chars = 0;
		finish_called = true;
		return out;
	}
		
	template <typename InputIterator, typename OutputIterator>
	OutputIterator decode (InputIterator first, InputIterator last, OutputIterator out)
	{
		assert(!finish_called);
		assert(num_chars < 4);
		for (; first != last; ++first)
		{
			char ch = *first;
		//  deal with the termination
			if (ch == '=')
			{
				if (++eq_count > 2) throw std::runtime_error("Base64 decode: More than two '='s");
				continue;
			}
			if (eq_count > 0) throw std::runtime_error("Base64 decode: Nothing after an '='");
			if (iswhitespace(ch)) continue;
			if (!is_validB64(ch)) throw std::runtime_error("Base64 decode: Bad Base64 char");

			chars = chars << 6 | B64_Index(ch);
			if (++num_chars == 4)
			{
				*out++ = static_cast<char>(chars >> 16);
				*out++ = static_cast<char>(chars >>  8);
				*out++ = static_cast<char>(chars      );
				num_chars = 0;
			}
		}
		return out;
	}
	
	template <typename InputIterator, typename OutputIterator>
	OutputIterator decode_and_finish (InputIterator first, InputIterator last, OutputIterator out)
	{
		return finish(decode(first, last, out));
	}

	// Upper bound; may be one too big in some cases
	static size_t decoded_size (size_t sz) noexcept { return ((sz + 3) / 4) * 3; }

	// I'd use std::isspace here, but that's locale-dependent
	static bool iswhitespace(char c) noexcept
	{
		return c ==  ' ' || c == '\f' || c == '\n'
		    || c == '\r' || c == '\t' || c == '\v';
	}

	static bool is_validB64(char c) noexcept { return B64_Index(c) < 64; }
	static size_t B64_Index(char c) noexcept
	{ 
		if (c >= 'A' && c <= 'Z') return 00 + c - 'A'; //  0 .. 25
		if (c >= 'a' && c <= 'z') return 26 + c - 'a'; // 26 .. 51
		if (c >= '0' && c <= '9') return 52 + c - '0'; // 52 .. 61
		if (c == '+') return 62;
		if (c == '/') return 63;
		return 64;
	}

private:	// later, we can pack this down
	bool finish_called;
	size_t eq_count;
	size_t num_chars;
	uint32_t chars;
	};


// I just want to decode a buffer. No fancy stuff
//		Base64Decoder{}.decode_and_finish(ptr, ptr + size, dest);

// I'm getting chunks from somewhere
//		Base64Decoder b64;
// 		while (!done) {
// 			dest = b64.decode(ptr, ptr + size, dest);
// 			.. get more data ..
// 		}
// 		dest = b64.finish(dest);

// I want to convert a string into another string
// 	string source = ....;
// 	string dest;
// 	dest.reserve(Base64Decoder::decoded_size(source.size())); // not strictly necessary
// 	Base64Decoder{}.decode_and_finish(source.begin(), source.end(),
// 			back_insert_iterator<char>(dest));
// 	



class Base64Encoder {
public:
	Base64Encoder () noexcept : finish_called(false), num_chars(0), chars(0) {};
	~Base64Encoder () { assert(finish_called); }
	
	template <typename OutputIterator>
	OutputIterator finish (OutputIterator out)
	{
		assert(num_chars < 3);
		//	output the last few chars
		if (num_chars == 2)
		{
			chars <<= 2;
			*out++ = encodings[static_cast<unsigned char>((chars >> 12) & 0x3F)];
			*out++ = encodings[static_cast<unsigned char>((chars >>  6) & 0x3F)];
			*out++ = encodings[static_cast<unsigned char>((chars      ) & 0x3F)];
			*out++ = '=';			
		}
		else if (num_chars == 1)
		{
			chars <<= 4;
			*out++ = encodings[static_cast<unsigned char>((chars >>  6) & 0x3F)];
			*out++ = encodings[static_cast<unsigned char>((chars      ) & 0x3F)];
			*out++ = '=';
			*out++ = '=';			
		}

		num_chars = 0;
		finish_called = true;
		return out;
	}
		
	template <typename InputIterator, typename OutputIterator>
	OutputIterator encode (InputIterator first, InputIterator last, OutputIterator out)
	{
		assert(!finish_called);
		assert(num_chars < 3);
		for (; first != last; ++first)
		{
			chars = chars << 8 | static_cast<unsigned char>(*first);
			if (++num_chars == 3)
			{
			//	output four chars
			//	Note that with rotate, this becomes (4x): rotate_left(x, 6), *out++ = encodings[x & 0x3F]
				*out++ = encodings[static_cast<unsigned char>((chars >> 18) & 0x3F)];
				*out++ = encodings[static_cast<unsigned char>((chars >> 12) & 0x3F)];
				*out++ = encodings[static_cast<unsigned char>((chars >>  6) & 0x3F)];
				*out++ = encodings[static_cast<unsigned char>((chars      ) & 0x3F)];
				num_chars = 0;
			}
		}
		return out;
	}
	
	template <typename InputIterator, typename OutputIterator>
	OutputIterator encode_and_finish (InputIterator first, InputIterator last, OutputIterator out)
	{
		return finish(encode (first, last, out));
	}

	// Upper bound; may be one too big in some cases
	static size_t encoded_size (size_t sz) { return ((sz + 2) / 3) * 4; }

private:	// later, we can pack this down
	static constexpr const char* encodings = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
	bool finish_called;
	size_t num_chars;
	uint32_t chars;
	};

#if 0
#include <iostream>
#include <iomanip>
#include <string_view>

int main () {
// 	const char *s0 = "Man";
// 	const char *s0_enc = "TWFu";
// 	Base64Encoder{}.encode_and_finish(s0, s0 + 3, std::ostream_iterator<char>(std::cout, ""));	
// 	std::cout << std::endl;
// 	Base64Decoder{}.decode_and_finish(s0_enc, s0_enc + 4, std::ostream_iterator<char>(std::cout, ""));	
// 	std::cout << std::endl;

// 	const char *fuzz = "\x0a\x0a\xf2\x38";
// 	Base64Encoder{}.encode_and_finish(fuzz, fuzz + 4, std::ostream_iterator<char>(std::cout, ""));	
// 	std::cout << std::endl;
// 	const char *fuzzE = "CgryOA==";
// 	Base64Decoder{}.decode_and_finish(fuzzE, fuzzE + 8, std::ostream_iterator<char>(std::cout, ""));	
// 	std::cout << std::endl;
// 	

	const char *fuzz = "\x2B\x41";
	std::cout << std::hex;
	Base64Decoder{}.decode_and_finish(fuzz, fuzz + 2, std::ostream_iterator<int>(std::cout, " "));	
	std::cout << std::endl;
	const char *fuzzD = "\xF8";
	Base64Encoder{}.encode_and_finish(fuzzD, fuzzD + 1, std::ostream_iterator<char>(std::cout, ""));	
	std::cout << std::endl;
	

	const char *s1 = "any carnal pleasure.";
	for (size_t sz = 16; sz < 21; sz++) {
	//	std::cout << std::setw(30) << std::string_view{s1, sz} << " --> ";
		std::string se, sd;
		std::string_view sv{s1, sz};
		Base64Encoder{}.encode_and_finish(sv.begin(), sv.end(), std::back_inserter(se));
		Base64Decoder{}.decode_and_finish(se.begin(), se.end(), std::back_inserter(sd));
		std::cout << "\t'" << sv << "'" << std::endl;
		std::cout << "\t'" << se << "'" << std::endl;
		if (sd != sv)
			std::cout << "\t'" << sd << "'  <<-- Mismatch (" << sv.size() << " vs. " << sd.size() << ')' << std::endl;
		std::cout << std::endl;
		}
}
#endif

#if 0
#include <vector>

//	Test encode then decode
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
	typedef std::vector<uint8_t> Vec;

	Vec original{Data, Data + Size};
	Vec encoded;
	Vec decoded;
	Base64Encoder{}.encode_and_finish(original.begin(), original.end(), std::back_inserter(encoded));
	Base64Decoder{}.decode_and_finish(encoded.begin(),  encoded.end(),  std::back_inserter(decoded));
	assert(original == decoded);
	
	return 0;  // Non-zero return values are reserved for future use.
}
#endif


#if 1
#include <vector>
#include <functional>
#include <iostream>
#include <iomanip>

//	Test decode then encode
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
	typedef std::vector<uint8_t> Vec;

	Vec encoded;
	Vec decoded;
	Vec decoded2;
	Vec stripped;
	try { Base64Decoder{}.decode_and_finish(Data, Data + Size, std::back_inserter(decoded));}
	catch (const std::runtime_error &) { return 0; }

//	If it decoded successfully, encode  it again, and see if they're the "same"
	Base64Encoder{}.encode_and_finish(decoded.begin(), decoded.end(), std::back_inserter(encoded));
	std::copy_if(Data, Data + Size, std::back_inserter(stripped), [](char c) {return !Base64Decoder::iswhitespace(c);});
	if (encoded.size() == 0)
		assert(stripped.size() == 0);
	else switch (stripped.size() % 4)
	{
		case 1: assert(false); // should never happen
		case 2: stripped.push_back('='); stripped.push_back('='); break;
		case 3: stripped.push_back('='); break;
		case 0: break;
	}
	assert(stripped == encoded);
	
// If it decoded successfully, encode  it again, and see if they're the "same"
// 	Base64Encoder{}.encode_and_finish(decoded.begin(), decoded.end(), std::back_inserter(encoded));
// 	Base64Decoder{}.decode_and_finish(encoded.begin(), encoded.end(), std::back_inserter(decoded2));
// 	assert(decoded2 == decoded);
// 	
	return 0;  // Non-zero return values are reserved for future use.
}
#endif
