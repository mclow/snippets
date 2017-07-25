#include <cstddef>		// for std::ptrdiff_t
#include <array>		// for std::array
#include <type_traits>	// for remove_cv, etc
#include <cassert>

/*	
	Things that I've found:

template <class ElementType, ptrdiff_t Extent>
constexpr bool operator!=(const span<ElementType, Extent>& l, 
							const span<ElementType, Extent>& r) const noexcept;

--> Can't have const on non-member functions
===
template <ptrdiff_t Count>
constexpr span<element_type, Count> last() const;
Returns: span(Count == 0 ? data() : data() + (size() – Count), Count);

--> why not just return span(data() + (size() – Count), Count);
===
The following code does not compile:
	int i = 23;
	span<int, 1> s2{&i, 1};
	span<int> s3{&i, 1};
	assert(s2 == s3);

--> I think it should

*/



//	http://open-std.org/JTC1/SC22/WG21/docs/papers/2016/p0122r1.pdf

/*
// [views.constants], constants constexpr ptrdiff_t dynamic_extent = -1;
// [span], class template span
template <class ValueType, ptrdiff_t Extent = dynamic_extent> class span;
 23.4 Associative containers
23.5 Unordered associative containers 23.6 Container adaptors
23.7 Views
23.7 Views [views]
23.7.1 General [views.general]
<stack>
<span>
                                 // [span.comparison], span comparison operators template <class ElementType, ptrdiff_t Extent>
  constexpr bool operator==(const span<ElementType, Extent>& l, const span<ElementType, Extent>& r) const noexcept;
   template <class ElementType, ptrdiff_t Extent>
constexpr bool operator!=(const span<ElementType, Extent>& l, const
  span<ElementType, Extent>& r) const noexcept;
  template <class ElementType, ptrdiff_t Extent>
constexpr bool operator<(const span<ElementType, Extent>& l, const
  span<ElementType, Extent>& r) const noexcept;
  template <class ElementType, ptrdiff_t Extent>
constexpr bool operator<=(const span<ElementType, Extent>& l, const
  span<ElementType, Extent>& r) const noexcept;
  template <class ElementType, ptrdiff_t Extent>
constexpr bool operator>(const span<ElementType, Extent>& l, const
  span<ElementType, Extent>& r) const noexcept;
  template <class ElementType, ptrdiff_t Extent>
constexpr bool operator>=(const span<ElementType, Extent>& l, const
  span<ElementType, Extent>& r) const noexcept;
  
  
<class ElementType, ptrdiff_t Extent>
  constexpr span<const char, ((Extent == dynamic_extent) ? dynamic_extent : (sizeof(ElementType) * Extent))> as_bytes(span<ElementType, Extent> s) noexcept;
   
template <class ElementType, ptrdiff_t Extent>
constexpr span<char, ((Extent == dynamic_extent) ? dynamic_extent :
 (sizeof(ElementType) * Extent))> as_writeable_bytes(span<ElementType, Extent> ) noexcept;
 
 */

constexpr std::ptrdiff_t dynamic_extent = -1;


// A view over a contiguous, single-dimension sequence of objects 
template <typename _Tp, ptrdiff_t _Extent = dynamic_extent>
class span {
public:
//	constants and types
	using element_type           = _Tp;
	using index_type             = std::ptrdiff_t;
	using pointer                = _Tp*;
	using reference              = _Tp&;
	using iterator               = pointer;
	using const_iterator         = const pointer;
	using reverse_iterator       = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	constexpr static index_type extent = _Extent;

  // [span.cons], span constructors, copy, assignment, and destructor 
	constexpr span() : __data{nullptr}, __size{0}
		{ static_assert(_Extent == dynamic_extent || _Extent == 0); }
	constexpr span(std::nullptr_t) : __data{nullptr}, __size{0}
		{ static_assert(_Extent == dynamic_extent || _Extent == 0); }
	constexpr span(pointer __ptr, index_type __count) : __data{__ptr}, __size{__count}
		{ assert(_Extent == dynamic_extent || _Extent == __size); }
	constexpr span(pointer __f, pointer __l) : __data{__f}, __size{std::distance(__f, __l)}
		{ assert(_Extent == dynamic_extent || _Extent == __size); }

	template <size_t _N>
		constexpr span(element_type (&__arr)[_N]) : __data{__arr}, __size{_N}
		{ static_assert(_Extent == dynamic_extent || _Extent == __size); }

	template <size_t N>
		constexpr span(std::array<std::remove_const_t<element_type>, N>& __arr)
		: __data{__arr.data()}, __size{__arr.size()}
		{ static_assert(_Extent == dynamic_extent || _Extent == __size); }

	template <size_t N>
		constexpr span(const std::array<std::remove_const_t<element_type>, N>& __arr)
		: __data{__arr.data()}, __size{__arr.size()}
		{ static_assert(_Extent == dynamic_extent || _Extent == __size); }

// 	template <class Container>
// 		constexpr span(Container& cont);
// 	template <class Container> span(const Container&&) = delete;

