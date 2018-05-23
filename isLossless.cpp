#include <iostream>
#include <cstdio>
#include <limits.h>
#include <set>

#include "file_helpers.hpp"

bool gPrintLossless = true;
bool gDebug = false;

class MPAFile {
public:
	typedef uint32_t AtomType;
	typedef uint32_t AtomSize;
	typedef void (*AtomProc) (const MPAFile *f, AtomType atom, size_t start, size_t length);
	
	MPAFile (const char *fileName) : file(fileName),
							containers_({'cmov', 'edts', 'trak', 'mdia', 'minf', 
							             'moov', 'stbl', 'rmda', 'rmra', 'stbl',
							             'trak'})
		{
		if (GetAtomType(0) != 'ftyp')
			throw std::runtime_error("does not have 'ftyp' as the first atom");			
		}

	~MPAFile() {}
	
	AtomSize GetAtomSize(size_t offset) const { return read32(offset); }
	AtomType GetAtomType(size_t offset) const { return read32(offset + 4); }
	size_t size() const { return file.size(); }

	const std::set<AtomType> & containers() const { return containers_; }
	bool AtomIsContainer(AtomType at) const { return containers_.find(at) != containers_.end();}

	std::string AtomName(AtomType at) const {
		std::string retVal ( "    " );
		const unsigned char *p = (const unsigned char *) &at;
		for (int i = 3; i >= 0; --i)
			retVal [ i ] = *p++;
		return retVal;
		}

	template <typename CB>
	void ForEachAtomDo (CB cb) const { ForEachAtomDo(0, size(), cb); }
	
	template <typename CB>
	void ForEachAtomDo (size_t start, size_t length, CB cb) const {
		size_t offset  = start;
		size_t end     = start + length;
	
		while (offset < end) {
			MPAFile::AtomType at = GetAtomType(offset);
			MPAFile::AtomSize sz = GetAtomSize(offset);

			if (end - offset < sz)
				throw std::runtime_error(std::string("Atom '") + AtomName(at) + "' has a size too big");
			else if (sz < 8)
				throw std::runtime_error(std::string("Atom '") + AtomName(at) + "' has a size too small");
			
			cb (this, at, offset + 8, sz - 8);
			offset += sz;
			}
		}

	uint32_t read32(size_t offset) const {
		uint32_t retVal = 0;
		const unsigned char *p = file.base() + offset;
	
		for (int i = 0; i < 4; ++i) {
			retVal <<= 8;
			retVal |= *p++;
		}

	return retVal;
	}

private:
	MMFile file;
	std::set<AtomType> containers_;
};


struct AtomCallback {
	static int cbIsLossless;
	AtomCallback(int indent = 0) : indentStr(indent, ' ') { cbIsLossless = -1; }
	
	void CheckForLossless ( const MPAFile *f, long start, long length ) {
		if ( gDebug )
			std::cout << "Found a 'stsd' at offset " << start << std::endl;
		cbIsLossless = f->read32 ( start + 12 ) == 0x616C6163 ? 1 : 0;
		}


	void operator() (const MPAFile *f, MPAFile::AtomType atom, size_t start, size_t length) {
		if ( gDebug )
			std::cout << indentStr << "'" << f->AtomName(atom) << "' (" << length << " bytes)" << std::endl;

		if ( 'stsd' == atom )
			CheckForLossless ( f, start, length );
		else if (f->AtomIsContainer (atom))
			f->ForEachAtomDo ( start, length, AtomCallback(indentStr.size() + 2) );		
		}
	std::string indentStr;
	};

int AtomCallback::cbIsLossless;

char toHex(int i) { return "0123456789ABCDEF"[i]; }

void ProcessAFile(const char *fileName) {
	try {
		MPAFile f(fileName);
		if (gDebug) {
			std::cout << "Processing " << fileName << std::endl;
			std::cout << "File  is " << f.size() << " bytes long" << std::endl;
			}

		f.ForEachAtomDo(AtomCallback{});
		if (AtomCallback::cbIsLossless < 0)
			throw std::runtime_error("does not have a 'stsd' atom");
		else if ( AtomCallback::cbIsLossless == gPrintLossless )
			std::cout << fileName << std::endl;
		}
	catch (const std::runtime_error &e) {
		std::cerr << "## File '" << fileName << "': " << e.what() << std::endl;
		}
	}

int main( int argc, char *argv [] )
{
	if ( argc == 1 ) {
		std::cout << "Usage: " << argv[0] << " [-!] <list of files" << std::endl;
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
