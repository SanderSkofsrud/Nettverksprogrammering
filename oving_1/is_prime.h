#ifndef OVING_1_IS_PRIME_H  // Start of include guard
#define OVING_1_IS_PRIME_H

namespace oving_1 {  // Declaring a namespace to encapsulate the function

    /**
     * @brief Checks whether a number is prime.
     *
     * This function implements a basic prime checking algorithm. It first handles
     * edge cases (like numbers less than 2). For other numbers, it checks divisibility
     * by 2 and 3 and then checks for factors in the range 5 to sqrt(n) in steps of 6.
     *
     * @param n The unsigned integer to be checked.
     * @return true if 'n' is a prime number, false otherwise.
     */
    bool is_prime(unsigned int n) {
        if (n <= 1) return false;  // Handling numbers less than 2 which are not prime
        if (n <= 3) return true;   // 2 and 3 are prime numbers

        // Checking for divisibility by 2 and 3
        if (n % 2 == 0 || n % 3 == 0) return false;

        // Checking for prime by testing for factors
        for (unsigned int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }

        return true;  // Returning true if no factors found, number is prime
    }

}  // End of namespace oving_1

#endif  // End of include guard
