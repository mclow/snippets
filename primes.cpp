#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

typedef std::vector<char> Primes;

Primes sieve(unsigned long size) {
	Primes res(size, 1);
	
	res[0] = 0;
	res[1] = 0;
	for (unsigned long i = 2; i < res.size(); ++i) {
		if (res[i] == 0) continue; // already not a prime
		unsigned long j = i + i;
		while (j < res.size()) {
			res[j] = 0;
			j += i;
			}
		}
	return res;
	}


// count all the primes in a sequence (n, n+step, n+2*step,...) for count values
void findEm(const Primes &primes, unsigned long max, 
            unsigned long count,  unsigned long step) {
	assert(primes.size() > max + (count * step));
 	for (unsigned long i = 2; i < max; ++i) {
 		if (primes[i]) {
 			int cnt = 1;
 			unsigned long val = i;
 			for (int j = 0; j < count; ++j) {
 				val += step;
				if (primes[val])
 					++cnt;
 				}
			if (cnt > 16)
				std::cout << i << " (" << step << "): " << cnt << std::endl;
	 		}
		}
	}

// find all the consecutive primes in a sequence (n, n+step, n+2*step,...)
void findConsecutive(const Primes &primes, unsigned long max, unsigned long step) {
 	for (unsigned long i = 2; i < max; ++i) {
		int cnt = 0;
		unsigned long val = i;
 		while (val < primes.size() && primes[val]) {
 			++cnt;
			val += step;
			}			

		if (cnt > 10) {
			if (val >= primes.size())
				std::cout << "Consecutive: " << i << " (" << step << "): " << cnt << " (and maybe more)" << std::endl;
			else
				std::cout << "Consecutive: " << i << " (" << step << "): " << cnt << std::endl;
			}
		}
	}

void findOne(const Primes &primes, unsigned long value, 
            unsigned long count,  unsigned long step) {
	assert(primes.size() > value + (count * step));
	if (primes[value]) {
		int cnt = 1;
		unsigned long val = value;
		// we go up to 'count - 1' because we've already counted one
		for (int j = 0; j < count - 1; ++j) {
			val += step;
			if (primes[val])
 				++cnt;
 			std::cout << val << (primes[val] ? " (prime)" : " (not prime)") << std::endl;
 			}
			if (cnt > 8)
				std::cout << value << " (" << step << "): " << cnt << std::endl;
		}
	}

int main (int, char **) {
	const unsigned long N    = 10000000;
	const unsigned long STEP =    10000;
	const unsigned long COUNT = 20;

	Primes primes = sieve(N + (COUNT * STEP) + 1);

// 	findOne(primes, 5UL, 10UL, 6UL);
	
// 	for (unsigned long step = 2; step <= STEP; step += 2)
// 		findEm(primes, N, COUNT, step);

	for (unsigned long step = 2; step <= STEP; step += 2)
		findConsecutive(primes, N, step);
	}