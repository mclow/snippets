#include <cmath>


#define _LIBCPP_DISABLE_UBSAN_INTEGER_TRUNCATION_CHECK __attribute__((__no_sanitize__("implicit-integer-truncation")))


_LIBCPP_BEGIN_NAMESPACE_STD


template <class _Tp>
_LIBCPP_INLINE_VISIBILITY constexpr
enable_if_t<is_integral_v<_Tp> && !is_same_v<bool, remove_cv_t<_Tp>>, _Tp>
midpoint(_Tp __a, _Tp __b) noexcept
_LIBCPP_DISABLE_UBSAN_UNSIGNED_INTEGER_CHECK
{
    using _Up = std::make_unsigned_t<remove_cv_t<_Tp>>;

    int __sign = 1;
    _Up __m = __a;
    _Up __M = __b;
    if (__a > __b)
    {
        __sign = -1;
        __m = __b;
        __M = __a;
    }
     return __a + __sign * _Tp(_Up(__M-__m) >> 1);
}


template <class _TPtr>
_LIBCPP_INLINE_VISIBILITY constexpr
enable_if_t<is_pointer_v<_TPtr>, _TPtr>
midpoint(_TPtr __a, _TPtr __b) noexcept
{
    return __a + midpoint<ptrdiff_t>(0, __b - __a);
}


template <class _Fp>
_LIBCPP_INLINE_VISIBILITY constexpr
enable_if_t<is_floating_point_v<_Fp>, _Fp>
midpoint(_Fp __a, _Fp __b) noexcept
{
    return isnormal(__a) && isnormal(__b)
             ?  __a / 2 + __b / 2
             : (__a + __b) / 2;
}

// Float midpoint(Float a, Float b)
// {return isnormal(a) && isnormal(b) ? a/2+b/2 : (a+b)/2;}

_LIBCPP_END_NAMESPACE_STD


#include <iostream>
#include <cassert>
#include <limits>
#include <cstdint>
#include <sstream>

#include <boost/math/special_functions/next.hpp>  // for float_distance.


template<typename Integer>
constexpr Integer midpoint(Integer a, Integer b) noexcept {
  using U = std::make_unsigned_t<Integer>;
  return a>b ? a-(U(a)-b)/2 : a+(U(b)-a)/2;
}

template <typename T>
void signed_int_test()
{
	static_assert(std::is_same_v<T, decltype(std::midpoint(T(), T()))>, "");
	static_assert(                  noexcept(std::midpoint(T(), T())), "");
	using limits = std::numeric_limits<T>;
	
	assert(std::midpoint<T>(0,0)   == T(0));
	assert(std::midpoint<T>(0,2)   == T(1));
	assert(std::midpoint<T>(2,0)   == T(1));
	assert(std::midpoint<T>(2,2)   == T(2));

	assert(std::midpoint<T>( 3, 4) == T(3));
	assert(std::midpoint<T>( 4, 3) == T(4));
	assert(std::midpoint<T>(-3, 4) == T(0));
	assert(std::midpoint<T>(-4, 3) == T(-1));
	assert(std::midpoint<T>( 3,-4) == T(0));
	assert(std::midpoint<T>( 4,-3) == T(1));
	assert(std::midpoint<T>(-3,-4) == T(-3));
	assert(std::midpoint<T>(-4,-3) == T(-4));
	
	assert(std::midpoint<T>(limits::min(), limits::max()) == T(-1));
	assert(std::midpoint<T>(limits::max(), limits::min()) == T(0));

	assert(std::midpoint<T>(limits::min(), 6) == limits::min()/2 + 3);
	assert(std::midpoint<T>(6, limits::min()) == limits::min()/2 + 3);
	assert(std::midpoint<T>(limits::max(), 6) == limits::max()/2 + 4);
	assert(std::midpoint<T>(6, limits::max()) == limits::max()/2 + 3);

	assert(std::midpoint<T>(limits::min(), -6) == limits::min()/2 - 3);
	assert(std::midpoint<T>(-6, limits::min()) == limits::min()/2 - 3);
	assert(std::midpoint<T>(limits::max(), -6) == limits::max()/2 - 2);
	assert(std::midpoint<T>(-6, limits::max()) == limits::max()/2 - 3);
}

