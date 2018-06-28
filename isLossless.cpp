#include <iostream>
#include <cstdio>
#include <limits.h>

#include "m4a_file.hpp"

bool gPrintLossless = true;
bool gDebug = false;

//	We're looking for a 'stsd' atom
struct AtomCallback {
	int isLossless;
	AtomCallback() { isLossless = -1; }
	
	void operator() (const M4AFile *f, M4AFile::AtomType atom, size_t start, size_t length, size_t depth)
	{
		if ( gDebug )
			std::cout << std::string(depth, ' ') << "'" << f->AtomName(atom) << "' (" << length << " bytes)" << std::endl;

		if ( 'stsd' == atom )
		{
			if ( gDebug )
				std::cout << "Found a 'stsd' at offset " << start << std::endl;
			if ( length < 16 )
				throw std::runtime_error("'stsd' atom is too small");
			isLossless = f->read32 ( start + 12 ) == 0x616C6163 ? 1 : 0;
		}
	}
};


void ProcessAFile(const char *fileName) {
	try {
		M4AFile f(fileName);
		if (gDebug) {
			std::cout << "Processing " << fileName << std::endl;
			std::cout << "File  is " << f.size() << " bytes long" << std::endl;
			}

		AtomCallback cb;
		f.ForEachAtomDo(std::ref(cb));
		if (cb.isLossless < 0)
			throw std::runtime_error("does not have a 'stsd' atom");
		else if ( cb.isLossless == gPrintLossless )
			std::cout << fileName << std::endl;
		}
	catch (const std::runtime_error &e) {
		std::cerr << "## File '" << fileName << "': " << e.what() << std::endl;
		}
	}

int main( int argc, char *argv [] )
{
	if ( argc == 1 ) {
		std::cout << "Usage: " << argv[0] << " [-!] [-d] <list of files" << std::endl;
		std::cout << "  prints the path of files that encoded with Apple lossless encoder" << std::endl;
		std::cout << "  if -! is specified, prints the path of files NOT encoded lossless" << std::endl;
		return 0;
		}
	
	int startArg = 1;
	if ( std::strcmp ( argv[startArg], "-!" ) == 0 ) {
		gPrintLossless = false;
		startArg = 2;
		}
		
	if ( std::strcmp ( argv[startArg], "-d" ) == 0 ) {
		gDebug = true;
		startArg++;
		}

 	for ( int i = startArg; i < argc; ++i )
		ProcessAFile(argv[i]);
		
	return 0;
}
