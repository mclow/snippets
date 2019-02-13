#include <iostream>
#include <iomanip>
#include <string_view>

#include "Base64.hpp"

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
