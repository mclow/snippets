// -*- C++ -*-
//===------------------------------ span ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//

#ifndef _LIBCPP_SPAN
#define _LIBCPP_SPAN

/*
    span synopsis

namespace std {
// constants
inline constexpr ptrdiff_t dynamic_extent = -1;

// [views.span], class template span
template <class ElementType, ptrdiff_t Extent = dynamic_extent> class span;

// [span.comparison], span comparison operators
template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
    constexpr bool operator==(span<T, X> l, span<U, Y> r);
template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
    constexpr bool operator!=(span<T, X> l, span<U, Y> r);
template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
    constexpr bool operator<(span<T, X> l, span<U, Y> r);
template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
    constexpr bool operator<=(span<T, X> l, span<U, Y> r);
template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
    constexpr bool operator>(span<T, X> l, span<U, Y> r);
template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
    constexpr bool operator>=(span<T, X> l, span<U, Y> r);

// [span.objectrep], views of object representation
template <class ElementType, ptrdiff_t Extent>
    span<const byte, ((Extent == dynamic_extent) ? dynamic_extent : 
        (static_cast<ptrdiff_t>(sizeof(ElementType)) * Extent))> as_bytes(span<ElementType, Extent> s) noexcept;

template <class ElementType, ptrdiff_t Extent>
    span<      byte, ((Extent == dynamic_extent) ? dynamic_extent : 
        (static_cast<ptrdiff_t>(sizeof(ElementType)) * Extent))> as_writable_bytes(span<ElementType, Extent> s) noexcept;


namespace std {
template <class ElementType, ptrdiff_t Extent = dynamic_extent>
class span {
public:
    // constants and types
    using element_type = ElementType;
    using value_type = remove_cv_t<ElementType>;
    using index_type = ptrdiff_t;
    using difference_type = ptrdiff_t;
    using pointer = element_type*;
    using reference = element_type&;
    using iterator = implementation-defined;
    using const_iterator = implementation-defined;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    static constexpr index_type extent = Extent;

    // [span.cons], span constructors, copy, assignment, and destructor
    constexpr span() noexcept;
    constexpr span(pointer ptr, index_type count);
    constexpr span(pointer firstElem, pointer lastElem);
    template <size_t N>
        constexpr span(element_type (&arr)[N]) noexcept;
    template <size_t N>
        constexpr span(array<value_type, N>& arr) noexcept;
    template <size_t N>
        constexpr span(const array<value_type, N>& arr) noexcept;
    template <class Container>
        constexpr span(Container& cont);
    template <class Container>
        constexpr span(const Container& cont);
    constexpr span(const span& other) noexcept = default;
    template <class OtherElementType, ptrdiff_t OtherExtent>
        constexpr span(const span<OtherElementType, OtherExtent>& s) noexcept;
    ~span() noexcept = default;
    constexpr span& operator=(const span& other) noexcept = default;

    // [span.sub], span subviews
    template <ptrdiff_t Count>
        constexpr span<element_type, Count> first() const;
    template <ptrdiff_t Count>
        constexpr span<element_type, Count> last() const;
    template <ptrdiff_t Offset, ptrdiff_t Count = dynamic_extent>
        constexpr span<element_type, see below> subspan() const;

    constexpr span<element_type, dynamic_extent> first(index_type count) const;
    constexpr span<element_type, dynamic_extent> last(index_type count) const;
    constexpr span<element_type, dynamic_extent> subspan(index_type offset, index_type count = dynamic_extent) const;

    // [span.obs], span observers
    constexpr index_type size() const noexcept;
    constexpr index_type size_bytes() const noexcept;
    constexpr bool empty() const noexcept;

    // [span.elem], span element access
    constexpr reference operator[](index_type idx) const;
    constexpr reference operator()(index_type idx) const;
    constexpr pointer data() const noexcept;

    // [span.iterators], span iterator support
    constexpr iterator begin() const noexcept;
    constexpr iterator end() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;
    constexpr reverse_iterator rbegin() const noexcept;
    constexpr reverse_iterator rend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

private:
    pointer data_;     // exposition only
    index_type size_;  // exposition only
};

template<class T, size_t N>
    span(T (&)[N]) -> span<T, N>;
  
template<class T, size_t N>
    span(array<T, N>&) -> span<T, N>;

template<class T, size_t N>
    span(const array<T, N>&) -> span<const T, N>;

template<class Container>
    span(Container&) -> span<typename Container::value_type>;

template<class Container>
    span(const Container&) -> span<const typename Container::value_type>;

} // namespace std

*/

