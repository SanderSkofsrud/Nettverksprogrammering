#ifndef OVING_1_PRIME_CALCULATIONS_H
#define OVING_1_PRIME_CALCULATIONS_H

#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>
#include "is_prime.h"

namespace oving_1 {
    void find_primes_in_range(unsigned int start, unsigned int end, std::vector<unsigned int>& primes);
    void run();
}

#endif // OVING_1_PRIME_CALCULATIONS_H