template <typename T>
void unsigned_int_test()
{
	static_assert(std::is_same_v<T, decltype(std::midpoint(T(), T()))>, "");
	static_assert(                  noexcept(std::midpoint(T(), T())), "");
	using limits = std::numeric_limits<T>;
	const T half_way = (limits::max() - limits::min())/2;
	
	assert(std::midpoint<T>(0,0)   == T(0));
	assert(std::midpoint<T>(0,2)   == T(1));
	assert(std::midpoint<T>(2,0)   == T(1));
	assert(std::midpoint<T>(2,2)   == T(2));

	assert(std::midpoint<T>(3,4)   == T(3));
	assert(std::midpoint<T>(4,3)   == T(4));
	
	assert(std::midpoint<T>(limits::min(), limits::max()) == T(half_way));
	assert(std::midpoint<T>(limits::max(), limits::min()) == T(half_way + 1));

	assert(std::midpoint<T>(limits::min(), 6) == limits::min()/2 + 3);
	assert(std::midpoint<T>(6, limits::min()) == limits::min()/2 + 3);
	assert(std::midpoint<T>(limits::max(), 6) == half_way + 4);
	assert(std::midpoint<T>(6, limits::max()) == half_way + 3);
}

template <typename T>
void pointer_test()
{
	T array[1000] = {}; // we need an array to make valid pointers
	static_assert(std::is_same_v<T*, decltype(std::midpoint(array, array))>, "");	
	static_assert(                   noexcept(std::midpoint(array, array)), "");

	assert(std::midpoint(array, array)        == array);
	assert(std::midpoint(array, array + 1000) == array + 500);

	assert(std::midpoint(array, array +    9) == array + 4);
	assert(std::midpoint(array, array +   10) == array + 5);
	assert(std::midpoint(array, array +   11) == array + 5);
	assert(std::midpoint(array +    9, array) == array + 5);
	assert(std::midpoint(array +   10, array) == array + 5);
	assert(std::midpoint(array +   11, array) == array + 6);
}


template <typename T>
bool closeEnough(T val, T expected, int p = std::numeric_limits<T>::max_digits10 - 3)
{
	if (val == expected) return true;
	T diff = val - expected;
	std::cout << "Distance :" << (int) boost::math::float_distance(val, expected) << std::endl;
// 	T rel = ( diff / val ) * std::pow(T(10), std::numeric_limits<T>::max_digits10);
// 	std::cout << "diff is :" << rel << " ppm" << std::endl;
	std::cout << std::hexfloat << val << " vs " << expected << std::endl;
	
	{
		T temp = val;
		size_t c;
		for (c = 1; temp < expected; ++c)
			temp = std::nexttoward(temp, expected);
		std::cout << "Took " << c << " steps to match\n";
		}
    {
    std::ostringstream o;
    o.precision(p);
    scientific(o);
    o << val;
    std::string a = o.str();
    std::cout << "First string: " << a;
    o.str("");
    o << expected;
    std::cout << "    second string: " << o.str() << std::endl;
    return true; // a == o.str();
	}
	
	return false;
}