#include <cstddef>      // for std::ptrdiff_t
#include <iterator>     // for iterators
#include <array>        // for std::array
#include <type_traits>  // for std::remove_cv, etc
#include <cstddef>      // for std::byte
#include <cassert>

inline constexpr std::ptrdiff_t dynamic_extent = -1;
template <typename _Tp, ptrdiff_t _Extent = dynamic_extent> class span;


template <class _Tp>
struct __is_span_impl : public std::false_type {};

template <class _Tp, ptrdiff_t _Extent>
struct __is_span_impl<span<_Tp, _Extent>> : public std::true_type {};

template <class _Tp>
struct __is_span : public __is_span_impl<std::remove_cv_t<_Tp>> {};


template <class _Tp>
struct __is_std_array_impl : public std::false_type {};

template <class _Tp, size_t _N>
struct __is_std_array_impl<std::array<_Tp, _N>> : public std::true_type {};

template <class _Tp>
struct __is_std_array : public __is_std_array_impl<std::remove_cv_t<_Tp>> {};


template <class _Tp, class _ElementType, class = void>
struct __is_span_compatible_container : public std::false_type {};

template <class _Tp, class _ElementType>
struct __is_span_compatible_container<_Tp, _ElementType,
        std::void_t<
		// is not a specialization of span
	   	 	typename std::enable_if<!__is_span<_Tp>::value, std::nullptr_t>::type,
		// is not a specialization of array
	   	 	typename std::enable_if<!__is_std_array<_Tp>::value, std::nullptr_t>::type,
		// data(cont) and size(cont) are well formed
			decltype(std::data(std::declval<_Tp>())),
			decltype(std::size(std::declval<_Tp>())),
		// remove_pointer_t<decltype(data(cont))>(*)[] is convertible to ElementType(*)[]
			typename std::enable_if<
				std::is_convertible_v<
					std::remove_pointer_t<decltype(std::data(std::declval<_Tp &>()))>(*)[],
																		 _ElementType(*)[]>,
				std::nullptr_t>::type
		>>
	: public std::true_type {};


template <typename _Tp, ptrdiff_t _Extent>
class span {
public:
//  constants and types
    using element_type           = _Tp;
    using value_type             = std::remove_cv_t<_Tp>;
    using index_type             = std::ptrdiff_t;
    using difference_type        = std::ptrdiff_t;
    using pointer                = _Tp*;
    using reference              = _Tp&;
    using iterator               = pointer;        // TODO libc++ wrap iterator
    using const_iterator         = const pointer;  // TODO libc++ wrap iterator
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr index_type extent = _Extent;
    static_assert (_Extent >= 0, "Can't have a span with an extent < 0");

// [span.cons], span constructors, copy, assignment, and destructor 
    constexpr span() : __data{nullptr}
        { static_assert(_Extent == 0); }

    constexpr span           (const span& __rhs) noexcept = default;
    constexpr span& operator=(const span& __rhs) noexcept = default;

    constexpr span(pointer __ptr, index_type __count) : __data{__ptr}
        { assert(_Extent == __count); }
    constexpr span(pointer __f, pointer __l) : __data{__f}
        { assert(_Extent == std::distance(__f, __l)); }

//  TODO This should not be a template - make _N and _Extent the same
    template <size_t _N>
        constexpr span(element_type (&__arr)[_N]) : __data{__arr}
        { static_assert(_Extent == _N); }

//  TODO This should not be a template - make _N and _Extent the same
    template <size_t _N>
        constexpr span(std::array<std::remove_const_t<element_type>, _N>& __arr)
        : __data{__arr.data()}
        { static_assert(_Extent == _N); }

//  TODO This should not be a template - make _N and _Extent the same
    template <size_t _N>
        constexpr span(const std::array<std::remove_const_t<element_type>, _N>& __arr)
        : __data{__arr.data()}
        { static_assert(_Extent == _N); }

