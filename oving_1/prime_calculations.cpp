// Include header files for prime calculation functions and standard libraries
#include "prime_calculations.h" // Include custom header file for prime calculations
#include "is_prime.h"           // Include custom header file to check if a number is prime
#include <iostream>             // Include a standard library for input/output operations
#include <vector>               // Include a standard library for using the vector container
#include <thread>               // Include a standard library for using threads
#include <algorithm>            // Include a standard library for algorithms like sort
#include <limits>               // Include a standard library for numeric limits
#include <chrono>               // Include a standard library for dealing with time

// Define a namespace 'oving_1'
namespace oving_1 {

    // Function to find prime numbers in a given range and store them in a vector
    /**
     * Finds all prime numbers in a specified range and appends them to a vector.
     *
     * @param start The starting number of the range.
     * @param end The ending number of the range.
     * @param primes A reference to a vector where the found prime numbers will be stored.
     */
    void find_primes_in_range(unsigned int start, unsigned int end, std::vector<unsigned int>& primes) {
        for (unsigned int i = start; i < end; ++i) { // Loop through the range from start to end
            if (is_prime(i)) { // Check if the current number 'i' is prime
                primes.push_back(i); // Add the prime number to the primes vector
            }
        }
    }

    // Main function to run the prime number finding operation using multithreading
    void run() {
        unsigned int start, end, thread_count; // Declare variables for start, end of range, and number of threads

        // Input loop for start of range
        while (true) {
            std::cout << "Enter start of range (positive integer): ";
            // Validate input for start of range
            if (!(std::cin >> start)) {
                std::cout << "Invalid input. Please enter a positive integer." << std::endl;
                std::cin.clear(); // Clear the error flag on cin
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line
                continue; // Continue to the next iteration of the loop
            }
            break; // Exit the loop if input is valid
        }

        // Input loop for end of range
        while (true) {
            std::cout << "Enter end of range (greater than start): ";
            // Validate input for end of range
            if (!(std::cin >> end) || end <= start) {
                std::cout << "Invalid input. Please enter a positive integer greater than start." << std::endl;
                std::cin.clear(); // Clear the error flag on cin
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line
                continue; // Continue to the next iteration of the loop
            }
            break; // Exit the loop if input is valid
        }

        // Determine the maximum number of threads that can be used
        unsigned int max_threads = std::thread::hardware_concurrency(); // Get the number of hardware threads
        max_threads = max_threads == 0 ? 4 : max_threads; // Use 4 threads if hardware_concurrency returns 0

        // Calculate the size of the range to be processed
        unsigned int range_size = end - start + 1; // Compute the total number of numbers in the range

        // Input loop for number of threads
        while (true) {
            std::cout << "Enter number of threads (positive integer, not greater than " << max_threads << "): ";
            // Validate input for number of threads
            if (!(std::cin >> thread_count) || thread_count == 0 || thread_count > max_threads) {
                std::cout << "Invalid input. Please enter a positive integer not exceeding " << max_threads << "." << std::endl;
                std::cin.clear(); // Clear the error flag on cin
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line
                continue; // Continue to the next iteration of the loop
            } else if (thread_count > range_size) {
                std::cout << "Number of threads cannot exceed " << range_size << " . Please enter a smaller number." << std::endl;
                std::cin.clear(); // Clear the error flag on cin
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line
                continue; // Continue to the next iteration of the loop
            }
            break; // Exit the loop if input is valid
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

        // Prepare for multithreading
        std::vector<std::thread> threads; // Vector to store the threads
        std::vector<std::vector<unsigned int>> prime_vectors(thread_count); // Vector of vectors to store primes found by each thread

        // Calculate the size of each chunk of range to be processed by each thread
        unsigned int chunk_size = range_size / thread_count; // Size of each chunk
        unsigned int remaining = range_size % thread_count; // Remaining numbers after equally dividing the range

        // Start timing the operation
        auto start_time = std::chrono::high_resolution_clock::now(); // Record the start time

        // Create threads to find primes
        for (unsigned int i = 0; i < thread_count; ++i) {
            unsigned int thread_start = start + i * chunk_size;
            unsigned int thread_end = thread_start + chunk_size;

            // Add one extra number to the range of the first 'remaining' threads
            if (i < remaining) {
                thread_end++;
            }

            // Adjust the end for the last thread to ensure the entire range is covered
            if (i == thread_count - 1) {
                thread_end = end;
            }

            threads.emplace_back(find_primes_in_range, thread_start, thread_end, std::ref(prime_vectors[i]));
        }

        // Wait for all threads to finish
        for (auto &t : threads) {
            t.join(); // Join each thread with the main thread
        }

        // Stop timing the operation
        auto end_time = std::chrono::high_resolution_clock::now(); // Record the end time

        // Combine the primes found by all threads
        std::vector<unsigned int> all_primes; // Vector to store all found primes
        for (const auto& vec : prime_vectors) {
            all_primes.insert(all_primes.end(), vec.begin(), vec.end()); // Append primes from each thread's vector to all_primes
        }
        std::sort(all_primes.begin(), all_primes.end()); // Sort the combined list of primes

        // Output the results
        std::cout << "Prime numbers: ";
        for (auto prime : all_primes) {
            std::cout << prime << " ";
        }
        // Output the time taken for the operation
        std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " ms" << std::endl;
        std::cout << std::endl;
    }

} // End of namespace oving_1
