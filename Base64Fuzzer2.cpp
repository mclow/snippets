#include <vector>
#include <iostream>
#include <iomanip>

#include "Base64.hpp"

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
	
	return 0;  // Non-zero return values are reserved for future use.
}
