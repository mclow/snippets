// based on https://wg21.link/P0429

#include <initializer_list>

/*

// 26.6.8, class template flatmap
template<class Key, class T, class Compare = less<Key>,
         class KeyContainer = vector<Key>, class MappedContainer = vector<T>>
class flat_map;

template<class Key, class T, class Compare,class KeyContainer, class MappedContainer>
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


*/

namespace some_std {


struct sorted_equivalent_t { explicit sorted_equivalent_t() = default; };
inline constexpr sorted_equivalent_t sorted_equivalent {};

struct sorted_unique_t { explicit sorted_unique_t() = default; };
inline constexpr sorted_unique_t sorted_unique {};


template <class _Key, class _Tp, class _Compare = _VSTD::less<_Key>,
          class _KeyContainer    = _VSTD::vector<_Key>, 
          class _MappedContainer = _VSTD::vector<_Tp> >
class _LIBCPP_TEMPLATE_VIS flat_map {
public:
public:
	// types:
	using key_type              = _Key;
	using mapped_type           = _Tp;
	using value_type            = _VSTD::pair<const _Key, _Tp>;
	using key_compare           = _Compare;
	using key_container_type    = _KeyContainer;
	using mapped_container_type = _MappedContainer;
	using key_allocator_type    = typename key_container_type::allocator_type;
	using mapped_allocator_type = typename mapped_container_type::allocator_type;
	using reference             = _VSTD::pair<const _Key&, _Tp&>;
	using const_reference       = _VSTD::pair<const _Key&, const _Tp&>;
	using size_type             = size_t;
	using difference_type       = ptrdiff_t;

// 	using iterator = implementation-defined ; // see 26.2
// 	using const_iterator = implementation-defined ; // see 26.2
// 	using reverse_iterator = std::reverse_iterator<iterator>;
// 	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	struct containers {
		_KeyContainer    keys;
		_MappedContainer values;
	};
	
	// 26.6.8.2, construct/copy/destroy
	flat_map();
	flat_map(                 key_container_type&& __k, mapped_container_type&& __v);
	flat_map(sorted_unique_t, key_container_type&& __k, mapped_container_type&& __v);

	template <class _Container>
	explicit flat_map(const _Container& __cont)
		: flat_map(__cont.begin(), __cont.end(), key_compare()) {}

	template <class _Container>
	flat_map(sorted_unique_t __s, const _Container& __cont)
		: flat_map(__s, __cont.begin(), __cont.end(), key_compare()) {}

// 	template <class _Container, class Alloc>
// 	flat_map(const _Container& __cont, const Alloc& a)
// 		: flat_map(__cont.begin(), __cont.end(), key_compare(), a) {}
// 
// 	
// 	template <class _Container, class Alloc>
// 	flat_map(sorted_unique_t __s, const _Container& __cont, const Alloc& a)
// 		: flat_map(__s, __cont.begin(), __cont.end(), Compare(), a) {}

	explicit flat_map(const key_compare& comp);
	
// 	template <class Alloc>
// 	flat_map(const key_compare& comp, const Alloc& a);
// 
// 	template <class Alloc>
// 	explicit flat_map(const Alloc& a)
// 		: flat_map(key_compare(), a) {}

	template <class _InputIterator>
	flat_map(sorted_unique_t, _InputIterator __first, _InputIterator __last, 
	                          const key_compare& comp = key_compare());

// 	template <class _InputIterator, class Alloc>
// 	flat_map(sorted_unique_t, _InputIterator __first, _InputIterator __last,
// 	                          const key_compare& comp, const Alloc& a);
// 	
// 	template <class _InputIterator, class Alloc>
// 	flat_map(sorted_unique_t, _InputIterator __first, _InputIterator __last,
// 	                            const Alloc& a)
// 		: flat_map(s, first, last, Compare(), a) { }
// 	
// 	template <class Alloc>
// 	flat_map(const flat_map& m, const Alloc& a)
// 		: compare{std::move(m.compare)}, c{{_VSTD::move(m.c.keys), a}, {_VSTD::move(m.c.values), a}} {}
// 
// 	template<class Alloc>
// 	flat_map(const flat_map& m, const Alloc& a)
// 		: compare{m.compare}, c{{m.c.keys, a}, {m.c.values, a}} {}

	flat_map(initializer_list<_VSTD::pair<Key, T>> __il, const Compare& comp = Compare())
		: flat_map(il, comp) {}

// 	template <class Alloc>
// 	flat_map(initializer_list<_VSTD::pair<Key, T>> __il, const Compare& comp, const Alloc& a)
// 		: flat_map(il, comp, a) {}
// 
// 	template <class Alloc>
// 	flat_map(initializer_list<_VSTD::pair<Key, T>> __il, const Alloc& a)
// 		: flat_map(il, Compare(), a) {}

	flat_map(sorted_unique_t __s, initializer_list<_VSTD::pair<Key, T>> __il, const key_compare& __comp = key_compare())
		: flat_map(__s, __il, __comp) {}

// 	template <class Alloc>
// 	flat_map(sorted_unique_t, initializer_list<_VSTD::pair<Key, T>> __il, const Compare& comp, const Alloc& a)
// 		: flat_map(s, il, comp, a) {}
// 		
// 	template <class Alloc>
// 	flat_map(sorted_unique_t, initializer_list<_VSTD::pair<Key, T>> __il, const Alloc& a)
// 		: flat_map(s, il, Compare(), a) {}

	flat_map& operator=(initializer_list<_VSTD::pair<Key, T>> __il);