    template <class _Container>
        constexpr span(      _Container& __c,
            const std::enable_if_t<__is_span_compatible_container<_Container, _Tp>::value, std::nullptr_t> = nullptr)
        : __data{std::data(__c)}
        { assert(_Extent == std::size(__c)); }

    template <class _Container>
        constexpr span(const _Container& __c,
            const std::enable_if_t<__is_span_compatible_container<const _Container, _Tp>::value, std::nullptr_t> = nullptr)
        : __data{std::data(__c)}
        { assert(_Extent == std::size(__c)); }


//  TODO This should not be a template - make _N and _Extent the same
//  Note: Weird to make a static extent span from a dynamic extent one
    template <class _OtherElementType, std::ptrdiff_t _OtherExtent>
        constexpr span(const span<_OtherElementType, _OtherExtent>& __other,
                       const std::enable_if_t<
                        std::is_convertible_v<_OtherElementType(*)[], element_type (*)[]>,
                        std::nullptr_t> = nullptr)
        : __data{__other.data()}
        {
            static_assert(_Extent == _OtherExtent || _OtherExtent == dynamic_extent);
            assert(_Extent == __other.size());
        }

    ~span() noexcept = default;

//  TODO make noexcept in libc++
    template <std::ptrdiff_t _Count>
    constexpr span<element_type, _Count> first() const
    {
        static_assert(_Count >= 0);
        assert(_Count <= size());
        return {data(), _Count};
    }

//  TODO make noexcept in libc++
    template <std::ptrdiff_t _Count>
    constexpr span<element_type, _Count> last() const
    {
        static_assert(_Count >= 0);
        assert(_Count <= size());
        return {data() + size() - _Count, _Count};
    }
    
//  TODO make noexcept in libc++
    constexpr span<element_type, dynamic_extent> first(index_type __count) const
    {
        assert(__count >= 0 && __count <= size());
        return {data(), __count};
    }

//  TODO make noexcept in libc++
    constexpr span<element_type, dynamic_extent> last(index_type __count) const
    {
        assert(__count >= 0 && __count <= size());
        return {data() + size() - __count, __count};
    }

//  TODO make noexcept in libc++
    template <std::ptrdiff_t _Offset, std::ptrdiff_t _Count = dynamic_extent>
    constexpr auto subspan() const
    -> span<element_type, _Count != dynamic_extent ? _Count : _Extent - _Offset>
    {
        assert(_Offset >= 0 && _Offset <= size());
        return {data() + _Offset, _Count == dynamic_extent ? size() - _Offset : _Count};
    }


//  TODO make noexcept in libc++
    constexpr span<element_type, dynamic_extent> subspan(index_type __offset, 
                                                         index_type __count = dynamic_extent) const
    { 
//  Still have to deal with count == -1 here
        assert( __offset >= 0 && __offset <= size());
        assert((__count  >= 0 && __count  <= size()) || __count == dynamic_extent);
        if (__count == dynamic_extent)
            return {data() + __offset, size() - __offset};
        assert(__offset + __count <= size());
        return {data() + __offset, __count};
    }

    constexpr index_type size()       const noexcept { return _Extent; }
    constexpr index_type size_bytes() const noexcept { return _Extent * sizeof(element_type); }
    constexpr bool empty()            const noexcept { return _Extent == 0; }

    constexpr reference operator[](index_type __idx) const { return __data[__idx]; } // TODO make noexcept in libc++
    constexpr reference operator()(index_type __idx) const { return __data[__idx]; } // TODO make noexcept in libc++
    constexpr pointer data()                         const noexcept { return __data; }

// [span.iter], span iterator support
    constexpr iterator                 begin() const noexcept { return data(); }
    constexpr iterator                   end() const noexcept { return data() + size(); }
    constexpr const_iterator          cbegin() const noexcept { return data(); }
    constexpr const_iterator            cend() const noexcept { return data() + size(); }
    constexpr reverse_iterator        rbegin() const noexcept { return reverse_iterator(end()); }
    constexpr reverse_iterator          rend() const noexcept { return reverse_iterator(begin()); }
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
    constexpr const_reverse_iterator   crend() const noexcept { return const_reverse_iterator(cbegin()); }

