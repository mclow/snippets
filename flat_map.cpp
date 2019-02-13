// based on https://wg21.link/P0429

#include <initializer_list>
#include <functional>
#include <algorithm>
#include <iterator>

/*

// 26.6.8, class template flatmap
template<class Key, class T, class Compare = less<Key>,
         class KeyContainer = vector<Key>, class MappedContainer = vector<T>>
class flat_map;

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator==(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator!=(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator< (const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator> (const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator<=(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare,class KeyContainer, class MappedContainer>
bool operator>=(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
void swap(flat_map<Key, T, Compare, KeyContainer, MappedContainer>& x,
          flat_map<Key, T, Compare, KeyContainer, MappedContainer>& y)
     noexcept(noexcept(x.swap(y)));

struct sorted_unique_t { explicit sorted_unique_t() = default; };
inline constexpr sorted_unique_t sorted_unique {};

// 26.6.9, class template flat_multimap
template<class Key, class T, class Compare = less<Key>,
         class KeyContainer = vector<Key>, class MappedContainer = vector<T>>
class flat_multimap;

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator==(const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator!=(const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator< (const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator> (const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
bool operator<=(const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare,class KeyContainer, class MappedContainer>
bool operator>=(const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
                const flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y);

template<class Key, class T, class Compare, class KeyContainer, class MappedContainer>
void swap(flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& x,
          flat_multimap<Key, T, Compare, KeyContainer, MappedContainer>& y)
     noexcept(noexcept(x.swap(y)));

struct sorted_equivalent_t { explicit sorted_equivalent_t() = default; };
inline constexpr sorted_equivalent_t sorted_equivalent {};

*/

namespace some_std {

namespace _STD = _VSTD;

template <class _Key, class _Tp, class _Compare = _STD::less<_Key>,
          class _KeyContainer    = _STD::vector<_Key>, 
          class _MappedContainer = _STD::vector<_Tp> >
class _LIBCPP_TEMPLATE_VIS flat_map;

template <class _Key, class _Tp, class _Compare = _STD::less<_Key>,
          class _KeyContainer    = _STD::vector<_Key>, 
          class _MappedContainer = _STD::vector<_Tp> >
class _LIBCPP_TEMPLATE_VIS flat_multimap;


struct sorted_equivalent_t { explicit sorted_equivalent_t() = default; };
inline constexpr sorted_equivalent_t sorted_equivalent {};

struct sorted_unique_t { explicit sorted_unique_t() = default; };
inline constexpr sorted_unique_t sorted_unique {};

template <class _KIterator, class _VIterator>
class _LIBCPP_TEMPLATE_VIS __flat_map_iterator {
    
public:
    using _KeyType              = typename _STD::iterator_traits<_KIterator>::value_type;
    using _ValueType            = typename _STD::iterator_traits<_VIterator>::value_type;
    using difference_type       = typename _KIterator::difference_type;
    using reference             = _STD::pair<const _KeyType&, _ValueType&>;
    using const_reference       = _STD::pair<const _KeyType&, const _ValueType&>;
//      using pointer               = reference *;
    using iterator_category     = _STD::random_access_iterator_tag;

    __flat_map_iterator() = default;
    __flat_map_iterator(const __flat_map_iterator &) = default;
    __flat_map_iterator &operator=(const __flat_map_iterator &) = default;
    ~__flat_map_iterator() = default;

    __flat_map_iterator(_KIterator __k, _VIterator __v)
        : __keyIter(__k), __valueIterator(__v) {}
    
    reference operator*() const { return {*__keyIter, *__valueIterator}; }
//  pointer   operator->() const;
    reference operator[](difference_type __n) const { return {__keyIter[__n], __valueIterator[__n]}; }

    __flat_map_iterator& operator++()                          { ++__keyIter; ++__valueIterator; return *this; }
    __flat_map_iterator& operator--()                          { --__keyIter; --__valueIterator; return *this; }
    __flat_map_iterator  operator++(int)                       { __flat_map_iterator __tmp(*this); ++(*this); return __tmp; }
    __flat_map_iterator  operator--(int)                       { __flat_map_iterator __tmp(*this); --(*this); return __tmp; }
    __flat_map_iterator  operator+ (difference_type __n) const { __flat_map_iterator __tmp(*this); __tmp += __n; return __tmp; }
    __flat_map_iterator  operator- (difference_type __n) const { __flat_map_iterator __tmp(*this); __tmp -= __n; return __tmp; }
    __flat_map_iterator& operator+=(difference_type __n)       { __keyIter += __n; __valueIterator += __n; return *this; }
    __flat_map_iterator  operator-=(difference_type __n)       { __keyIter -= __n; __valueIterator -= __n; return *this; }

