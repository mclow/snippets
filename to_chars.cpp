#include <system_error>
#include <type_traits>
#include <cassert>
#include <string_view>
#include <iostream>

using namespace std::string_view_literals;

  struct to_chars_result {
    char* ptr;
    std::errc ec;
  };
//	Useful bits

  constexpr std::string_view all_digits = "0123456789abcdefghijklmnopqrstuvwxyz"sv;
//   constexpr std::string_view dec_digits{all_digits.data(), 10};
//   constexpr std::string_view oth_digits{all_digits.data() + 10, 26};
 

  template <typename _Tp>
  to_chars_result __to_chars_positive(char* first, size_t dist, _Tp value, int base)
  {
  char buffer[100];
  char *p = buffer + dist;

  while (value > 0)
  {
  	if (p == buffer)
  		return to_chars_result{first + dist, std::errc::value_too_large};
  	*--p = all_digits[value % base];
  	value /= base;
  }

  const size_t len = buffer + dist - p;
  std::memcpy(first, p, len);
  return to_chars_result{first + len, {}};
  }


//	The enable_if here should cover:
//				for all signed and unsigned integer types and char
//		is_integral also handles bool. TODO fix that.
	template <typename _Tp>
	typename std::enable_if<std::is_integral<_Tp>::value, to_chars_result>::type
	to_chars(char* first, char* last, _Tp value, int base = 10)
	{
//	This is a requirement; strictly speaking, we don't need to test for it
	if ((base < 2) || (base > 36))
		return to_chars_result{first, std::errc::invalid_argument};
		
	const size_t dist = std::distance(first, last);
	if (dist == 0)
		return to_chars_result{last, std::errc::value_too_large};

	if (value == 0)
	{
		*first = '0';
		return to_chars_result{first + 1, {}};
	}

	if constexpr (std::numeric_limits<_Tp>::is_signed)
	{
		if (value < 0)
		{
			if (value == std::numeric_limits<_Tp>::min())
			{
				int rem = -(value % base); // last 'digit'
				*first = '-';
				to_chars_result r = __to_chars_positive(++first, 
											dist-1, -(value / base), base);
				if (r.ec != std::errc{})
					return r;
				if (r.ptr == last) 
					return to_chars_result{last, std::errc::value_too_large};
				*r.ptr++ = all_digits[rem];
				return r;
			}
			else
			{
				*first = '-';
				return __to_chars_positive(++first, dist-1, -value, base);
			}
		}
	}	
	return __to_chars_positive(first, dist, value, base);
  }


  struct from_chars_result {
    const char* ptr;
    std::errc ec;
  };


  template <typename _Tp>
  from_chars_result
  __from_chars_unsigned(const char* first, const char* last, _Tp& value, int base)
  {
	if (first == last)
		return from_chars_result{last, std::errc::invalid_argument};

	from_chars_result ret;
	const std::string_view digits{all_digits.data(), static_cast<size_t>(base)};
	const _Tp limit = std::numeric_limits<_Tp>::max() / base;
// 	std::cout << "Limit = " << (long) limit << '\n';
	
//	Look at the first digit
	auto idx = digits.find(*first);
	if ( idx == std::string_view::npos)
		return from_chars_result{first, std::errc::invalid_argument};
	value = idx;

//	And all the rest
	while (++first != last) {
		idx = digits.find(*first);
		if (idx == std::string_view::npos)
			break;
	//	This test is not perfect; it will catch 130 for char, but not 128/129
// 		std::cout << "Value = " << (long) value << '\n';
		if (value > limit)
			return from_chars_result{first, std::errc::result_out_of_range};
		value = value * base + idx;
		}
	return from_chars_result{first, std::errc{}};
  }


  template <typename _Tp>
  typename std::enable_if<std::is_integral<_Tp>::value, from_chars_result>::type
  from_chars(const char* first, const char* last, _Tp& value, int base = 10)
  {
//	This is a requirement; strictly speaking, we don't need to test for it
	if ((base < 2) || (base > 36))
		return from_chars_result{last, std::errc::invalid_argument};

	if (first == last)
		return from_chars_result{last, std::errc::invalid_argument};

	if constexpr (!std::numeric_limits<_Tp>::is_signed)
		return __from_chars_unsigned(first, last, value, base);

	using _UTp = std::make_unsigned_t<_Tp>;
	_UTp __val;	
	from_chars_result ret;
	if (*first == '-')
		ret = __from_chars_unsigned(first + 1, last, __val, base);
	else
		ret = __from_chars_unsigned(first,     last, __val, base);
// 	std::cout << "Calculated " << (long) __val << std::endl;
	
	if (ret.ec == std::errc{})
	{
		if (*first == '-')
		{
// 			if (__val >= std::numeric_limits<_Tp>::min())
				value = -__val;
// 			else
// 				return from_chars_result{ret.ptr, std::errc::result_out_of_range};
		}
		else
		{
			if (__val <= std::numeric_limits<_Tp>::max())
				value = __val;
			else
				return from_chars_result{ret.ptr, std::errc::result_out_of_range};
		}
	}
	return ret;	
  }