template <typename T>
void fp_test()
{
	std::cout << "Digits: " << std::numeric_limits<T>::max_digits10 << std::endl;
	std::cout << "Eps:  "   << std::numeric_limits<T>::epsilon() << std::endl;
//	Things that can be compared exactly
    assert(std::midpoint(T(0), T(0)) == T(0));
    assert(std::midpoint(T(2), T(4)) == T(3));
    assert(std::midpoint(T(4), T(2)) == T(3));
    assert(std::midpoint(T(3), T(4)) == T(3.5));

//	Things that can't be compared exactly
	assert((closeEnough(std::midpoint(T( 1.3), T(11.4)), T( 6.35))));
	assert((closeEnough(std::midpoint(T(11.33), T(31.45)), T(21.39))));
	assert((closeEnough(std::midpoint(T(-1.3), T(11.4)), T( 5.05))));
	assert((closeEnough(std::midpoint(T(11.4), T(-1.3)), T( 5.05))));

	assert((closeEnough(std::midpoint(T(11.2345), T(14.5432)), T(12.88885))));
	
//	From e to pi
	assert((closeEnough(std::midpoint(T(2.71828182845904523536028747135266249775724709369995),
	                                  T(3.14159265358979323846264338327950288419716939937510)),
	                                  T(2.92993724102441923691146542731608269097720824653752))));
//										2.92993724102441923691146542731608269097720824653752
//	Denormalized values
    
//	Check two values "close to each other"
    T d1 = 3.14;
    T d0 = std::nexttoward(d1, T(2));
    T d2 = std::nexttoward(d1, T(5));
    assert(d0 < d1);  // sanity checking
    assert(d1 < d2);  // sanity checking

//	Since there's nothing in between, the midpoint has to be one or the other
	T res;
	res = std::midpoint(d0, d1);
	assert(res == d0 || res == d1);
	assert(d0 <= res);
	assert(res <= d1);
	res = std::midpoint(d1, d0);
	assert(res == d0 || res == d1);
	assert(d0 <= res);
	assert(res <= d1);

	res = std::midpoint(d1, d2);
	assert(res == d1 || res == d2);
	assert(d1 <= res);
	assert(res <= d2);
	res = std::midpoint(d2, d1);
	assert(res == d1 || res == d2);
	assert(d1 <= res);
	assert(res <= d2);
	
}

template <typename T>
void test (T a, T b)
{
	static_assert(std::is_same_v<T, decltype(std::midpoint(a, b))>, "");
	static_assert(noexcept(std::midpoint(a, b)), "");
	T mid0 = std::midpoint(a, b);
//	T mid1 = std::midpoint(b, a);
	T left_diff = mid0 - a;
	T right_diff = b - mid0;
	assert(left_diff + right_diff == b-a);
	T diff_diff = left_diff - right_diff; // should be at most 1
	assert(diff_diff == 0 || diff_diff == 1 || diff_diff  == -1);
}


int main ()
{
//     test(3, 4);
//     test(4, 3);
//     test(-4, -3);
//     test(-3, -4);
//     test(3u, 4u);
//     test(4u, 3u);
//     test(0, INT_MAX);
//     test(INT_MAX, 0);
//     test(0, INT_MIN+1);
//     test(INT_MIN+1, 0);
//     test(INT_MIN, INT_MAX);
//     test(INT_MAX, INT_MIN);
//     test(0u, UINT_MAX);
//     test(UINT_MAX, 0u);
    
    signed_int_test<signed char>();
    signed_int_test<short>();
    signed_int_test<int>();
    signed_int_test<long>();
    signed_int_test<long long>();

    signed_int_test<int8_t>();
    signed_int_test<int16_t>();
    signed_int_test<int32_t>();
    signed_int_test<int64_t>();
    signed_int_test<__int128_t>();

    unsigned_int_test<unsigned char>();
    unsigned_int_test<unsigned short>();
    unsigned_int_test<unsigned int>();
    unsigned_int_test<unsigned long>();
    unsigned_int_test<unsigned long long>();

    unsigned_int_test<uint8_t>();
    unsigned_int_test<uint16_t>();
    unsigned_int_test<uint32_t>();
    unsigned_int_test<uint64_t>();
    unsigned_int_test<__uint128_t>();

//     int_test<char>();
    signed_int_test<ptrdiff_t>();
    unsigned_int_test<size_t>();
    
    pointer_test<               char>();
    pointer_test<const          char>();
    pointer_test<      volatile char>();
    pointer_test<const volatile char>();
    
    pointer_test<int>();
    pointer_test<double>();
    
    fp_test<float>();
    fp_test<double>();
    fp_test<long double>();
}