    bool __equal(const __flat_map_iterator &rhs) const
    { return __keyIter == rhs.__keyIter && __valueIterator == rhs.__valueIterator; }

private:
    _KIterator __keyIter;
    _VIterator __valueIterator;
};

template<class _KIterator, class _VIterator>
bool operator==(const __flat_map_iterator<_KIterator, _VIterator>& x,
                const __flat_map_iterator<_KIterator, _VIterator>& y)
{ return x.__equal(y); }

template<class _KIterator, class _VIterator>
bool operator!=(const __flat_map_iterator<_KIterator, _VIterator>& x,
                const __flat_map_iterator<_KIterator, _VIterator>& y)
{ return !(x == y); }


template <class _Key, class _Tp, class _Compare,
          class _KeyContainer, class _MappedContainer>
class _LIBCPP_TEMPLATE_VIS flat_map {
public:
    // types:
    using key_type               = _Key;
    using mapped_type            = _Tp;
    using value_type             = _STD::pair<const _Key, _Tp>;
    using key_compare            = _Compare;
    using key_container_type     = _KeyContainer;
    using mapped_container_type  = _MappedContainer;
    using key_allocator_type     = typename key_container_type::allocator_type;
    using mapped_allocator_type  = typename mapped_container_type::allocator_type;
    using reference              = _STD::pair<const _Key&, _Tp&>;
    using const_reference        = _STD::pair<const _Key&, const _Tp&>;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;

    using iterator               = __flat_map_iterator<typename _KeyContainer::iterator,       typename _MappedContainer::iterator>;
    using const_iterator         = __flat_map_iterator<typename _KeyContainer::const_iterator, typename _MappedContainer::const_iterator>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    struct containers {
        _KeyContainer    keys;
        _MappedContainer values;
    };
    
    // 26.6.8.2, construct/copy/destroy
    flat_map() {}
    flat_map(                 key_container_type&& __k, mapped_container_type&& __v);
    flat_map(sorted_unique_t, key_container_type&& __k, mapped_container_type&& __v);
//	extra!!
	flat_map(           const key_container_type& __k, const mapped_container_type& __v)
		: __c{__k, __v}, __compare{} {}


    template <class _Container>
    explicit flat_map(const _Container& __cont)
        : flat_map(__cont.begin(), __cont.end(), key_compare()) {}

    template <class _Container>
    flat_map(sorted_unique_t __s, const _Container& __cont)
        : flat_map(__s, __cont.begin(), __cont.end(), key_compare()) {}

//  template <class _Container, class Alloc>
//  flat_map(const _Container& __cont, const Alloc& a)
//      : flat_map(__cont.begin(), __cont.end(), key_compare(), a) {}
//  
//  template <class _Container, class Alloc>
//  flat_map(sorted_unique_t __s, const _Container& __cont, const Alloc& a)
//      : flat_map(__s, __cont.begin(), __cont.end(), Compare(), a) {}

    explicit flat_map(const key_compare& comp);
    
//  template <class Alloc>
//  flat_map(const key_compare& comp, const Alloc& a);
// 
//  template <class Alloc>
//  explicit flat_map(const Alloc& a)
//      : flat_map(key_compare(), a) {}

    template <class _InputIterator>
    flat_map(sorted_unique_t, _InputIterator __first, _InputIterator __last, 
                              const key_compare& comp = key_compare());

//  template <class _InputIterator, class Alloc>
//  flat_map(sorted_unique_t, _InputIterator __first, _InputIterator __last,
//                            const key_compare& comp, const Alloc& a);
//  
//  template <class _InputIterator, class Alloc>
//  flat_map(sorted_unique_t, _InputIterator __first, _InputIterator __last,
//                              const Alloc& a)
//      : flat_map(s, first, last, Compare(), a) { }
//  
//  template <class Alloc>
//  flat_map(const flat_map& m, const Alloc& a)
//      : compare{std::move(m.compare)}, c{{_VSTD::move(m.c.keys), a}, {_VSTD::move(m.c.values), a}} {}
// 
//  template<class Alloc>
//  flat_map(const flat_map& m, const Alloc& a)
//      : compare{m.compare}, c{{m.c.keys, a}, {m.c.values, a}} {}