    span<const std::byte, _Extent * sizeof(element_type)> __as_bytes() const noexcept
    { return {reinterpret_cast<const std::byte *>(data()), size_bytes()}; }

    span<std::byte, _Extent * sizeof(element_type)> __as_writeable_bytes() const noexcept
    { return {reinterpret_cast<std::byte *>(data()), size_bytes()}; }

private:
    pointer    __data;

};


template <typename _Tp>
class span<_Tp, dynamic_extent> {
private:

public:
//  constants and types
    using element_type           = _Tp;
    using value_type             = std::remove_cv_t<_Tp>;
    using index_type             = std::ptrdiff_t;
    using difference_type        = std::ptrdiff_t;
    using pointer                = _Tp*;
    using reference              = _Tp&;
    using iterator               = pointer;        // TODO libc++ wrap iterator
    using const_iterator         = const pointer;  // TODO libc++ wrap iterator
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr index_type extent = dynamic_extent;

// [span.cons], span constructors, copy, assignment, and destructor 
    constexpr span() : __data{nullptr}, __size{0} {}

    constexpr span           (const span& __rhs) noexcept = default;
    constexpr span& operator=(const span& __rhs) noexcept = default;
    constexpr span(pointer __ptr, index_type __count) : __data{__ptr}, __size{__count} {}
    constexpr span(pointer __f, pointer __l) : __data{__f}, __size{std::distance(__f, __l)} {}

    template <size_t _N>
    constexpr span(element_type (&__arr)[_N]) : __data{__arr}, __size{_N} {}

    template <size_t _N>
    constexpr span(std::array<std::remove_const_t<element_type>, _N>& __arr)
        : __data{__arr.data()}, __size{_N} {}
    
    template <size_t _N>
        constexpr span(const std::array<std::remove_const_t<element_type>, _N>& __arr)
        : __data{__arr.data()}, __size{_N} {}

    template <class _Container>
        constexpr span(      _Container& __c,
            const std::enable_if_t<__is_span_compatible_container<_Container, _Tp>::value, std::nullptr_t> = nullptr)
        : __data{std::data(__c)}, __size{(index_type) std::size(__c)} {}

    template <class _Container>
        constexpr span(const _Container& __c,
            const std::enable_if_t<__is_span_compatible_container<const _Container, _Tp>::value, std::nullptr_t> = nullptr)
        : __data{std::data(__c)}, __size{(index_type) std::size(__c)} {}


    template <class _OtherElementType, std::ptrdiff_t _OtherExtent>
        constexpr span(const span<_OtherElementType, _OtherExtent>& __other,
                       const std::enable_if_t<
                        std::is_convertible_v<_OtherElementType(*)[], element_type (*)[]>,
                        std::nullptr_t> = nullptr)
        : __data{__other.data()}, __size{__other.size()} {}

    ~span() noexcept = default;

     // TODO make noexcept in libc++
    template <std::ptrdiff_t _Count>
    constexpr span<element_type, _Count> first() const
    {
        static_assert(_Count >= 0);
        assert(_Count <= size());
        return {data(), _Count};
    }

     // TODO make noexcept in libc++
    template <std::ptrdiff_t _Count>
    constexpr span<element_type, _Count> last() const
    {
        static_assert(_Count >= 0);
        assert(_Count <= size());
        return {data() + size() - _Count, _Count};
    }

     // TODO make noexcept in libc++
    constexpr span<element_type, dynamic_extent> first(index_type __count) const
    {
        assert(__count >= 0 && __count <= size());
        return {data(), __count};
    }
        
    constexpr span<element_type, dynamic_extent> last (index_type __count) const
    {
        assert(__count >= 0 && __count <= size());
        return {data() + size() - __count, __count};
    }

