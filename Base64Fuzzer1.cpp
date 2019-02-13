#include <vector>
#include "Base64.hpp"

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