    flat_map(std::initializer_list<_STD::pair<_Key, _Tp>> __il, const key_compare& comp = key_compare())
        : flat_map(__il.begin(), __il.end(), comp) {}

    flat_map(sorted_unique_t __s, std::initializer_list<_STD::pair<_Key, _Tp>> __il, const key_compare& __comp = key_compare())
        : flat_map(__s, __il.begin(), __il.end(), __comp) {}

//  template <class Alloc>
//  flat_map(initializer_list<_STD::pair<Key, T>> __il, const Compare& comp, const Alloc& a)
//      : flat_map(il, comp, a) {}
// 
//  template <class Alloc>
//  flat_map(initializer_list<_STD::pair<Key, T>> __il, const Alloc& a)
//      : flat_map(il, Compare(), a) {}
//
//  template <class Alloc>
//  flat_map(sorted_unique_t s, initializer_list<_STD::pair<Key, T>> __il, const Compare& comp, const Alloc& a)
//      : flat_map(s, il, comp, a) {}
//      
//  template <class Alloc>
//  flat_map(sorted_unique_t s, initializer_list<_STD::pair<Key, T>> __il, const Alloc& a)
//      : flat_map(s, il, Compare(), a) {}

    flat_map& operator=(std::initializer_list<_STD::pair<_Key, _Tp>> __il);
//  flat_map& operator=(const flat_map &); // Do we need this?

    // iterators
    iterator               begin()         noexcept { return {__c.keys.begin(),   __c.values.begin()}; }
    const_iterator         begin()   const noexcept { return {__c.keys.begin(),   __c.values.begin()}; }
    iterator               end()           noexcept { return {__c.keys.end(),     __c.values.end()}; }
    const_iterator         end()     const noexcept { return {__c.keys.end(),     __c.values.end()}; }

    reverse_iterator       rbegin()        noexcept { return       reverse_iterator({__c.keys.end(),   __c.values.end()}); }
    const_reverse_iterator rbegin()  const noexcept { return const_reverse_iterator({__c.keys.end(),   __c.values.end()}); }
    reverse_iterator       rend()          noexcept { return       reverse_iterator({__c.keys.begin(), __c.values.begin()}); }
    const_reverse_iterator rend()    const noexcept { return const_reverse_iterator({__c.keys.begin(), __c.values.begin()}); }

