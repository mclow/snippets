// #include <experimental/functional>  // for default_searcher
#include <experimental/string_view>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>

// Find a single element in the sequence
template <typename CharT>
struct single_searcher
{
	single_searcher (CharT c) : __c(c) {}
	
    template <typename _ForwardIterator>
    _LIBCPP_INLINE_VISIBILITY
    std::pair<_ForwardIterator, _ForwardIterator>
    operator () (_ForwardIterator __f, _ForwardIterator __l) const
    {
    	_ForwardIterator __found = std::find(__f, __l, __c);
    	if ( __found == __l) return std::make_pair(__l, __l);
    	_ForwardIterator __temp = __found;
    	return std::make_pair(__found, ++__temp);
    	}

    CharT __c;
};

template <typename _ForwardIterator>
struct any_of_searcher
{
	any_of_searcher (_ForwardIterator pFirst, _ForwardIterator pLast) : __pFirst(pFirst), __pLast(pLast) {}
	
    template <typename _ForwardIterator2>
    _LIBCPP_INLINE_VISIBILITY
    std::pair<_ForwardIterator2, _ForwardIterator2>
    operator () (_ForwardIterator2 __f, _ForwardIterator2 __l) const
    {
    	_ForwardIterator __found = std::find_first_of(__f, __l, __pFirst, __pLast);
    	if ( __found == __l) return std::make_pair(__l, __l);
    	_ForwardIterator __temp = __found;
    	return std::make_pair(__found, ++__temp);
    	}

    _ForwardIterator __pFirst;
    _ForwardIterator __pLast;
};

	
template <typename InIterator, typename Searcher, typename OutIter>
OutIter split(InIterator first, InIterator last, const Searcher &s, OutIter out)
{
	while (first != last)
	{
		std::pair<InIterator, InIterator> found = s(first, last);
		// We've got three cases here:
		//  The pattern is found in the middle of the input; output a chunk, and go around again
		//  The pattern doesn't exist: output the rest of the input and terminate.
		//	The pattern is found at the end of the input, output an empty chunk and terminate.
		*out++ = std::make_pair(first, found.first);
		if (found.second == last && found.first != found.second)
			*out++ = std::make_pair(last, last);
		first = found.second;
	}
	return out;
}

template <typename Searcher>
void test ( const Searcher &s, const char *source, int numExpected, const char **expected )
{
	std::vector<std::pair<const char *, const char *>> chunks;
	
	(void) split(source, source + strlen(source), s, std::back_inserter(chunks));
	assert(chunks.size() == numExpected);
	for ( int i = 0; i < numExpected; ++i )
	{
		std::experimental::string_view sv{chunks[i].first, static_cast<unsigned long>(chunks[i].second - chunks[i].first)};
		assert(sv == expected[i]);
	}
}

const char * source0 = "";
const int    expected0 = 0;
const char * results0 [] = {};

const char * source1 = "ABC!DEF";
const int    expected1 = 1;
const char * results1 [] = { "ABC!DEF" };

const char * source2 = "ABC*DEF**GHI*JLK";
const int    expected2 = 5;
const char * results2 [] = { "ABC", "DEF", "", "GHI", "JLK" };

const char * source3 = "*ABC*DEF**GHI*JLK*";
const int    expected3 = 7;
const char * results3 [] = { "", "ABC", "DEF", "", "GHI", "JLK", "" };

const char * source4 = "*ABC*DEF**GHI*JLK**";
const int    expected4 = 8;
const char * results4 [] = { "", "ABC", "DEF", "", "GHI", "JLK", "", "" };

const char * source5 = "*ABC*DEF**GHI*JLK*!";
const int    expected5 = 7;
const char * results5 [] = { "", "ABC", "DEF", "", "GHI", "JLK", "!" };


int main () {
	const char * source = "*ABC*DEF**GHI*JLK*";
	const char * pattern = "*";
//	auto s = std::experimental::make_default_searcher(pattern, pattern + 1);
//	single_searcher<char> s{'*'};
	any_of_searcher<const char *> s{pattern, pattern + strlen(pattern)};
	std::vector<std::pair<const char *, const char *>> chunks;
	
	(void) split(source, source + strlen(source), s, std::back_inserter(chunks));
	std::cout << "Found " << chunks.size() << " chunks" << std::endl;
	for (const auto & p : chunks) {
		std::experimental::string_view sv{p.first, static_cast<unsigned long>(p.second - p.first)};
		std::cout << "  " << sv << std::endl;
		}

	test(s, source0, expected0, results0 );
	test(s, source1, expected1, results1 );
	test(s, source2, expected2, results2 );
	test(s, source3, expected3, results3 );
	test(s, source4, expected4, results4 );
	test(s, source5, expected5, results5 );
	}