     // TODO make noexcept in libc++
    template <std::ptrdiff_t _Offset, std::ptrdiff_t _Count = dynamic_extent>
    constexpr span<_Tp, dynamic_extent> subspan() const
    {
        assert(_Offset >= 0 && _Offset <= size());
        assert(_Count == dynamic_extent || _Offset + _Count <= size());
        return {data() + _Offset, _Count == dynamic_extent ? size() - _Offset : _Count};
    }


//  TODO make noexcept in libc++
    constexpr span<element_type, dynamic_extent> subspan(index_type __offset, 
                                                         index_type __count = dynamic_extent) const
    { 
        assert( __offset >= 0 && __offset <= size());
        assert((__count  >= 0 && __count  <= size()) || __count == dynamic_extent);
        if (__count == dynamic_extent)
            return {data() + __offset, size() - __offset};
        assert(__offset + __count <= size());
        return {data() + __offset, __count};
    }

    constexpr index_type size()       const noexcept { return __size; }
    constexpr index_type size_bytes() const noexcept { return __size * sizeof(element_type); }
    constexpr bool empty()            const noexcept { return __size == 0; }

    constexpr reference operator[](index_type __idx) const { return __data[__idx]; } // TODO make noexcept in libc++
    constexpr reference operator()(index_type __idx) const { return __data[__idx]; } // TODO make noexcept in libc++
    constexpr pointer data()                         const noexcept { return __data; }

// [span.iter], span iterator support
    constexpr iterator                 begin() const noexcept { return data(); }
    constexpr iterator                   end() const noexcept { return data() + size(); }
    constexpr const_iterator          cbegin() const noexcept { return data(); }
    constexpr const_iterator            cend() const noexcept { return data() + size(); }
    constexpr reverse_iterator        rbegin() const noexcept { return reverse_iterator(end()); }
    constexpr reverse_iterator          rend() const noexcept { return reverse_iterator(begin()); }
    constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
    constexpr const_reverse_iterator   crend() const noexcept { return const_reverse_iterator(cbegin()); }

    span<const std::byte, dynamic_extent> __as_bytes() const noexcept
    { return {reinterpret_cast<const std::byte *>(data()), size_bytes()}; }

    span<std::byte, dynamic_extent> __as_writeable_bytes() const noexcept
    { return {reinterpret_cast<std::byte *>(data()), size_bytes()}; }

private:
    pointer    __data;
    index_type __size;
};

template <class _Tp1, class _Tp2, class = void>
struct __is_equality_comparable : public std::false_type {};

template <class _Tp1, class _Tp2>
struct __is_equality_comparable<
        _Tp1, _Tp2, 
        std::void_t<decltype(std::declval<const _Tp1&>() == std::declval<const _Tp2 &>())>>
        : public std::true_type {};

template <class _Tp1, class _Tp2, class = void>
struct __is_less_than_comparable : public std::false_type {};

template <class _Tp1, class _Tp2>
struct __is_less_than_comparable<
        _Tp1, _Tp2, 
        std::void_t<decltype(std::declval<const _Tp1&>() < std::declval<const _Tp2&>())>>
        : public std::true_type {};

//  TODO - update P0805 - SFINAE away the comparisons if the underlying types are not comparable
template <class _Tp1, std::ptrdiff_t _Extent1, class _Tp2, std::ptrdiff_t _Extent2>
    constexpr typename std::enable_if<__is_equality_comparable<_Tp1, _Tp2>::value, bool>::type
    operator==(const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return std::equal(__lhs.begin(), __lhs.end(), __rhs.begin(), __rhs.end()); }

template <class _Tp1, std::ptrdiff_t _Extent1, class _Tp2, std::ptrdiff_t _Extent2>
    constexpr typename std::enable_if<__is_equality_comparable<_Tp1, _Tp2>::value, bool>::type
    operator!=(const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return !(__rhs == __lhs); }

template <class _Tp1, std::ptrdiff_t _Extent1, class _Tp2, std::ptrdiff_t _Extent2>
    constexpr typename std::enable_if<__is_less_than_comparable<_Tp1, _Tp2>::value, bool>::type
    operator< (const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return std::lexicographical_compare (__lhs.begin(), __lhs.end(), __rhs.begin(), __rhs.end()); }
    
