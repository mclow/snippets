/* Implement rotation ops as defined in http://wg21.link/P0553R1
 *
 * (C) Copyright Marshall Clow 2017
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See http://www.boost.org/LICENSE_1_0.txt)
 *
 */
 
#include <algorithm>	// for std::ptrdiff_t
#include <type_traits>	// for remove_cv, etc
#include <cassert>

/*

namespace std {
  inline namespace bit_ops {
  // 23.20.2, rotating   
  template<class T>
    constexpr T rotl(T x, unsigned int s) noexcept;
  template<class T>
    constexpr T rotr(T x, unsigned int s) noexcept;

  // 23.20.3, counting
  template<class T>
    constexpr int countl_zero(T x) noexcept;
  template<class T>
    constexpr int countl_one(T x) noexcept;
  template<class T>
    constexpr int countr_zero(T x) noexcept;
  template<class T>
    constexpr int countr_one(T x) noexcept;
  template<class T>
    constexpr int popcount(T x) noexcept;
  }
}
*/

_LIBCPP_BEGIN_NAMESPACE_STD
inline namespace bit_ops {

template <class _Tp>
constexpr inline _LIBCPP_ALWAYS_INLINE
enable_if_t<is_integral_v<_Tp> && is_unsigned_v<_Tp>, _Tp>
rotl(_Tp __x, unsigned int __count)
{
	const unsigned __digits = numeric_limits<_Tp>::digits;
	const unsigned __shift  = __count % __digits;
	if (__shift == 0) return __x;
	return (__x << __shift ) | (__x >> (__digits - __shift));
}

template <class _Tp>
constexpr inline _LIBCPP_ALWAYS_INLINE
enable_if_t<is_integral_v<_Tp> && is_unsigned_v<_Tp>, _Tp>
rotr(_Tp __x, unsigned int __count)
{
	const unsigned __digits = numeric_limits<_Tp>::digits;
	const unsigned __shift  = __count % __digits;
	if (__shift == 0) return __x;
	return (__x >> __shift ) | (__x << (__digits - __shift));
}


// We have:
//	unsigned long long __clz(unsigned long long __x)
//	unsigned long __clz(unsigned long __x)
//	unsigned __clz(unsigned __x)
//	and the same for __ctz and __pop_count



template <class _Tp>
constexpr inline _LIBCPP_ALWAYS_INLINE
enable_if_t<is_integral_v<_Tp> && is_unsigned_v<_Tp>, int>
countl_zero(_Tp __x)
{
	return __x == 0 ? numeric_limits<_Tp>::digits : __clz(__x);
}

template <class _Tp>
constexpr inline _LIBCPP_ALWAYS_INLINE
enable_if_t<is_integral_v<_Tp> && is_unsigned_v<_Tp>, int>
countl_one(_Tp __x)
{
	return __x == numeric_limits<_Tp>::max() ?
					numeric_limits<_Tp>::digits : countl_zero(static_cast<_Tp>(~__x));
}

template <class _Tp>
constexpr inline _LIBCPP_ALWAYS_INLINE
enable_if_t<is_integral_v<_Tp> && is_unsigned_v<_Tp>, int>
countr_zero(_Tp __x)
{
	return __x == 0 ? numeric_limits<_Tp>::digits : __ctz(__x);
}

template <class _Tp>
constexpr inline _LIBCPP_ALWAYS_INLINE
enable_if_t<is_integral_v<_Tp> && is_unsigned_v<_Tp>, int>
countr_one(_Tp __x)
{
	return __x == numeric_limits<_Tp>::max() ?
					numeric_limits<_Tp>::digits : countr_zero(static_cast<_Tp>(~__x));
}

template <class _Tp>
constexpr inline _LIBCPP_ALWAYS_INLINE
enable_if_t<is_integral_v<_Tp> && is_unsigned_v<_Tp>, int>
popcount(_Tp __x)
{
	return __pop_count(__x);
}

} // namespace bit_ops
_LIBCPP_END_NAMESPACE_STD


#include <iostream>

int main () {
	for (unsigned i = 0; i < 40; ++i) {
		std::cout << i << ": ";
		std::cout << std::rotl<unsigned>(1,i) << ' ';
		std::cout << std::rotr<unsigned>(1,i) << ' ';
		std::cout << std::endl;
		}

	for (unsigned i = 0; i < 40; ++i) {
		std::cout << i << ": ";
		std::cout << std::countl_zero<unsigned>(i) << ' ';
		std::cout << std::countl_one<unsigned>(i) << ' ';
		std::cout << std::countr_zero<unsigned>(i) << ' ';
		std::cout << std::countr_one<unsigned>(i) << ' ';
		std::cout << std::popcount<unsigned>(i) << ' ';
		std::cout << std::endl;
		}
	}