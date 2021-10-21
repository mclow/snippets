#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <cassert>

int comparisons;
int swaps;
std::mt19937 gen32;

struct Logger {
	Logger(int v) : value(v) {}
	int value;
	
};

bool operator <(const Logger &rhs, const Logger &lhs)
{
	++comparisons;
	return rhs.value < lhs.value;
}

bool operator>=(const Logger &rhs, const Logger &lhs)
{
	++comparisons;
	return rhs.value >= lhs.value;
}

bool operator<=(const Logger &rhs, const Logger &lhs)
{
	++comparisons;
	return rhs.value <= lhs.value;
}

void swap(Logger &lhs, Logger &rhs) noexcept {
	++swaps;
	std::swap(lhs.value, rhs.value);
}

typedef std::tuple<int,int, long long> Results;

Results partial_sort(const std::vector<Logger> &vals)
{
	std::vector<Logger> work(vals);
	comparisons = 0;
	swaps = 0;
	auto it = work.begin() + 1;
	std::partial_sort(work.begin(), it + 1, work.end());
	Results ret = {comparisons, swaps, 0};
	assert(std::is_sorted(work.begin(), it + 1));
	assert(std::all_of(work.begin (), it,  [it](const Logger &v) {return v <= *it;}));
	assert(std::all_of(it + 1, work.end(), [it](const Logger &v) {return v >= *it;}));
	return ret;
}

Results nth_element(const std::vector<Logger> &vals)
{
	std::vector<Logger> work(vals);
	comparisons = 0;
	swaps = 0;
	auto it = work.begin() + 1;
	std::nth_element(work.begin(), it, work.end());
	Results ret = {comparisons, swaps, 0};
	assert(std::is_sorted(work.begin(), it + 1));
	assert(std::all_of(work.begin (), it,  [it](const Logger &v) {return v <= *it;}));
	assert(std::all_of(it + 1, work.end(), [it](const Logger &v) {return v >= *it;}));
	return ret;
}

Results firstTwo(const std::vector<Logger> &vals)
{
	std::vector<Logger> work(vals);
	comparisons = 0;
	swaps = 0;

	bool printResults = false; //  = vals.size() == 10U;
	if (printResults) {
		for (const auto& l: work)
			std::cout << l.value << ' ';
		std::cout << std::endl;
		}

	auto smallest = work.begin();
	auto nextSmaller = work.begin() + 1;
	if (*nextSmaller < *smallest)
		std::swap(smallest, nextSmaller); // swapping iterators
	for (auto walker = work.begin() + 2; walker != work.end(); ++walker) {
		if (printResults) {
			std::cout << smallest->value << " " 
			          << nextSmaller->value << " " 
			          << walker->value << std::endl;
			}

		if (*walker < *nextSmaller) {
			if (*walker < *smallest) {
				nextSmaller = smallest;
				smallest = walker;  // we moved it away!
				}
			else {
				nextSmaller = walker;
				}
			}
		}

	if (printResults) {
		std::cout << smallest->value << " " 
		          << nextSmaller->value << std::endl;
		std::cout << std::distance(work.begin(), smallest) << " "
		          << std::distance(work.begin(), nextSmaller) << std::endl;
		}

//	We have to be careful here:
//		What if the "next smallest"	is the first element?
//  	What if the "smallest element" is in position #1
	if (smallest != work.begin()) {
		std::iter_swap(smallest, work.begin());
		if (nextSmaller == work.begin())
			nextSmaller = smallest;
		}
	if (nextSmaller != work.begin() + 1)
		std::iter_swap(nextSmaller, work.begin() + 1);
	
	Results ret = {comparisons, swaps, 0};
	if (printResults) {
		for (const auto& l: work)
			std::cout << l.value << ' ';
		std::cout << std::endl;
		}

	auto it = work.begin() + 1;
	assert(std::is_sorted(work.begin(), it + 1));
	assert(std::all_of(work.begin (), it,  [it](const Logger &v) {return v <= *it;}));
	assert(std::all_of(it + 1, work.end(), [it](const Logger &v) {return v >= *it;}));
	return ret;
}

void TestASize (size_t size)
{
	std::vector<Logger> v;
	v.reserve(size);
	for (int i = 0; i < size; ++i)
		v.push_back(gen32());
	const auto [psComp, psSwap, psTime] = partial_sort(v);
	const auto [neComp, neSwap, neTime] = nth_element(v);
  	const auto [ftComp, ftSwap, ftTime] = firstTwo(v);
	std::cout << "Size = " << size << std::endl;
	std::cout << "partial_sort: " << psComp << " comparisons and " << psSwap << " swaps" << " in " << psTime << "ms" << std::endl;
	std::cout << "nth_element:  " << neComp << " comparisons and " << neSwap << " swaps" << " in " << neTime << "ms" << std::endl;
	std::cout << "firstTwo:     " << ftComp << " comparisons and " << ftSwap << " swaps" << " in " << ftTime << "ms" << std::endl;
}

int main()
{
 	TestASize(10U);
	TestASize(100U);
	TestASize(1000U);
	TestASize(10000U);
	TestASize(100000U);
	TestASize(1000000U);
// 	TestASize(10000000U);
// 	TestASize(100000000U);
}