template <class _Tp1, std::ptrdiff_t _Extent1, class _Tp2, std::ptrdiff_t _Extent2>
    constexpr typename std::enable_if<__is_less_than_comparable<_Tp2, _Tp1>::value, bool>::type
    operator<=(const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return !(__rhs < __lhs); }

template <class _Tp1, std::ptrdiff_t _Extent1, class _Tp2, std::ptrdiff_t _Extent2>
    constexpr typename std::enable_if<__is_less_than_comparable<_Tp2, _Tp1>::value, bool>::type
    operator> (const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return __rhs < __lhs; }
    
template <class _Tp1, std::ptrdiff_t _Extent1, class _Tp2, std::ptrdiff_t _Extent2>
    constexpr typename std::enable_if<__is_less_than_comparable<_Tp1, _Tp2>::value, bool>::type
    operator>=(const span<_Tp1, _Extent1>& __lhs, const span<_Tp2, _Extent2>& __rhs)
    { return !(__lhs < __rhs); }


//  as_bytes & as_writeable_bytes
template <class _Tp, ptrdiff_t _Extent>
    auto as_bytes(span<_Tp, _Extent> __s) noexcept 
    -> decltype(__s.__as_bytes())
    { return __s.__as_bytes(); }

template <class _Tp, ptrdiff_t _Extent>
    auto as_writeable_bytes(span<_Tp, _Extent> __s) noexcept
    -> typename std::enable_if<!std::is_const_v<_Tp>, decltype(__s.__as_writeable_bytes())>::type
    { return __s.__as_writeable_bytes(); }

//  Deduction guides
template<class _Tp, size_t _N>
    span(_Tp (&)[_N]) -> span<_Tp, _N>;
  
template<class _Tp, size_t _N>
    span(std::array<_Tp, _N>&) -> span<_Tp, _N>;

template<class _Tp, size_t _N>
    span(const std::array<_Tp, _N>&) -> span<const _Tp, _N>;

template<class _Container>
    span(_Container&) -> span<typename _Container::value_type>;

template<class _Container>
    span(const _Container&) -> span<const typename _Container::value_type>;

#endif // _LIBCPP_SPAN



#include <string>
#include <array>
#include <vector>
#include <list>

