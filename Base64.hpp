// base64 encoding and decoding - info from https://en.wikipedia.org/wiki/Base64
// Copyright 2019 Marshall Clow
// Licensed under the Boost Software License
//
//
//	General idea: have an object that manages the conversion
//		It keeps a bit of state, for when the input need not be an even multiple of 3
//		(on encoding) or 4 (on decoding). This allows you to process in chunks
//		(say streaming) instead of requiring the entire input to be available at once.
//
//	There are two calls: encode(decode) and finish. The first one processes as much as
//		possible on each call. The 'finish' method is used to signal that there is no
//		more input, and any waiting data should be written.
//
//	Examples:
//	I just want to decode a buffer. No fancy stuff
//		Base64Decoder{}.decode_and_finish(ptr, ptr + size, dest);
//
//	I'm getting chunks from somewhere
//		Base64Decoder b64;
// 		while (!done) {
// 			dest = b64.decode(ptr, ptr + size, dest);
// 			.. get more data ..
// 		}
// 		dest = b64.finish(dest);
//
//	I want to convert a string into another string
// 	string source = ....;
// 	string dest;
// 	dest.reserve(Base64Decoder::decoded_size(source.size())); // not strictly necessary
// 	Base64Decoder{}.decode_and_finish(source.begin(), source.end(),
// 			back_insert_iterator<char>(dest));
// 	
//
//	Problems
//		How to report/recover from errors
//			At the present time, I throw exceptions
//		How do ensure that people call "finish"?
//			I can assert/throw in the destructor, but that falls afoul of the other error handling.
//	
//	This technique can be used for uuencode/decode as well https://en.wikipedia.org/wiki/Uuencoding

#include <iterator>
#include <cassert>
#include <stdexcept>

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
