#include <array>
#include <algorithm>


_LIBCPP_BEGIN_NAMESPACE_STD

template <class T1, class T2, size_t N1, size_t N2>
inline _LIBCPP_INLINE_VISIBILITY
bool operator==(const array<T1,N1>& __x, const array<T2,N2>& __y)
{
    if (N1 != N2) return false;
    return _VSTD::equal(__x.__elems_, __x.__elems_ + __x.size(), __y.__elems_, __y.__elems_ + __y.size());
}

template <class T1, class T2, size_t N1, size_t N2>
inline _LIBCPP_INLINE_VISIBILITY
bool operator!=(const array<T1,N1>& __x, const array<T2,N2>& __y)
{ return !(__x == __y); }

template <class T1, class T2, size_t N1, size_t N2>
inline _LIBCPP_INLINE_VISIBILITY
bool operator<(const array<T1,N1>& __x, const array<T2,N2>& __y)
{
    return _VSTD::lexicographical_compare(__x.__elems_, __x.__elems_ + __x.size(), __y.__elems_, __y.__elems_ + __y.size());
}

template <class T1, class T2, size_t N1, size_t N2>
inline _LIBCPP_INLINE_VISIBILITY
bool operator>(const array<T1,N1>& __x, const array<T2,N2>& __y)
{ return __y < __x; }

template <class T1, class T2, size_t N1, size_t N2>
inline _LIBCPP_INLINE_VISIBILITY
bool operator<=(const array<T1,N1>& __x, const array<T2,N2>& __y)
{ return !(__y < __x); }

template <class T1, class T2, size_t N1, size_t N2>
inline _LIBCPP_INLINE_VISIBILITY
bool operator>=(const array<T1,N1>& __x, const array<T2,N2>& __y)
{ return !(__x < __y); }

_LIBCPP_END_NAMESPACE_STD



int main () {
	std::array<int, 5> five_i = {1,2,3,4,5};
	std::array<int, 6> six_i = {1,2,3,4,5,6};
	std::array<long, 5> five_l = {1,2,3,4,5};
	
	assert(!(five_i == six_i));
	assert(  five_i != six_i );
	assert(  five_i <  six_i );
	assert(  five_i <= six_i );
	assert(!(five_i >  six_i));
	assert(!(five_i >= six_i));

	assert(  five_i == five_l );
	assert(!(five_i != five_l));
	assert(!(five_i <  five_l));
	assert(  five_i <= five_l );
	assert(!(five_i >  five_l));
	assert(  five_i >= five_l );
}