int main ()
{
    span<int> s0;
    assert(s0.size() == 0);
    span<int, 0> s1;
    assert(s1.size() == 0);
    assert (s0 == s1);
    assert (!(s0 < s1));
    
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

    auto b2 = as_bytes(s2);
    auto b2w = as_writeable_bytes(s2);
    assert((void *) b2.data() == (void *)s2.data());
    assert(b2.size()/sizeof(int) == s2.size());
    assert((void *) b2w.data() == (void *) s3.data());
    assert(b2w.size()/sizeof(int) == s3.size());

    auto b3 = as_bytes(s3);
    auto b3w = as_writeable_bytes(s3);
    assert((void *) b3.data() == (void *) s3.data());
    assert(b3.size()/sizeof(int) == s3.size());
    assert((void *) b3w.data() == (void *) s3.data());
    assert(b3w.size()/sizeof(int) == s3.size());

    span<const int, 0> s4;
    span<const int> s5;
    auto b4 = as_bytes(s4);
    auto b5 = as_bytes(s5);
    assert(b4.data() == nullptr);
    assert(b5.data() == nullptr);
    assert(b4.size() == 0);
    assert(b5.size() == 0);
//  auto b4w = as_writeable_bytes(s4);
//  auto b5w = as_writeable_bytes(s5);

    auto ss1 = s2.subspan<0, -1>(); assert(ss1.size() == 1);
    auto ss2 = s2.subspan<0,  1>(); assert(ss2.size() == 1);
    auto ss3 = s2.subspan<1,  0>(); assert(ss3.size() == 0);
    auto ss4 = s2.subspan<1, -1>(); assert(ss4.size() == 0);

    auto ss5 = s3.subspan<0, -1>(); assert(ss5.size() == 1);
    auto ss6 = s3.subspan<0,  1>(); assert(ss6.size() == 1);
    auto ss7 = s3.subspan<1,  0>(); assert(ss7.size() == 0);
    auto ss8 = s3.subspan<1, -1>(); assert(ss8.size() == 0);

//  Converting
    span<int, 0>       convs0 = s0;     // dynamic -> static extent
    span<const int, 0> convs1 = s1;     // non-const -> const
    span<int>          convs2 = convs0; // static -> dynamic extent
//  span<int, 1>       convs4 = s4;     // const -> non-const  FAILS

//  Deduce from array
    {
    int arr[] = { 1, 2, 3, 4, 5};
    span s6 = arr;
    static_assert(std::is_same_v<int, decltype(s6)::value_type>);
    static_assert(std::is_same_v<int, decltype(s6)::element_type>);
    assert(s6.size() == std::size(arr));
    assert(s6.data() == arr);

    span<int, 5> s6f = arr;
    span<int>    s6d = arr;
    span<const int, 5> s6cf = arr;
    span<const int>    s6cd = arr;
    
    const int carr[] = { 1, 2, 3, 4};
    span s7 = carr;
    static_assert(std::is_same_v<      int, decltype(s7)::value_type>);
    static_assert(std::is_same_v<const int, decltype(s7)::element_type>);
    assert(s7.size() == std::size(carr));
    assert(s7.data() == carr);

//  span<int, 4> s7f = carr;
//  span<int>    s7d = carr;
    span<const int, 4> s7cf = carr;
    span<const int>    s7cd = carr;
    }
    
//  Deduce from std::array
    {
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    span s6 = arr;
    static_assert(std::is_same_v<int, decltype(s6)::value_type>);
    static_assert(std::is_same_v<int, decltype(s6)::element_type>);
    assert(s6.size() == std::size(arr));
    assert(s6.data() == arr.data());

    span<int, 5> s6f = arr;
    span<int>    s6d = arr;
    span<const int, 5> s6cf = arr;
    span<const int>    s6cd = arr;

    const std::array<int, 4> carr = {1, 2, 3, 4};
    span s7 = carr;
    static_assert(std::is_same_v<      int, decltype(s7)::value_type>);
    static_assert(std::is_same_v<const int, decltype(s7)::element_type>);
    assert(s7.size() == std::size(carr));
    assert(s7.data() == carr.data());

//  span<int, 4> s7f = carr;
//  span<int>    s7d = carr;
    span<const int, 4> s7cf = carr;
    span<const int>    s7cd = carr;
    }

    {
    static_assert(!__is_equality_comparable<char, std::string>::value, "" );
    static_assert( __is_equality_comparable<char, int>::value, "" );
    static_assert(!__is_equality_comparable<span<char>, span<std::string>>::value, "");
    static_assert( __is_equality_comparable<span<char>, span<int>>::value, "" );
    }

    {
    static_assert( __is_span_compatible_container<std::vector<int>, int>::value);
    static_assert( __is_span_compatible_container<const std::vector<int>, const int>::value);
    static_assert(!__is_std_array<std::list<int>>::value);
    static_assert( __is_std_array<std::array<int, 5>>::value);
    static_assert(!__is_std_array<span<int, 5>>::value);
    static_assert(!__is_std_array<span<int>>::value);
    static_assert(!__is_span<std::list<int>>::value);
    static_assert(!__is_span<std::array<int, 5>>::value);
    static_assert( __is_span<span<int, 5>>::value);
    static_assert( __is_span<span<int>>::value);
    static_assert(!__is_span_compatible_container<std::array<int, 5>, int>::value);
    static_assert(!__is_span_compatible_container<std::vector<int>, std::string>::value);
    static_assert(!__is_span_compatible_container<std::list<int>, int>::value);
    }

    {
        std::vector<int> v1;
        const std::vector<int> v2;
        span<int> s1 = v1;
        span<const int> sc1 = v1;
        span<int, 0> s1f = v1;
        span<const int, 0> sc1f = v1;
        span sd1 = v1;

//      span<int> s2 = v2; // fails
//      span<int, 0> s2f = v2; // fails
        span<const int> s2 = v2;
        span<const int, 0> s2f = v2;
        span sd2 = v2;
    }
        
}