	// iterators
	iterator               begin()         noexcept;
	const_iterator         begin()   const noexcept;
	iterator               end()           noexcept;
	const_iterator         end()     const noexcept;
	reverse_iterator       rbegin()        noexcept;
	const_reverse_iterator rbegin()  const noexcept;
	reverse_iterator       rend()          noexcept;
	const_reverse_iterator rend()    const noexcept;
	const_iterator         cbegin()  const noexcept;
	const_iterator         cend()    const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend()   const noexcept;

	// capacity
	[[nodiscard]] bool empty() const noexcept { return size() == 0; }
	size_type  size()    const noexcept { return __c.keys.size(); }
	size_type max_size() const noexcept;

	// 26.6.8.4, element access
	mapped_type& operator[](const key_type&  x);
	mapped_type& operator[](      key_type&& x);
	mapped_type&       at(const key_type& x);
	const mapped_type& at(const key_type& x) const;

	// 26.6.8.5, modifiers
	template <class... Args>
	_VSTD::pair<iterator, bool> emplace(Args&&... args);
	template <class... Args>
	iterator emplace_hint(const_iterator position, Args&&... args);

	_VSTD::pair<iterator, bool> insert(const value_type&  x);
	_VSTD::pair<iterator, bool> insert(      value_type&& x);

	template <class P>
	_VSTD::pair<iterator, bool> insert(P&& x);

	iterator insert(const_iterator position, const value_type&  x);
	iterator insert(const_iterator position,       value_type&& x);

	template <class P>
	iterator insert(const_iterator position, P&&);

	template <class _InputIterator>
	void insert(                 _InputIterator __first, _InputIterator __last);
	template <class _InputIterator>
	void insert(sorted_unique_t, _InputIterator __first, _InputIterator __last);

	void insert(                 initializer_list<_VSTD::pair<_Key, _Tp>> __il);
	void insert(sorted_unique_t, initializer_list<_VSTD::pair<_Key, _Tp>> __il);

	containers extract() &&;

	void replace(key_container_type&& __k, mapped_container_type&& __v);

	template <class... _Args>
	_VSTD::pair<iterator, bool> try_emplace(const key_type&  __k, _Args&&... __args);
	template <class... _Args>
	_VSTD::pair<iterator, bool> try_emplace(      key_type&& __k, _Args&&... __args);

	template <class... _Args>
	iterator try_emplace(const_iterator hint, const key_type&  __k, _Args&&... __args);
	template <class... _Args>
	iterator try_emplace(const_iterator hint,       key_type&& __k, _Args&&... __args);

	template <class _Up>
	_VSTD::pair<iterator, bool> insert_or_assign(const key_type&  __k, _Up&& __v);
	template <class _Up>
	_VSTD::pair<iterator, bool> insert_or_assign(      key_type&& __k, _Up&& __v);
	template <class _Up>
	iterator insert_or_assign(const_iterator hint, const key_type&  __k, _Up&& __v);
	template <class _Up>
	iterator insert_or_assign(const_iterator hint,       key_type&& __k, _Up&& __v);

	iterator erase(iterator position);
	iterator erase(const_iterator position);
	size_type erase(const key_type& x);
	iterator erase(const_iterator first, const_iterator last);

	void swap(flat_map& fm)
	noexcept(
		noexcept(declval<key_container_type>().swap(declval<key_container_type&>())) &&
		noexcept(declval<mapped_container_type>().swap(declval<mapped_container_type&>()))
	);

	void clear() noexcept { __c.keys.clear(); __c.values.clear(); };

	template<class C2>
	void merge(flat_map<Key, T, C2, KeyContainer, MappedContainer>& source);
	template<class C2>
	void merge(flat_map<Key, T, C2, KeyContainer, MappedContainer>&& source);
	template<class C2>
	void merge(flat_map<Key, T, C2, KeyContainer, MappedContainer>& source);
	template<class C2>
	void merge(flat_map<Key, T, C2, KeyContainer, MappedContainer>&& source);

	// observers
	key_compare key_comp() const;
	value_compare value_comp() const;

	// map operations
	bool contains(const key_type& __v)             const { return find(__v) != cend(); }
	template <class K> bool contains(const K& __v) const { return find(__v) != cend(); }

	      iterator find(const key_type& __v);
	const_iterator find(const key_type& __v) const;
	template <class K>       iterator find(const K& __v);
	template <class K> const_iterator find(const K& __v) const;

	size_type                    count(const key_type& __v) const { return contains(__v) ? 1 : 0; }
	template <class K> size_type count(const K& __v)        const { return contains(__v) ? 1 : 0; }

	      iterator lower_bound(const key_type& __v);
	const_iterator lower_bound(const key_type& __v) const;
	template <class K>       iterator lower_bound(const K& __v);
	template <class K> const_iterator lower_bound(const K& __v) const;

	      iterator upper_bound(const key_type& __v);
	const_iterator upper_bound(const key_type& __v) const;
	template <class K>       iterator upper_bound(const K& __v);
	template <class K> const_iterator upper_bound(const K& __v) const;

	_VSTD::pair<      iterator,       iterator> equal_range(const key_type& __v)
	{ return _VSTD::make_pair<lower_bound(__v), upper_bound(__v)); }
	
	_VSTD::pair<const_iterator, const_iterator> equal_range(const key_type& __v) const
	{ return _VSTD::make_pair<lower_bound(__v), upper_bound(__v)); }

	template <class K> _VSTD::pair<      iterator,       iterator> equal_range(const K& __v)
	{ return _VSTD::make_pair<lower_bound(__v), upper_bound(__v)); }

	template <class K> _VSTD::pair<const_iterator, const_iterator> equal_range(const K& __v) const
	{ return _VSTD::make_pair<lower_bound(__v), upper_bound(__v)); }

private:
	containers __c;
	key_compare __compare;
};

} // namespace some_std

#include <vector>

int main () {
}