// Header guards to prevent multiple inclusions of this header file.
#ifndef OVING_1_PRIME_CALCULATIONS_H
#define OVING_1_PRIME_CALCULATIONS_H

// Including necessary libraries and headers.
#include <vector>    // Standard vector container for dynamic arrays.
#include <thread>    // For creating and managing threads in multi-threading.
#include <iostream>  // Standard library for input and output operations.
#include <algorithm> // Contains various algorithm functions like sort, find, etc.
#include <cmath>     // Contains math functions like sqrt, pow, etc.
#include <limits>    // Provides limits of fundamental types.
#include "is_prime.h"// Custom header for checking if a number is prime.

// Define a namespace to encapsulate the functions and avoid naming conflicts.
namespace oving_1 {

    /**
     * Function to find prime numbers within a specified range.
     * This function iterates through all the numbers in the given range and checks
     * if each number is a prime using the `is_prime` function. If a number is prime,
     * it is added to the provided vector.
     *
     * @param start The starting integer of the range (inclusive).
     * @param end The ending integer of the range (exclusive).
     * @param primes Reference to a vector where the prime numbers will be stored.
     */
    void find_primes_in_range(unsigned int start, unsigned int end, std::vector<unsigned int>& primes) {
        for (unsigned int i = start; i < end; ++i) {
            if (is_prime(i)) {
                primes.push_back(i); // Add the prime number to the vector.
            }
        }
    }

    /**
     * Main function to execute the prime number calculation program.
     * This function demonstrates user input handling, basic error handling,
     * and working with multiple threads to divide the task of finding prime
     * numbers in a range. It takes user input for the range and the number of
     * threads to use, and then divides the range among the threads for parallel
     * processing.
     */
    void run() {
        unsigned int start, end, thread_count; // Variables to store user inputs.

        // Input loop for the start of the range with error handling.
        while (true) {
            std::cout << "Enter start of range (positive integer): ";
            if (!(std::cin >> start)) { // Check for invalid input.
                std::cout << "Invalid input. Please enter a positive integer." << std::endl;
                std::cin.clear(); // Reset error flags on cin.
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line.
                continue;
            }
            break;
        }

        // Input loop for the end of the range with error handling.
        while (true) {
            std::cout << "Enter end of range (greater than start): ";
            if (!(std::cin >> end) || end <= start) { // Check for invalid or illogical input.
                std::cout << "Invalid input. Please enter a positive integer greater than start." << std::endl;
                std::cin.clear(); // Reset error flags on cin.
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            break;
        }

        // Get the number of supported concurrent threads.
        unsigned int max_threads = std::thread::hardware_concurrency();
        if (max_threads == 0) {
            max_threads = 4; // Fallback if hardware concurrency is not available.
        }

        // Calculate the size of the range.
        unsigned int range_size = end - start + 1;

        // Input loop for the number of threads with error handling.
        while (true) {
            std::cout << "Enter number of threads (positive integer, not greater than " << max_threads << "): ";
            if (!(std::cin >> thread_count) || thread_count == 0 || thread_count > max_threads) {
                std::cout << "Invalid input. Please enter a positive integer not exceeding " << max_threads << "." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            } else if (thread_count > range_size) {
                std::cout << "Number of threads cannot exceed " << range_size << " . Please enter a smaller number." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            break;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer.

        std::vector<std::thread> threads; // Vector to store threads.
        // Vector of vectors for storing primes from each thread.
        std::vector<std::vector<unsigned int>> prime_vectors(thread_count);

        unsigned int chunk_size = (end - start + 1) / thread_count; // The size of the range each thread will process.
        unsigned int remaining = (end - start + 1) % thread_count; // The remaining range to be distributed.

        // Creating and launching threads for parallel computation.
        for (unsigned int i = 0; i < thread_count; ++i) {
            unsigned int thread_start = start + i * chunk_size;
            unsigned int thread_end = thread_start + chunk_size + (i < remaining ? 1 : 0);

            // Construct and add a new thread to the vector.
            threads.emplace_back(find_primes_in_range, thread_start, thread_end, std::ref(prime_vectors[i]));
        }

        // Wait for all threads to complete.
        for (auto &t : threads) {
            t.join();
        }

        // Merge and sort the prime numbers from all threads.
        std::vector<unsigned int> all_primes;
        for (const auto& vec : prime_vectors) {
            all_primes.insert(all_primes.end(), vec.begin(), vec.end()); // Combine vectors.
        }
        std::sort(all_primes.begin(), all_primes.end()); // Sort the combined vector.

        // Display the prime numbers found.
        std::cout << "Prime numbers: ";
        for (auto prime : all_primes) {
            std::cout << prime << " "; // Print each prime number.
        }
        std::cout << std::endl;
    }

}

#endif // End of header guard OVING_1_PRIME_CALCULATIONS_H
