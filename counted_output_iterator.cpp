//	An iterator that writes a fixed number of things, then stops.

#include <iterator>

template <typename Iterator>
class counted_output_iterator {
private:
	Iterator base_;
	size_t space_;
public:
//	typedefs
	typedef void                     value_type;
	typedef void                     difference_type;
	typedef void                     pointer;
	typedef void                     reference;
	typedef std::output_iterator_tag iterator_category;

	counted_output_iterator(Iterator b, size_t sz) : base_(b), space_(sz) {}
	
	counted_output_iterator& operator++()    { return *this; }
	counted_output_iterator& operator++(int) { return *this; }
	counted_output_iterator& operator*()     { return *this; }
	
	template <typename T>
	counted_output_iterator& operator=(const T &value)
	{
		if (space_ > 0)
		{
			--space_;
			*base_++ = value;
		}
		return *this;
	}
};

#include <algorithm>
#include <cassert>

int main () {
	char dest[11] = {0};
	assert(dest[10] == 0);
	std::copy_n("abcdefghijklmnop", 12, counted_output_iterator<char *>(dest, 10));
	for (int i = 0; i < 10; ++i)
		assert(dest[i] == 'a' + i);
	assert(dest[10] == 0);
}
