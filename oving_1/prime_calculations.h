// OVING_1_PRIME_CALCULATIONS_H
#ifndef OVING_1_PRIME_CALCULATIONS_H
#define OVING_1_PRIME_CALCULATIONS_H

#include <vector>    // Includes the standard vector container, similar to Java's ArrayList
#include <thread>    // Includes support for multi-threading, which is similar to Java's Thread class but with different syntax
#include <iostream>  // For input/output operations, akin to Java's System.in and System.out
#include <algorithm> // Provides a collection of algorithm functions such as sort
#include <cmath>     // For mathematical functions
#include <limits>    // Provides information about numeric limits of types
#include "is_prime.h"// Custom header file for prime number checking function

namespace oving_1 {

    /**
     * Finds prime numbers within a given range and stores them in a vector.
     * Similar to Java, it passes the vector by reference to modify the original object.
     *
     * @param start The starting integer of the range (inclusive).
     * @param end The ending integer of the range (exclusive).
     * @param primes A reference to a vector to store the found prime numbers.
     */
    void find_primes_in_range(unsigned int start, unsigned int end, std::vector<unsigned int>& primes) {
        for (unsigned int i = start; i < end; ++i) {
            if (is_prime(i)) {
                primes.push_back(i); // Adds the prime number to the end of the vector, similar to ArrayList.add in Java
            }
        }
    }

    /**
     * The main function to run the prime number calculation.
     * It demonstrates input handling, loop control, and basic exception handling in C++.
     */
    void run() {
        unsigned int start, end, thread_count; // Variables for user input

        // Loop for start input with error handling
        while (true) {
            std::cout << "Enter start of range (positive integer): ";
            if (!(std::cin >> start)) {
                std::cout << "Invalid input. Please enter a positive integer." << std::endl;
                std::cin.clear(); // Clears the error flag on cin
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignores the rest of the current line
                continue;
            }
            break;
        }

        // Loop for end input with error handling
        while (true) {
            std::cout << "Enter end of range (greater than start): ";
            if (!(std::cin >> end) || end <= start) {
                std::cout << "Invalid input. Please enter a positive integer greater than start." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            break;
        }

        // Loop for thread count input with error handling
        while (true) {
            std::cout << "Enter number of threads (positive integer, not greater than the range): ";
            if (!(std::cin >> thread_count) || thread_count == 0 || thread_count > (end - start + 1)) {
                std::cout << "Invalid input. Please enter a positive integer not exceeding the range (" << (end - start + 1) << ")." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            break;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clears the input buffer

        std::vector<std::thread> threads; // Vector to hold multiple thread objects
        std::vector<std::vector<unsigned int>> prime_vectors(thread_count); // A vector of vectors to store primes found by each thread

        unsigned int chunk_size = (end - start + 1) / thread_count; // Calculates size of range for each thread
        unsigned int remaining = (end - start + 1) % thread_count; // Calculates the remainder for even distribution among threads

        // Creating threads to perform parallel computation
        for (unsigned int i = 0; i < thread_count; ++i) {
            unsigned int thread_start = start + i * chunk_size;
            unsigned int thread_end = thread_start + chunk_size + (i < remaining ? 1 : 0);

            // Emplace_back constructs the thread object in-place and adds it to the threads vector.
            // It's more efficient than push_back as it avoids an extra copy or move operation.
            threads.emplace_back(find_primes_in_range, thread_start, thread_end, std::ref(prime_vectors[i]));
        }

        // Joining threads - waiting for all threads to complete execution
        for (auto &t : threads) {
            t.join(); // join() waits for a thread to finish its execution
        }

        // Merging all primes into a single vector and sorting them
        std::vector<unsigned int> all_primes;
        for (const auto& vec : prime_vectors) {
            all_primes.insert(all_primes.end(), vec.begin(), vec.end()); // Combines vectors from each thread
        }
        std::sort(all_primes.begin(), all_primes.end()); // Sorts the combined vector of primes

        // Displaying the prime numbers
        std::cout << "Prime numbers: ";
        for (auto prime : all_primes) {
            std::cout << prime << " "; // Iterates and prints each prime number
        }
        std::cout << std::endl;
    }

} // namespace oving_1

#endif // OVING_1_PRIME_CALCULATIONS_H
