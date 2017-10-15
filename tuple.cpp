#include <tuple>
#include <algorithm>

using T1 = std::tuple<int, float, char>;
using T2 = std::tuple<long, double>;

template <size_t _Ip>
struct eq_
{
    template <class _Tp, class _Up>
    constexpr
    bool operator()(const _Tp& __x, const _Up& __y)
    {
        return eq_<_Ip - 1>()(__x, __y) && _VSTD::get<_Ip-1>(__x) == _VSTD::get<_Ip-1>(__y);
    }
};

template <>
struct eq_<0>
{
    template <class _Tp, class _Up>
    constexpr
    bool operator()(const _Tp&, const _Up&)
    {
        return true;
    }
};


template <typename T1, typename T2>
constexpr bool eq (const T1 &t1, const T2 &t2) {
	if constexpr (std::tuple_size<T1>::value == std::tuple_size<T2>::value)
		return eq_<std::tuple_size<T1>::value>()(t1, t2);
	return false;
}


template <size_t _Ip, size_t _Sz>
struct cmp_
{
    template <class _Tp, class _Up>
    constexpr
    int operator()(const _Tp& __x, const _Up& __y)
    {
        const size_t __idx = _Sz - _Ip;
        if (std::get<__idx>(__x) < std::get<__idx>(__y))
            return -1;
        if (std::get<__idx>(__y) < std::get<__idx>(__x))
            return 1;
        return cmp_<_Ip-1, _Sz>()(__x, __y);
    }
};

template <size_t _Sz>
struct cmp_<0, _Sz>
{
    template <class _Tp, class _Up>
    constexpr
    int operator()(const _Tp&, const _Up&)
    {
        return 0;
    }
};



template <typename T1, typename T2>
constexpr bool lt (const T1 &t1, const T2 &t2) {
	const size_t sz = std::min(std::tuple_size<T1>::value,std::tuple_size<T2>::value);
	const int ret = cmp_<sz, sz>()(t1, t2);
	if (ret < 0) return true;
	if (ret > 0) return false;
//	ret == 0: equal length tuples are equal, not less than
	return std::tuple_size<T1>::value < std::tuple_size<T2>::value;
}

int main () {
	constexpr T1 t1{3, 4.0f, 'a'};
	constexpr T2 t2{5L, 6.0};
	
	static_assert(!eq(t1, t2));
	static_assert( lt(t1, t2));
}