    const_iterator         cbegin()  const noexcept { return begin(); }
    const_iterator         cend()    const noexcept { return end(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend()   const noexcept { return rend(); }

    // capacity
    [[nodiscard]] bool empty() const noexcept { return size() == 0; }
    size_type           size() const noexcept { return __c.keys.size(); }
    size_type       max_size() const noexcept { return _STD::max(__c.keys.max_size(), __c.values.max_size()); }

    // 26.6.8.4, element access
    mapped_type& operator[](const key_type&  __k) { return (*find(__k)).second; } // TODO FIX
    mapped_type& operator[](      key_type&& __k);
    mapped_type&         at(const key_type& __k)       { return (*find(__k)).second; } // TODO THROW
    const mapped_type&   at(const key_type& __k) const { return (*find(__k)).second; } // TODO THROW

    // 26.6.8.5, modifiers
    template <class... Args>
    _STD::pair<iterator, bool> emplace(Args&&... args);

    template <class... Args>
    iterator emplace_hint(const_iterator position, Args&&... args);

    _STD::pair<iterator, bool> insert(const value_type&  x);
    _STD::pair<iterator, bool> insert(      value_type&& x);

    template <class P>
    _STD::pair<iterator, bool> insert(P&& x);

    iterator insert(const_iterator position, const value_type&  x);
    iterator insert(const_iterator position,       value_type&& x);

    template <class P>
    iterator insert(const_iterator position, P&&);

    template <class _InputIterator>
    void insert(                 _InputIterator __first, _InputIterator __last);
    template <class _InputIterator>
    void insert(sorted_unique_t, _InputIterator __first, _InputIterator __last);

    void insert(                 std::initializer_list<_STD::pair<_Key, _Tp>> __il);
    void insert(sorted_unique_t, std::initializer_list<_STD::pair<_Key, _Tp>> __il);

    containers extract() &&;

    void replace(key_container_type&& __k, mapped_container_type&& __v);

    template <class... _Args>
    _STD::pair<iterator, bool> try_emplace(const key_type&  __k, _Args&&... __args);
    template <class... _Args>
    _STD::pair<iterator, bool> try_emplace(      key_type&& __k, _Args&&... __args);

    template <class... _Args>
    iterator try_emplace(const_iterator hint, const key_type&  __k, _Args&&... __args);
    template <class... _Args>
    iterator try_emplace(const_iterator hint,       key_type&& __k, _Args&&... __args);

    template <class _Up>
    _STD::pair<iterator, bool> insert_or_assign(const key_type&  __k, _Up&& __v);
    template <class _Up>
    _STD::pair<iterator, bool> insert_or_assign(      key_type&& __k, _Up&& __v);
    template <class _Up>
    iterator insert_or_assign(const_iterator hint, const key_type&  __k, _Up&& __v);
    template <class _Up>
    iterator insert_or_assign(const_iterator hint,       key_type&& __k, _Up&& __v);

    iterator  erase(iterator position);
    iterator  erase(const_iterator position);
    size_type erase(const key_type& x);
    iterator  erase(const_iterator first, const_iterator last);

    void swap(flat_map& fm)
        noexcept(_STD::is_nothrow_swappable_v<key_container_type> 
              && _STD::is_nothrow_swappable_v<mapped_container_type>
              && _STD::is_nothrow_swappable_v<key_compare>)
    {
        using std::swap;
        swap(__c.keys,   fm.__c.keys);
        swap(__c.values, fm.__c.values);
        swap(__compare,  __compare);
    }

    void clear() noexcept { __c.keys.clear(); __c.values.clear(); };

//     template<class C2>
//     void merge(flat_multimap<_Key, _Tp, C2, _KeyContainer, _MappedContainer>& source);
// 
//     template<class C2>
//     void merge(flat_multimap<_Key, _Tp, C2, _KeyContainer, _MappedContainer>&& source);
// 
//     template<class C2>
//     void merge(flat_map<_Key, _Tp, C2, _KeyContainer, _MappedContainer>& source);
// 
//     template<class C2>
//     void merge(flat_map<_Key, _Tp, C2, _KeyContainer, _MappedContainer>&& source);


// observers
    key_compare key_comp() const;
//    value_compare value_comp() const;
    const _KeyContainer & keys()      const { return __c.keys; }
    const _MappedContainer & values() const { return __c.values; }

//	map operations
    bool contains(const key_type& __v)             const { return find(__v) != cend(); }
    template <class K> bool contains(const K& __v) const { return find(__v) != cend(); }

    iterator __make_iterator(typename key_container_type::iterator __it)
    {
        if (__it == __c.keys.end())
            return { __it, __c.values.end() };
        else
            return { __it, __c.values.begin() + _STD::distance(__c.keys.begin(), __it) };
    }

    const_iterator __make_iterator(typename key_container_type::const_iterator __it) const
    {
        if (__it == __c.keys.end())
            return { __it, __c.values.end() };
        else
            return { __it, __c.values.begin() + _STD::distance(__c.keys.begin(), __it) };
    }

    iterator find(const key_type& __v)
    {
    	typename key_container_type::iterator __it = _STD::lower_bound(__c.keys.begin(), __c.keys.end(), __v, __compare);
    	return __make_iterator(__it != __c.keys.end() && !__compare(__v, *__it) ? __it : __c.keys.end());
    }

    const_iterator find(const key_type& __v) const
    {
    	typename key_container_type::const_iterator __it = _STD::lower_bound(__c.keys.begin(), __c.keys.end(), __v, __compare);
    	return __make_iterator(__it != __c.keys.end() && !__compare(__v, *__it) ? __it : __c.keys.end());
    }

    template <class _K2>
    typename _STD::enable_if<_STD::__is_transparent<_Compare, _K2>::value,iterator>::type
    find(const _K2& __v)
    {
    	typename key_container_type::iterator __it = _STD::lower_bound(__c.keys.begin(), __c.keys.end(), __v, __compare);
    	return __make_iterator(__it != __c.keys.end() && !__compare(__v, *__it) ? __it : __c.keys.end());
    }
    
    template <class _K2>
    typename _STD::enable_if<_STD::__is_transparent<_Compare, _K2>::value,const_iterator>::type
    find(const _K2& __v) const
    {
    	typename key_container_type::const_iterator __it = _STD::lower_bound(__c.keys.begin(), __c.keys.end(), __v, __compare);
    	return __make_iterator(__it != __c.keys.end() && !__compare(__v, *__it) ? __it : __c.keys.end());
    }

    size_type                    count(const key_type& __v) const { return contains(__v) ? 1 : 0; }
    template <class K> size_type count(const K& __v)        const { return contains(__v) ? 1 : 0; }

    iterator lower_bound(const key_type& __v)
    { return __make_iterator(_STD::lower_bound(__c.keys.begin(), __c.keys.end(), __v, __compare)); }
    
    const_iterator lower_bound(const key_type& __v) const
    { return __make_iterator(_STD::lower_bound(__c.keys.begin(), __c.keys.end(), __v, __compare)); }
    
    template <class _K2>
    typename _STD::enable_if<_STD::__is_transparent<_Compare, _K2>::value, iterator>::type
    lower_bound(const _K2& __v)
    { return __make_iterator(_STD::lower_bound(__c.keys.begin(), __c.keys.end(), __v, __compare)); }
    
    template <class _K2>
    typename _STD::enable_if<_STD::__is_transparent<_Compare, _K2>::value, const_iterator>::type
    lower_bound(const _K2& __v) const
    { return __make_iterator(_STD::lower_bound(__c.keys.begin(), __c.keys.end(), __v, __compare)); }

    iterator upper_bound(const key_type& __v)
    { return __make_iterator(_STD::upper_bound(__c.keys.begin(), __c.keys.end(), __v, __compare)); }
    
    const_iterator upper_bound(const key_type& __v) const
    { return __make_iterator(_STD::upper_bound(__c.keys.begin(), __c.keys.end(), __v, __compare)); }

    template <class _K2>
    typename _STD::enable_if<_STD::__is_transparent<_Compare, _K2>::value, iterator>::type
    upper_bound(const _K2& __v)
    { return __make_iterator(_STD::upper_bound(__c.keys.begin(), __c.keys.end(), __v, __compare)); }

    template <class _K2>
    typename _STD::enable_if<_STD::__is_transparent<_Compare, _K2>::value, const_iterator>::type
    upper_bound(const _K2& __v) const
    { return __make_iterator(_STD::upper_bound(__c.keys.begin(), __c.keys.end(), __v, __compare)); }

    _STD::pair<      iterator,       iterator> equal_range(const key_type& __v)
    { return _STD::make_pair(lower_bound(__v), upper_bound(__v)); }
    
    _STD::pair<const_iterator, const_iterator> equal_range(const key_type& __v) const
    { return _STD::make_pair(lower_bound(__v), upper_bound(__v)); }

    template <class _K2>
    _STD::pair<      iterator,       iterator> equal_range(const _K2& __v)
    { return _STD::make_pair(lower_bound(__v), upper_bound(__v)); }

    template <class _K2>
    _STD::pair<const_iterator, const_iterator> equal_range(const _K2& __v) const
    { return _STD::make_pair(lower_bound(__v), upper_bound(__v)); }

private:
    containers __c;
    key_compare __compare;
};

} // namespace some_std

#include <vector>
#include <deque>
#include <string>
#include <cassert>
#include <iostream>

int main () {
    {
    some_std::flat_map<int, std::string> fm;
    assert(fm.size() == 0);
    assert(fm.begin() == fm.end());
//  assert(std::distance(fm.begin(), fm.end()) == 0);
	for (const auto &p : fm) {}  // can we enumerate?
	}
	{
		std::vector<int> v = {1, 2, 3, 4, 6};
		std::deque<std::string> d = { "one", "two", "three", "four", "six" };
    	some_std::flat_map<int, std::string, std::less<int>, std::vector<int>, std::deque<std::string>> fm{v, d};
	    assert(fm.size() == 5);
		for (const auto &p : fm) { std::cout << "  " << p.first << ": " << p.second << std::endl; }  // can we enumerate?
    	
		std::cout << "fm[3] = " << (*fm.find(3)).second << std::endl;
		assert(fm.find(0) == fm.end());
		assert(fm.find(2) != fm.end());
		assert(fm.find(5) == fm.end());
		assert(fm.find(9) == fm.end());
		assert(fm.find(6) == fm.lower_bound(5));
		assert(fm.find(6) == fm.upper_bound(5));
		assert( fm.contains(3));
		assert(!fm.contains(5));
// 		assert( fm.contains(3.0));
// 		assert(!fm.contains(3.5));
	}
}