//   to_chars_result to_chars(char* first, char* last, float       value);
//   to_chars_result to_chars(char* first, char* last, double      value);
//   to_chars_result to_chars(char* first, char* last, long double value);
// 
//   to_chars_result to_chars(char* first, char* last, float       value, chars_format fmt);
//   to_chars_result to_chars(char* first, char* last, double      value, chars_format fmt);
//   to_chars_result to_chars(char* first, char* last, long double value, chars_format fmt);
// 
//   to_chars_result to_chars(char* first, char* last, float       value, chars_format fmt, int precision);
//   to_chars_result to_chars(char* first, char* last, double      value, chars_format fmt, int precision);
//   to_chars_result to_chars(char* first, char* last, long double value, chars_format fmt, int precision);
// 
//   from_chars_result from_chars(const char* first, const char* last, float& value, chars_format fmt = chars_format::general);
//   from_chars_result from_chars(const char* first, const char* last, double& value, chars_format fmt = chars_format::general);
//   from_chars_result from_chars(const char* first, const char* last, long double& value, chars_format fmt = chars_format::general);

int digits(unsigned long long value, int base) {
	const unsigned long long max = std::numeric_limits<unsigned long long>::max() / base;
	unsigned long long ctr = base;
	int power = 1;
	do {
		if (value < ctr) return power;
		ctr *= base;
		power++;
		} while (ctr < max);
	return power;
	}

template <typename T>
int out_size(T value, int base)
{
	if (std::numeric_limits<T>::is_signed && value < 0)
		return 1 + digits(
			value == std::numeric_limits<T>::min() ? std::numeric_limits<T>::max() : -value, base);

	return digits(value, base);
}

template <typename T>
bool test_success(T value, size_t len, const char *expected, int base = 10)
{
	char b[100];
	assert(len < sizeof(b));		// sanity
	auto r = to_chars(b, b + 100, value, base);
// 	for (const char *p = b; p < r.ptr; ++p ) std::cout << *p;
// 	std::cout << '\n';
	assert(r.ec      == std::errc{});	// success!
	assert(r.ptr - b == strlen(expected));
	return std::equal(b, r.ptr, expected);
}

template <typename T>
void test_failure(T value, int len, std::errc error, int base = 10)
{
	char b[100];
	assert(len < sizeof(b));		// sanity
	auto r = to_chars(b, b + len, value, base);
	assert(r.ec == error);
	assert(r.ptr == b + len);
}

template <typename T>
void test_length(T value, int base, const char *expected)
{
	char b[100];
	const size_t len = strlen(expected);
	assert(len < sizeof(b));		// sanity
	to_chars_result r;
	for (size_t sz = 0; sz < sizeof(b); ++sz)
	{
		r = to_chars(b, b + sz, value, base);
		if (sz >= len)
 			assert(std::equal(b, r.ptr, expected));
 		else
		{
			assert(r.ec == std::errc::value_too_large);
			assert(r.ptr == b + sz);
		}
	}			
}

template <typename T>
void round_trip(int base)
{
	static_assert(sizeof(T) < sizeof(long long), "" );
	T val;
	char buffer[100];
	
	for (long long i  = std::numeric_limits<T>::min();
				   i <= std::numeric_limits<T>::max(); ++i ) {
		auto r1 = to_chars<T>(buffer, buffer + 100, static_cast<T>(i), base);
		assert(r1.ec == std::errc{});
		auto r2 = from_chars(buffer, r1.ptr, val, base);
		assert(r1.ec == std::errc{});
		if (val != i)
			std::cout << "Round tripping failed for " << i << " (base " << base << ")" << std::endl;
//  		if (i % 100000 == 0) std::cerr << '.';
		}
}
		
