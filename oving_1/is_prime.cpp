#include "is_prime.h"

namespace oving_1 {

    bool is_prime(unsigned int n) {
        // Handle small numbers directly
        if (n <= 1) return false;
        if (n <= 3) return true; // 2 and 3 are prime

        // Check if n is divisible by 2 or 3
        if (n % 2 == 0 || n % 3 == 0) return false;

        // Check for divisibility by numbers of the form 6k ± 1
        for (unsigned int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }

        return true;
    }

} // namespace oving_1
