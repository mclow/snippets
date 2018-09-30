#include <string_view>
#include <iterator>

namespace somestd {

/*
Notes:
// These two need "typename"
    using size_type = view::size_type;
    using difference_type = view::difference_type;

//  iterator and const_iterator should be "implementation-defined"
    
//  I'd really rather that 'view' be called 'view_type'
//  where are the literals? --> "abc"fs

All the concat operators mix up L/R and M/N
Why do you use "N - 1 + M" in one case, and "N + M - 1" in another?

Should the template parameter "N" have to be > 0?
If not, what happens when you concat two empty strings?
Do you get a string of size_t(-1)?

What's that 'L' doing in swap?

constexpr unsigned long stoul(const fixed_string<N>& str, int base = 10;
--> missing a closing parenthesis

Don't use unicode in the code font - makes copy/paste hard.
"constexpr basic_fixed_string<charT, N ­ 1>" contains a unicode '-'

* I don't think you've thought through all the corner cases for make_fixed_string.
Consider:
   char arr5[5] = "ABCDE";
   char arrN[]  = "ABCDE";
   char arr9[9] = "ABCDE";

   auto fs5 = make_fixed_string(arr5); // what is fs5.size()?
   auto fsN = make_fixed_string(arrN); // what is fsN.size()?
   auto fs9 = make_fixed_string(arr9); // what is fs9.size()?
   
*/

namespace _STD = _VSTD;


template <class _CharT, size_t _Sz>
    class basic_fixed_string;

//	basic_fixed_string nonmember concatenation functions
template <class _CharT, size_t L, size_t R>
    constexpr basic_fixed_string<_CharT, L + R> 
    operator+(const basic_fixed_string<_CharT, L>& lhs,
              const basic_fixed_string<_CharT, R>& rhs) noexcept;

template <class _CharT, size_t L, size_t R>
    constexpr basic_fixed_string<_CharT, L + R - 1>
    operator+(const _CharT(&lhs)[L], 
              const basic_fixed_string<_CharT, R> &rhs) noexcept;

template <class _CharT, size_t _Sz>
    constexpr basic_fixed_string<_CharT, _Sz + 1>
    operator+(_CharT lhs, const basic_fixed_string<_CharT, _Sz>& rhs) noexcept;


template <class _CharT, size_t L, size_t R>
	constexpr basic_fixed_string<_CharT, L + R - 1>
	operator+(const basic_fixed_string<_CharT, L> &lhs,
	          const _CharT(&rhs)[R]) noexcept;

template <class _CharT, size_t _Sz>
	constexpr basic_fixed_string<_CharT, _Sz + 1>
	operator+(const basic_fixed_string<_CharT, _Sz>& lhs, _CharT rhs) noexcept;


//	basic_fixed_string nonmember comparison functions
template <class _CharT, size_t L, size_t R>
    constexpr bool operator==(const basic_fixed_string<_CharT, L> &__lhs,
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept;

template <class _CharT, size_t L, size_t R>
	constexpr bool operator==(const _CharT(&__lhs)[L],
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept;

template <class _CharT, size_t L, size_t R>
	constexpr bool operator==(const basic_fixed_string<_CharT, L> &__lhs,
                              const _CharT(&__rhs)[R]) noexcept;


template <class _CharT, size_t L, size_t R>
    constexpr bool operator!=(const basic_fixed_string<_CharT, L> &__lhs,
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept
	{ return !(__lhs == __rhs);}
	
template <class _CharT, size_t L, size_t R>
	constexpr bool operator!=(const _CharT(&__lhs)[L],
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept
	{ return !(__lhs == __rhs);}

template <class _CharT, size_t L, size_t R>
	constexpr bool operator!=(const basic_fixed_string<_CharT, L> &__lhs,
                              const _CharT(&__rhs)[R]) noexcept
	{ return !(__lhs == __rhs);}


template <class _CharT, size_t L, size_t R>
    constexpr bool operator< (const basic_fixed_string<_CharT, L> &__lhs,
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept;

template <class _CharT, size_t L, size_t R>
	constexpr bool operator< (const _CharT(&__lhs)[L],
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept;

template <class _CharT, size_t L, size_t R>
	constexpr bool operator< (const basic_fixed_string<_CharT, L> &__lhs,
                              const _CharT(&__rhs)[R]) noexcept;


template <class _CharT, size_t L, size_t R>
    constexpr bool operator> (const basic_fixed_string<_CharT, L> &__lhs,
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept;

template <class _CharT, size_t L, size_t R>
	constexpr bool operator> (const _CharT(&__lhs)[L],
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept;

template <class _CharT, size_t L, size_t R>
	constexpr bool operator> (const basic_fixed_string<_CharT, L> &__lhs,
                              const _CharT(&__rhs)[R]) noexcept;


template <class _CharT, size_t L, size_t R>
    constexpr bool operator<=(const basic_fixed_string<_CharT, L> &__lhs,
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept;

template <class _CharT, size_t L, size_t R>
	constexpr bool operator<=(const _CharT(&__lhs)[L],
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept;

template <class _CharT, size_t L, size_t R>
	constexpr bool operator<=(const basic_fixed_string<_CharT, L> &__lhs,
                              const _CharT(&__rhs)[R]) noexcept;

template <class _CharT, size_t L, size_t R>
    constexpr bool operator>=(const basic_fixed_string<_CharT, L> &__lhs,
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept;

template <class _CharT, size_t L, size_t R>
	constexpr bool operator>=(const _CharT(&__lhs)[L],
                              const basic_fixed_string<_CharT, R> &__rhs) noexcept;

template <class _CharT, size_t L, size_t R>
	constexpr bool operator>=(const basic_fixed_string<_CharT, L> &__lhs,
                              const _CharT(&__rhs)[R]) noexcept;

//	swap
template <class _CharT, size_t _Sz>
	constexpr void swap(basic_fixed_string<_CharT, _Sz>& __lhs, 
	                    basic_fixed_string<_CharT, _Sz>& __rhs) noexcept;


//	basic_fixed_string type aliases template <size_t _Sz>
template <size_t _Sz>
	using fixed_string = basic_fixed_string<char, _Sz>; 

template <size_t _Sz>
	using fixed_u16string = basic_fixed_string<char16_t, _Sz>;

template <size_t _Sz>
	using fixed_u32string = basic_fixed_string<char32_t, _Sz>;

template <size_t _Sz>
	using fixed_wstring = basic_fixed_string<wchar_t, _Sz>;


//	numeric conversions
template <size_t _Sz>
	constexpr int stoi(const fixed_string<_Sz>& str, int base = 10);
	
template <size_t _Sz>
	constexpr unsigned stou(const fixed_string<_Sz>& str, int base = 10);
	
template <size_t _Sz>
	constexpr long stol(const fixed_string<_Sz>& str, int base = 10);
	
template <size_t _Sz>
	constexpr unsigned long stoul(const fixed_string<_Sz>& str, int base = 10);
	
template <size_t _Sz>
	constexpr long long stoll(const fixed_string<_Sz>& str, int base = 10);
	
template <size_t _Sz>
	constexpr unsigned long long stoull(const fixed_string<_Sz>& str, int base = 10);

template <size_t _Sz>
	constexpr float stof(const fixed_string<_Sz>& str);

template <size_t _Sz>
	constexpr double stod(const fixed_string<_Sz>& str);
	
template <size_t _Sz>
	constexpr long double stold(const fixed_string<_Sz>& str);

// template <int val>
// 	constexpr fixed_string</*...*/> to_fixed_string_i() noexcept;
// 
// template <unsigned val>
// 	constexpr fixed_string</*...*/> to_fixed_string_u() noexcept;
// 	
// template <long val>
// 	constexpr fixed_string</*...*/> to_fixed_string_l() noexcept;
// 	
// template <unsigned long val>
// 	constexpr fixed_string</*...*/> to_fixed_string_ul() noexcept;
// 	
// template <long long val>
// 	constexpr fixed_string</*...*/> to_fixed_string_ll() noexcept;
// 	
// template <unsigned long long val>
// 	constexpr fixed_string</*...*/> to_fixed_string_ull() noexcept;

//	XY._Sz+6, creation helper function template <class _CharT, size_t _Sz>
template <class _CharT, size_t _Sz>
	constexpr basic_fixed_string<_CharT, _Sz - 1>
		make_fixed_string(const _CharT(&a)[_Sz]) noexcept;
	

template <class _CharT, size_t _Sz>
class basic_fixed_string {
public:
    using value_type             = _CharT;
    using view                   = _STD::basic_string_view<_CharT>; // should be 'view_type'
    using size_type              = typename view::size_type;
    using difference_type        = typename view::difference_type;
    using reference              =       value_type &;
    using const_reference        = const value_type &;
    using pointer                =       value_type *;
    using const_pointer          = const value_type *;
    using iterator               = pointer;       // should be "implementation-defined"
    using const_iterator         = const_pointer; // should be "implementation-defined"
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos = view::npos;

//  construct/copy/conversions
    constexpr basic_fixed_string() noexcept;
    constexpr basic_fixed_string(const basic_fixed_string& __str) noexcept;
    constexpr basic_fixed_string(const _CharT(&__arr)[_Sz + 1]) noexcept;

    constexpr basic_fixed_string& operator=(const basic_fixed_string& __str) noexcept;
    constexpr basic_fixed_string& operator=(const _CharT(&__arr)[_Sz + 1]) noexcept;
    constexpr operator view() const noexcept;

//  iterators
    constexpr iterator               begin()         noexcept;
    constexpr const_iterator         begin()   const noexcept;
    constexpr iterator               end()           noexcept;
    constexpr const_iterator         end()     const noexcept;
    constexpr reverse_iterator       rbegin()        noexcept;
    constexpr const_reverse_iterator rbegin()  const noexcept;
    constexpr reverse_iterator       rend()          noexcept;
    constexpr const_reverse_iterator rend()    const noexcept;
    constexpr const_iterator         cbegin()  const noexcept;
    constexpr const_iterator         cend()    const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend()   const noexcept;

//  capacity
    constexpr size_type size()     const noexcept;
    constexpr size_type length()   const noexcept;
    constexpr size_type max_size() const noexcept { return _Sz; }
    constexpr size_type capacity() const noexcept { return _Sz; }
    constexpr bool      empty()    const noexcept;

//  element access
    constexpr const_reference operator[](size_type __pos) const noexcept;
    constexpr       reference operator[](size_type __pos)       noexcept;
    constexpr const_reference at        (size_type __pos) const noexcept;
    constexpr       reference at        (size_type __pos)       noexcept;
    
    constexpr const_reference front() const noexcept;
    constexpr       reference front()       noexcept;
    constexpr const_reference back()  const noexcept;
    constexpr       reference back()        noexcept;

//  modifications
    constexpr basic_fixed_string& replace(size_t pos, view str);
    constexpr void swap(basic_fixed_string& str);

//  string operations
    constexpr const _CharT* c_str() const noexcept;
    constexpr const _CharT* data() const noexcept;

    constexpr size_type find(view str, size_type __pos = 0) const noexcept;
    constexpr size_type find(const _CharT *__s, size_type __pos, size_type __n) const;
    constexpr size_type find(const _CharT *__s, size_type __pos = 0) const;
    constexpr size_type find(_CharT c, size_type __pos = 0) const noexcept;

    constexpr size_type rfind(view str, size_type __pos = npos) const noexcept;
    constexpr size_type rfind(const _CharT *__s, size_type __pos, size_type __n) const;
    constexpr size_type rfind(const _CharT *__s, size_type __pos = npos) const;
    constexpr size_type rfind(_CharT c, size_type __pos = npos) const;

    constexpr size_type find_first_of(view str, size_type __pos = 0) const noexcept;
    constexpr size_type find_first_of(const _CharT *__s, size_type __pos, size_type __n) const;
    constexpr size_type find_first_of(const _CharT *__s, size_type __pos = 0) const;
    constexpr size_type find_first_of(_CharT c, size_type __pos = 0) const;

    constexpr size_type find_last_of(view str, size_type __pos = npos) const noexcept;
    constexpr size_type find_last_of(const _CharT *__s, size_type __pos, size_type __n) const;
    constexpr size_type find_last_of(const _CharT *__s, size_type __pos = npos) const;
    constexpr size_type find_last_of(_CharT c, size_type __pos = npos) const;

    constexpr size_type find_first_not_of(view str, size_type __pos = 0) const noexcept;
    constexpr size_type find_first_not_of(const _CharT *__s, size_type __pos, size_type __n) const;
    constexpr size_type find_first_not_of(const _CharT *__s, size_type __pos = 0) const;
    constexpr size_type find_first_not_of(_CharT c, size_type __pos = 0) const;

    constexpr size_type find_last_not_of(view str, size_type __pos = npos) const noexcept;
    constexpr size_type find_last_not_of(const _CharT *__s, size_type __pos, size_type __n) const;
    constexpr size_type find_last_not_of(const _CharT *__s, size_type __pos = npos) const;
    constexpr size_type find_last_not_of(_CharT c, size_type __pos = npos) const;

//  template <size_type __pos = 0, size_type count = npos>
//  constexpr basic_fixed_string<_CharT, /*...*/> substr() const noexcept;
    
    constexpr int compare(view str) const noexcept;
    constexpr int compare(size_type __pos1, size_type __n1, view str) const;
    constexpr int compare(size_type __pos1, size_type __n1, view str, size_type __pos2, size_type __n2 = npos) const;
    constexpr int compare(const _CharT* s) const;
    constexpr int compare(size_type __pos1, size_type __n1, const _CharT* s) const;
    constexpr int compare(size_type __pos1, size_type __n1, const _CharT *__s, size_type __n2) const;

private:
//  _CharT data_[_Sz+1]; //exposition only
};

} //namespace somestd

int main() 
{
}