int main ()
{
	test_failure(123, 0, std::errc::value_too_large);
	test_failure(123, 1, std::errc::value_too_large);
 	test_failure(123, 2, std::errc::value_too_large);
 	assert(test_success(123, 3,  "123"));

	test_failure<char>(-128, 3, std::errc::value_too_large);
	test_failure<int> (-128, 3, std::errc::value_too_large);
	test_failure<long>(-128, 3, std::errc::value_too_large);
	assert(test_success<char>(-128, 4, "-128"));
	assert(test_success<int> (-128, 4, "-128"));
	assert(test_success<long>(-128, 4, "-128"));
	
//	various bases
	assert(test_success(  0, 20,  "0"));
	assert(test_success( -0, 20,  "0"));
	assert(test_success(133, 20, "133"));
	assert(test_success(133, 20,  "85", 16));
	assert(test_success(133, 20,  "7e", 17));
	assert(test_success(133, 20,  "6d", 20));
	assert(test_success(133, 20,  "53", 26));

//	various bases - negative numbers
	assert(test_success(-133, 20, "-133"));
	assert(test_success(-133, 20,  "-85", 16));
	assert(test_success(-133, 20,  "-7e", 17));
	assert(test_success(-133, 20,  "-6d", 20));
	assert(test_success(-133, 20,  "-53", 26));
	
	assert(test_success<char>(-128, 20, "-128"));
	assert(test_success<int> (-128, 20, "-128"));
	assert(test_success<long>(-128, 20, "-128"));
	
//	CHAR_MIN, but not as a char
	assert(std::numeric_limits<signed char>::min() == -128);
	assert(test_success(-128, 20, "-10000000", 2));
	assert(test_success(-128, 20,    "-11202", 3));
	assert(test_success(-128, 20,     "-2000", 4));
	assert(test_success(-128, 20,     "-1003", 5));
	assert(test_success(-128, 20,      "-332", 6));
	assert(test_success(-128, 20,      "-242", 7));
	assert(test_success(-128, 20,      "-200", 8));

//	CHAR_MIN, as a char
	assert(test_success<signed char>(-128, 20, "-10000000", 2));
	assert(test_success<signed char>(-128, 20,    "-11202", 3));
	assert(test_success<signed char>(-128, 20,     "-2000", 4));
	assert(test_success<signed char>(-128, 20,     "-1003", 5));
	assert(test_success<signed char>(-128, 20,      "-332", 6));
	assert(test_success<signed char>(-128, 20,      "-242", 7));
	assert(test_success<signed char>(-128, 20,      "-200", 8));

	test_length<char>         (      123, 10,       "123");
	test_length<signed char>  (     -128, 10,      "-128");
	test_length<int>          (    12345, 10,     "12345");
	test_length<unsigned long>(123456789, 10, "123456789");
	
// 	std::cout << "Quo: " << -128 / 10 << std::endl;
// 	std::cout << "Rem: " << -128 % 10 << std::endl;
	
	{
	const char *foo = "-123Q";
	int val;
	{
		auto re = from_chars(foo, foo, val);
		assert(re.ec == std::errc::invalid_argument);
		assert(re.ptr == foo);
	}
	{
		auto re = from_chars(foo+1, foo+2, val);
		assert(re.ec == std::errc{});
		assert(re.ptr == foo + 2);
		assert(val == 1);
	}
	{
		auto re = from_chars(foo+1, foo+3, val);
		assert(re.ec == std::errc{});
		assert(re.ptr == foo + 3);
		assert(val == 12);
	}
	{
		auto re = from_chars(foo+1, foo+4, val);
		assert(re.ec == std::errc{});
		assert(re.ptr == foo + 4);
		assert(val == 123);
	}
	{
		auto re = from_chars(foo+1, foo+5, val);
		assert(re.ec == std::errc{});
		assert(re.ptr == foo + 4);
		assert(val == 123);
	}
//	Negative numbers
	{
		auto re = from_chars(foo, foo+1, val);
		assert(re.ec == std::errc::invalid_argument);
		assert(re.ptr == foo + 1);
	}
	{
		auto re = from_chars(foo, foo+2, val);
		assert(re.ec == std::errc{});
		assert(re.ptr == foo + 2);
		assert(val == -1);
	}
	{
		auto re = from_chars(foo, foo+3, val);
		assert(re.ec == std::errc{});
		assert(re.ptr == foo + 3);
		assert(val == -12);
	}
	{
		auto re = from_chars(foo, foo+4, val);
		assert(re.ec == std::errc{});
		assert(re.ptr == foo + 4);
		assert(val == -123);
	}

	{
		auto re = from_chars(foo, foo+5, val);
		assert(re.ec == std::errc{});
		assert(re.ptr == foo + 4);
		assert(val == -123);
	}

// 	{
// 		const char *cMin = "-2000";
// 		char cVal;
// 		auto re = from_chars<char>(cMin, cMin+5, cVal, 4);
// 		assert(re.ec == std::errc{});
// 		assert(re.ptr == cMin + 5);
// 		std::cout << "Value == " << (int) cVal << std::endl;
// // 		assert(val == -123);
// 	}

	for (int i = 2; i <= 36; ++i)
	{
		round_trip<char>(i);
		round_trip<unsigned char>(i);
	}
	
	for (int i = 2; i <= 36; ++i)
	{
		round_trip<short>(i);
		round_trip<unsigned short>(i);
	}

// 	round_trip<short>(10);
// 	round_trip<unsigned short>(10);

// 	round_trip<int>(10);
// 	round_trip<unsigned int>(10);

	}

}
