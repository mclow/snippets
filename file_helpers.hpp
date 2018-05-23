#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


//	A RAII class that encapsulates a unix file descriptor
class unixFile {
public:
	unixFile (const char *fileName)
	{
		fd = ::open ( fileName, O_RDONLY );
		if ( fd < 0 )
			throw std::runtime_error("can't open");
	}

	unixFile(const unixFile &) = delete;
	unixFile & operator=(const unixFile&) = delete;
	unixFile(unixFile &&rhs) : fd(rhs.release ()) {}
	unixFile & operator=(unixFile &&rhs) { fd = rhs.release(); return *this; }

	~unixFile() { if (fd != -1 ) ::close(fd); }
	int release() { int old = fd; fd = -1; return old; }
	int getFD() const { return fd; }
		
private:
	int fd;
};


//	A RAII class that encapsulates a memory-mapped file
class MMFile {
public:
	MMFile (const char *fileName) : f(fileName)
	{
	    struct stat fileStat;
    	if ( ::fstat(f.getFD(), &fileStat) < 0 )
			throw std::runtime_error("can't fstat");
		fSize = fileStat.st_size;

		fAddr = ::mmap(nullptr, fSize, PROT_READ, MAP_SHARED, f.getFD(), 0);
		if (fAddr == MAP_FAILED)
			throw std::runtime_error("can't mmap");
	}

	~MMFile() { if (fAddr != nullptr ) ::munmap(fAddr, fSize); }
	MMFile(const MMFile &) = delete;
	MMFile & operator=(const MMFile&) = delete;
	MMFile(MMFile &&rhs) : f(std::move(rhs.f)), fSize(rhs.fSize), fAddr(rhs.fAddr)
	{
		rhs.fSize = 0;
		rhs.fAddr = nullptr;
	}

	MMFile & operator=(MMFile &&rhs)
	{
		f = std::move(rhs.f);
		fSize = rhs.fSize;  rhs.fSize = 0;
		fAddr = rhs.fAddr;  rhs.fAddr = nullptr;
		return *this;
	}
		
	size_t size() const { return fSize; }
	const unsigned char * base() const { return (const unsigned char *) fAddr; }
	
private:
	unixFile f;
	size_t fSize;
	void *fAddr;
};


//	A RAII class that encapsulates a POSIX file descriptor
class posixFile {
public:
	posixFile (const char *fileName, const char *mode)
	{
		fp = ::fopen ( fileName, mode );
		if ( fp == nullptr )
			throw std::runtime_error("can't open");
	}

	posixFile(const posixFile &) = delete;
	posixFile & operator=(const posixFile&) = delete;
	posixFile(posixFile &&rhs) : fp(rhs.release ()) {}
	posixFile & operator=(posixFile &&rhs) { fp = rhs.release(); return *this; }

	~posixFile() { if (fp != nullptr) ::fclose(fp); }
	FILE * release() { FILE *old = fp; fp = nullptr; return old; }
	operator FILE *() const { return fp; }

private:
	FILE * fp;
};

