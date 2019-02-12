// base64 encoding and decoding
#include <array>
#include <iterator>
#include <cassert>

#include <iostream>

class Base64Decoder {
public:
	Base64Decoder () noexcept : finish_called(false), num_chars(0), eq_count(0) {};
	~Base64Decoder () { assert(finish_called); }
	
	template <typename OutputIterator>
	OutputIterator finish (OutputIterator out)
	{
		assert(num_chars < chars.size());
		if (eq_count > 0 && num_chars + eq_count != 4)
			throw std::runtime_error("Base64 decode: wrong # of '='");
		if (num_chars == 1)
			throw std::runtime_error("Base64 decode: wrong # characters");

	//	Output the last few chars
		uint32_t bits = 0;
		for (size_t sz = 0; sz < num_chars; ++sz)
			bits = bits << 6 | B64_Index(chars[sz]);
			
		if (num_chars == 2)
			*out++ = static_cast<char>(bits >> 4);
		else if (num_chars == 3)
		{
			*out++ = static_cast<char>(bits >> 10);
			*out++ = static_cast<char>(bits >>  2);
		}

		num_chars = 0;
		finish_called = true;
		return out;
	}
		
	template <typename InputIterator, typename OutputIterator>
	OutputIterator decode (InputIterator first, InputIterator last, OutputIterator out)
	{
		assert(!finish_called);
		assert(num_chars < chars.size());
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
			chars[num_chars++] = ch;
			if (num_chars == chars.size())
			{
				uint32_t bits = 0;
				bits =             B64_Index(chars[0]);
				bits = bits << 6 | B64_Index(chars[1]);
				bits = bits << 6 | B64_Index(chars[2]);
				bits = bits << 6 | B64_Index(chars[3]);

				*out++ = static_cast<char>(bits >> 16);
				*out++ = static_cast<char>(bits >>  8);
				*out++ = static_cast<char>(bits      );
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
		return c == ' ' || c == '\f' || c == '\n'
		    || c == 'r' || c == '\t' || c == '\v';
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
	std::array<char, 4> chars;
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
	Base64Encoder () noexcept : finish_called(false), num_chars(0) {};
	~Base64Encoder () { assert(finish_called); }
	
	template <typename OutputIterator>
	OutputIterator finish (OutputIterator out)
	{
		assert(num_chars < chars.size());
		//	output the last few chars
		if (num_chars == 2)
		{
			*out++ = encodings[((chars[0]       ) >> 2)];
			*out++ = encodings[((chars[0] & 0x03) << 4) | ((chars[1] & 0xF0) >> 4)];
			*out++ = encodings[((chars[1] & 0x0F) << 2)];
			*out++ = '=';			
		}
		else if (num_chars == 1)
		{
			*out++ = encodings[((chars[0]       ) >> 2)];
			*out++ = encodings[((chars[0] & 0x03) << 4)];
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
		assert(num_chars < chars.size());
		for (; first != last; ++first)
		{
			chars[num_chars++] = static_cast<unsigned char>(*first);
			if (num_chars == chars.size())
			{
			//	output four chars
			//	Note that with rotate, this becomes (4x): rotate_left(x, 6), *out++ = encodings[x & 0x3F]
				*out++ = encodings[((chars[0]       ) >> 2)];
				*out++ = encodings[((chars[0] & 0x03) << 4) | ((chars[1] & 0xF0) >> 4)];
				*out++ = encodings[((chars[1] & 0x0F) << 2) | ((chars[2] & 0xC0) >> 6)];
				*out++ = encodings[((chars[2] & 0x3F)     )];
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
	std::array<unsigned char, 3> chars;
	};

#include <iomanip>
#include <string_view>

int main () {
	const char *s0 = "Man";
	const char *s0_enc = "TWFu";
	Base64Encoder{}.encode_and_finish(s0, s0 + 3, std::ostream_iterator<char>(std::cout, ""));	
	std::cout << std::endl;
	Base64Decoder{}.decode_and_finish(s0_enc, s0_enc + 4, std::ostream_iterator<char>(std::cout, ""));	
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