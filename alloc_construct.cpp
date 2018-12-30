#include <vector>
#include <iostream>

template <typename T>
struct MyAlloc
{
    typedef unsigned                                                   size_type;
    typedef int                                                        difference_type;
    typedef T                                                        value_type;
    typedef value_type*                                                pointer;
    typedef const value_type*                                          const_pointer;
    typedef typename std::add_lvalue_reference<value_type>::type       reference;
    typedef typename std::add_lvalue_reference<const value_type>::type const_reference;

    template <class U> struct rebind {typedef MyAlloc<U> other;};

    MyAlloc() noexcept {}
    ~MyAlloc() {}

    pointer allocate(size_type n, const void* = 0)
        { return (pointer)::operator new(n * sizeof(value_type)); }
    
    void deallocate(pointer p, size_type)
        { ::operator delete((void*)p);}
};


template <>
struct MyAlloc<int>
{
    typedef unsigned                                                   size_type;
    typedef int                                                        difference_type;
    typedef int                                                        value_type;
    typedef value_type*                                                pointer;
    typedef const value_type*                                          const_pointer;
    typedef typename std::add_lvalue_reference<value_type>::type       reference;
    typedef typename std::add_lvalue_reference<const value_type>::type const_reference;

    template <class U> struct rebind {typedef MyAlloc<U> other;};

    MyAlloc() noexcept {}
    ~MyAlloc() {}

    pointer allocate(size_type n, const void* = 0)
        { return (pointer)::operator new(n * sizeof(value_type)); }
    
    void deallocate(pointer p, size_type)
        { ::operator delete((void*)p);}
    
    void construct(pointer p)
        { *p = 3; }

    void destroy(pointer p) {}    
};


int main () {
	std::vector<int, std::allocator<int>> v1;
	v1.resize(5);
	for (int i: v1) std::cout << i << ' ';
	std::cout << std::endl;

	std::vector<int, MyAlloc<int>> v2;
	v2.resize(5);
	for (int i: v2) std::cout << i << ' ';
	std::cout << std::endl;
	}