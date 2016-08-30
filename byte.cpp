/* An implementation of the byte type proposed in http://wg21.link/P0298
 *
 * (C) Copyright Marshall Clow 2016
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See http://www.boost.org/LICENSE_1_0.txt)
 *
 */
 
#include <type_traits>

namespace me {
	enum class byte : unsigned char {};
//	unsigned char __to_char(byte __b) { return static_cast<unsigned char>(__b); }
	
	template <class _Integer>
	  constexpr typename std::enable_if<std::is_integral_v<_Integer>, byte>::type &
	  operator<<=(byte& __lhs, _Integer __shift) noexcept
	  { return __lhs = byte(static_cast<unsigned char>(__lhs) << __shift); }
	  
	template <class _Integer>
	  constexpr typename std::enable_if<std::is_integral_v<_Integer>, byte>::type
	  operator<< (byte  __lhs, _Integer __shift) noexcept
	  { return         byte(static_cast<unsigned char>(__lhs) << __shift); }

	template <class _Integer>
	  constexpr typename std::enable_if<std::is_integral_v<_Integer>, byte>::type &
	  operator>>=(byte& __lhs, _Integer __shift) noexcept
	  { return __lhs = byte(static_cast<unsigned char>(__lhs) >> __shift); }

	template <class _Integer>
	  constexpr typename std::enable_if<std::is_integral_v<_Integer>, byte>::type
	  operator>> (byte  __lhs, _Integer __shift) noexcept
	  { return         byte(static_cast<unsigned char>(__lhs) >> __shift); }
	  
	constexpr byte& operator|=(byte& __lhs, byte __rhs) noexcept
	{ return __lhs = byte(static_cast<unsigned char>(__lhs) | static_cast<unsigned char>(__rhs)); }
	constexpr byte  operator| (byte  __lhs, byte __rhs) noexcept
	{ return         byte(static_cast<unsigned char>(__lhs) | static_cast<unsigned char>(__rhs)); }

	constexpr byte& operator&=(byte& __lhs, byte __rhs) noexcept
	{ return __lhs = byte(static_cast<unsigned char>(__lhs) & static_cast<unsigned char>(__rhs)); }
	constexpr byte  operator& (byte  __lhs, byte __rhs) noexcept
	{ return         byte(static_cast<unsigned char>(__lhs) & static_cast<unsigned char>(__rhs)); }

	constexpr byte& operator^=(byte& __lhs, byte __rhs) noexcept 
	{ return __lhs = byte(static_cast<unsigned char>(__lhs) ^ static_cast<unsigned char>(__rhs)); }
	constexpr byte  operator^ (byte  __lhs, byte __rhs) noexcept
	{ return         byte(static_cast<unsigned char>(__lhs) ^ static_cast<unsigned char>(__rhs)); }

	constexpr byte  operator~ (byte __b) noexcept
	{ return  byte(~static_cast<unsigned char>(__b)); }
// 	constexpr bool operator! (byte __b) noexcept
// 	{ return !static_cast<unsigned char>(__b); }

	template <class _Integer>
	  constexpr typename std::enable_if<std::is_integral_v<_Integer>, _Integer>::type
	  to_integer(byte __b) noexcept { return _Integer(__b); }
}

#include <cassert>
#include <iostream>

int main () {
	using me::byte;
	
	byte b1;
	byte b2{1};
	byte b3{3};
	assert(!(b1 == b2));
	assert(!(b1 == b3));

	assert(1 == me::to_integer<int>(b2));
	assert(3 == me::to_integer<int>(b3));

	assert(3 == me::to_integer<int>(b2|b3));
	assert(1 == me::to_integer<int>(b2&b3));

	b2 <<= 4;
	assert(16 == me::to_integer<int>(b2));
	assert(12 == me::to_integer<int>(b3 << 2));

	b2 |= byte{4};
	assert(20 == me::to_integer<int>(b2));

	b2 &= byte{16};
	assert(16 == me::to_integer<int>(b2));

	b2 ^= byte{24};
	assert(8 == me::to_integer<int>(b2));
}