	constexpr span(const span&  other) noexcept = default;
	constexpr span(      span&& other) noexcept = default; 

// 	template <class OtherElementType, std::ptrdiff_t OtherExtent>
// 		constexpr span(const span<OtherElementType, OtherExtent>& other);
// 		
// 	template <class OtherElementType, std::ptrdiff_t OtherExtent>
// 		constexpr span(span<OtherElementType, OtherExtent>&& other);

	~span() noexcept = default;

	constexpr span& operator=(const span&  other) noexcept = default;
	constexpr span& operator=(      span&& other) noexcept = default;

	template <std::ptrdiff_t _Count>
	 	constexpr span<element_type, _Count> first() const
	 	{ return span{data(), _Count}; }

	template <std::ptrdiff_t _Count>
		constexpr span<element_type, _Count> last() const
	 	{ return span{data() + size() - _Count, _Count}; }	// !! not as spec'ed

// 	template <std::ptrdiff_t Offset, std::ptrdiff_t Count = dynamic_extent>
// 		constexpr span<element_type, Count> subspan() const;

	constexpr span<element_type, dynamic_extent> first(index_type __count) const
		{ return span{data(), __count}; }
	constexpr span<element_type, dynamic_extent> last(index_type __count)  const
	 	{ return span{data() + size() - __count, __count}; }	// !! not as spec'ed
	
// 	constexpr span<element_type, dynamic_extent> subspan(index_typeoffset, index_type count = dynamic_extent) const;

// 	constexpr index_type length() const noexcept;
	constexpr index_type size() const noexcept { return __size; }
// 	constexpr index_type length_bytes() const noexcept;
// 	constexpr index_type size_bytes() const noexcept;
	constexpr bool empty() const noexcept { return __size == 0; }

	constexpr reference operator[](index_type __idx) const { return __data[__idx]; }
	constexpr reference operator()(index_type __idx) const  { return __data[__idx]; }
	constexpr pointer data() const noexcept { return __data; }

// [span.iter], span iterator support
	iterator                 begin() const noexcept { return data(); }
	iterator                   end() const noexcept { return data() + size(); }
	const_iterator          cbegin() const noexcept { return data(); }
	const_iterator            cend() const noexcept { return data() + size(); }
	reverse_iterator        rbegin() const noexcept { return reverse_iterator(end()); }
	reverse_iterator          rend() const noexcept { return reverse_iterator(begin()); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
	const_reverse_iterator   crend() const noexcept { return const_reverse_iterator(cbegin()); }

private:
//	exposition only
	pointer    __data;
	index_type __size;
};

template <class ElementType, std::ptrdiff_t Extent>
	constexpr bool operator==(const span<ElementType, Extent>& __lhs, const span<ElementType, Extent>& __rhs) noexcept
	{ return std::equal(__lhs.begin(), __lhs.end(), __rhs.begin(), __rhs.end()); }
// 
// template <class ElementType, std::ptrdiff_t Extent>
// 	constexpr bool operator!=(const span<ElementType, Extent>& l, const span<ElementType, Extent>& r) const noexcept;
// 
// template <class ElementType, std::ptrdiff_t Extent>
// 	constexpr bool operator< (const span<ElementType, Extent>& l, const span<ElementType, Extent>& r) const noexcept;
// 
// template <class ElementType, std::ptrdiff_t Extent>
// 	constexpr bool operator<=(const span<ElementType, Extent>& l, const span<ElementType, Extent>& r) const noexcept;
// 
// template <class ElementType, std::ptrdiff_t Extent>
// 	constexpr bool operator> (const span<ElementType, Extent>& l, const span<ElementType, Extent>& r) const noexcept;
// 
// template <class ElementType, std::ptrdiff_t Extent>
// 	constexpr bool operator>=(const span<ElementType, Extent>& l, const span<ElementType, Extent>& r) const noexcept;
// 
// // [span.objectrep], views of object representation template
// <class ElementType, std::ptrdiff_t Extent>
// 	constexpr span<const byte, ((Extent == dynamic_extent) ? dynamic_extent : (sizeof(ElementType) * Extent))> as_bytes(span<ElementType, Extent> s) noexcept;
// 
// template <class ElementType, std::ptrdiff_t Extent>
// 	constexpr span<      byte, ((Extent == dynamic_extent) ? dynamic_extent : (sizeof(ElementType) * Extent))> as_writeable_bytes(span<ElementType, Extent> ) noexcept;


int main ()
{
	span<int> s0;
	assert(s0.size() == 0);
	span<int, 0> s1;
	assert(s1.size() == 0);
	
	int i = 23;
	span<int, 1> s2{&i, 1};
	assert(s2.size() == 1);
	assert(s2.data() == &i);
	assert(s2[0] == 23);
	auto it2 = s2.begin();
	assert(*it2 == 23);
	++it2;
	assert(it2 == s2.end());
	
	span<int> s3{&i, 1};
	assert(s3.size() == 1);
	assert(s3.data() == &i);
	assert(s3[0] == 23);
	auto it3 = s2.rbegin();
	assert(*it3 == 23);
	++it3;
	assert(it3 == s2.rend());
	
	assert(s2 == s3);
	
}