/* A container adaptor that implements a circular buffer
 *
 * (C) Copyright Marshall Clow 2017
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See http://www.boost.org/LICENSE_1_0.txt)
 *
 */

/*
	The elements in the container are not destroyed when they are popped.
	They are destroyed when they are overwritten or when the container is destroyed.
	
	first --> the first element in the buffer. Never == c.end().
	last  --> the next element to be placed. Never == c.end().
	If the buffer is empty, then first == last
	wrapped --> last is "before" first.
	
TODO:
	* Moar tests
	* Decide what to do when you push and the buffer is full. Throw? UB? Assert?
	* Can I make emplace work? To do that I need the allocator from the underlying container.
	* How much allocator support do I really want?
		* Pass in an allocator upon construction?
		* Pass in a container upon construction?
		* Pass in an allocator/max size on construction?
*/

#include <vector>
#include <cassert>

template <typename T, size_t N, typename Container = std::vector<T>>
class circular_buffer {
public:
    typedef Container                                container_type;
    typedef typename container_type::value_type      value_type;
    typedef typename container_type::reference       reference;
    typedef typename container_type::const_reference const_reference;
    typedef typename container_type::size_type       size_type;
	
protected:
    typedef typename container_type::iterator        iterator;
    typedef typename container_type::const_iterator  const_iterator;
    
    container_type c;
	typename container_type::iterator first;
	typename container_type::iterator last;
	bool wrapped;
	
    bool         full () const { return size() == c.size(); }
	void advance_first() { if (++first == c.end()) { wrapped = false; first = c.begin(); }}
	void advance_last () { if (++last  == c.end()) { wrapped = true;  last  = c.begin(); }}
	const_iterator advance(const_iterator it) { return ++it != c.end() ? it : c.begin(); }
	typename container_type::iterator back_iter() const { return std::prev(last != c.front() ? last : c.end()); }

public:
	circular_buffer() : c(N), first(c.begin()), last(c.begin()), wrapped(false) {}
	~circular_buffer() = default;

    bool      empty()    const { return size() == 0; }
	size_type capacity() const { return N; }
    size_type size()     const {
    	if (wrapped)
    		return std::distance((const_iterator)first, c.end()) + std::distance(c.begin(), (const_iterator) last);
    	else
    		return std::distance(first, last);
    	}

    reference       front()        { assert(!empty()); return *first; }
    const_reference front() const  { assert(!empty()); return *first; }
    void        pop_front()        { assert(!empty()); advance_first(); }

    reference       back()         { assert(!empty()); return *back_iter(); }
    const_reference back() const   { assert(!empty()); return *back_iter(); }

    void push(const value_type& v) { assert(!full()); *last = v;            advance_last (); }
    void push(value_type&& v)      { assert(!full()); *last = std::move(v); advance_last (); }

//     template <class... Args>
//       void emplace(Args&&... args) { 
//         assert(!full());
//         typename container_type::pointer p = &*last;
//         typedef typename std::allocator_traits<typename container_type::allocator> traits;
//  	//	This is clearly not exception-safe
//  	    traits::destroy(c.allocator(), p);
//         traits::construct(c.allocator(), p, std::forward<Args...>(args...));
//         advance_last();
//         return *p;
//         }

    void swap(circular_buffer& cb) noexcept(std::is_nothrow_swappable<container_type>::value)
    {
    	using std::swap;
    	swap(c,     cb.c);
    	swap(first, cb.first);
    	swap(last,  cb.last);
    }

	bool operator==(const circular_buffer &rhs)
	{
		if (size() != rhs.size()) return false;
		const_iterator it1 = first;
		const_iterator it2 = rhs.first;
		for (size_t i = 0; i < size(); ++i)
		{
			if (*it1 != *it2)
				return false;
			it1 = advance(it1);
			it2 = rhs.advance(it2);
		}
		return true;
	}

	bool operator<(const circular_buffer &rhs)
	{
		const_iterator it1 = first;
		const_iterator it2 = rhs.first;
		for (size_t i = 0; i < std::min(size(), rhs.size()); ++i)
		{
			if (*it1 < *it2)
				return true;
			if (*it2 < *it1)
				return false;
			it1 = advance(it1);
			it2 = rhs.advance(it2);
		}
		return size() < rhs.size();
	}

};

template <class T, size_t N, class Container>
bool operator==(const circular_buffer<T, N, Container>& lhs, const circular_buffer<T, N, Container>& rhs)
  { return lhs.operator==(rhs); }

template <class T, size_t N, class Container>
bool operator< (const circular_buffer<T, N, Container>& lhs, const circular_buffer<T, N, Container>& rhs)
  { return lhs.operator<(rhs); }

template <class T, size_t N, class Container>
bool operator!=(const circular_buffer<T, N, Container>& lhs, const circular_buffer<T, N, Container>& rhs)
  { return !(lhs == rhs); }
  
template <class T, size_t N, class Container>
bool operator> (const circular_buffer<T, N, Container>& lhs, const circular_buffer<T, N, Container>& rhs)
  { return rhs < lhs; }

template <class T, size_t N, class Container>
bool operator>=(const circular_buffer<T, N, Container>& lhs, const circular_buffer<T, N, Container>& rhs)
  { return !(lhs < rhs); }

template <class T, size_t N, class Container>
bool operator<=(const circular_buffer<T, N, Container>& lhs, const circular_buffer<T, N, Container>& rhs)
  { return !(rhs < lhs); }

template <class T, size_t N, class Container>
void swap(const circular_buffer<T, N, Container>& lhs, const circular_buffer<T, N, Container>& rhs)
  { lhs.swap(rhs); }



int main () {
	circular_buffer<int, 10> b;
	assert(b.size() == 0);
	assert(b.capacity() == 10);
	for (int i = 0; i < 7; ++i )
		b.push(i);
	assert(b.size() == 7);

	for (int i = 0; i < 10; ++i )
	{
		assert(b.front() == i);
		b.pop_front();
		assert(b.size() == 6);
		b.push(7 + i);
		assert(b.size() == 7);
	}	

	for (int i = 0; i < 7; ++i )
	{
		assert(b.front() == 10 + i);
		b.pop_front();
		assert(b.size() == 6 - i);
	}	
	assert(b.empty());

	for (int i = 0; i < 10; ++i )
	{
		b.push(100 + i);
		assert(b.size() == i + 1);
	}
	assert(b.size() == b.capacity());
	for (int i = 0; i < 20; ++i )
	{
		assert(b.front() == 100 + i);
		b.pop_front();
		b.push(110 + i);
		assert(b.size() == b.capacity());
	}
}
