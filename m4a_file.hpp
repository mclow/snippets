#pragma once

#include <set>

#include "file_helpers.hpp"

//	See https://wiki.multimedia.cx/index.php/QuickTime_container

class M4AFile {
public:
	typedef uint32_t AtomType;
	typedef uint32_t AtomSize;
	
	M4AFile (const char *fileName) : file(fileName),
							containers_({'cmov', 'edts', 'trak', 'mdia', 'minf', 
							             'moov', 'stbl', 'rmda', 'rmra', 'stbl',
							             'trak'})
		{
		if (GetAtomType(0) != 'ftyp')
			throw std::runtime_error("does not have 'ftyp' as the first atom");			
		}

	~M4AFile() {}
	
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
	void ForEachAtomDo (CB cb) const { ForEachAtomDo(0, size(), 0, cb); }
	
	template <typename CB>
	void ForEachAtomDo (size_t start, size_t length, size_t depth, CB cb) const {
		size_t offset  = start;
		size_t end     = start + length;
	
		while (offset < end) {
			AtomType at = GetAtomType(offset);
			AtomSize sz = GetAtomSize(offset);

			if (end - offset < sz)
				throw std::runtime_error(std::string("Atom '") + AtomName(at) + "' has a size too big");
			else if (sz < 8)
				throw std::runtime_error(std::string("Atom '") + AtomName(at) + "' has a size too small");
			
			cb (this, at, offset + 8, sz - 8, depth);
			if (AtomIsContainer (at))
				ForEachAtomDo (offset + 8, sz - 8, depth + 1, cb);		
			
			offset += sz;
			}
		}

	const uint8_t * getPointerTo(size_t offset) const
	{
		return file.base() + offset;
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
