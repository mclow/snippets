#include <random>
#include <chrono>
#include <cassert>
#include <iostream>

// A simple replacement for rand() using C++11 random facilities
// rand()
//	Returns a pseudo-random integral number in the range between 0 and RAND_MAX.

// simple_rand takes two params (min & max) and returns a number in the range [min, max)
//	Seeded from the system clock
//	Note: T must be an integral type
template <typename T>
struct simple_rand
{
	static const size_t seed_from_clock = (size_t) -1;
	simple_rand(T min, T max, size_t seed = seed_from_clock) : 
		gen_(seed != seed_from_clock 
			? seed 
			: std::chrono::system_clock::now().time_since_epoch().count()), 
		dist_(min, max) {}

	T operator ()() { return dist_(gen_); }

	std::default_random_engine gen_;
	std::uniform_int_distribution<T> dist_;
};


int main ()
{
	simple_rand<int> r (1, 6);
	size_t counts [6] = {0};
	for (size_t i = 0; i < 1000; ++ i)
	{
		int val = r();
		assert(val >= 1 && val <= 6);
		counts[val - 1]++;
	}
	for (auto c: counts)
		std::cout << c << ' ';
	std::cout << std::endl;
}
