#include <iostream>    // Including standard IO library
#include <vector>      // Including library for using vectors
#include <future>      // Including library for asynchronous operations
#include <algorithm>   // Including library for algorithms like sort
#include "is_prime.h"  // Including the custom header for prime number checking

namespace oving_1 {  // Namespace declaration to avoid naming conflicts

    /**
     * @brief Reads a line of input from the user.
     *
     * This function reads a complete line of text from standard input (cin).
     * It's used to gather input from the user in a string format.
     *
     * @return The input line as a string.
     */
    std::string read_input() {
        std::string input;          // Declaring a string to store the input
        std::getline(std::cin, input);  // Reading a line from standard input into the string
        return input;               // Returning the input string
    }

    /**
     * @brief Reads a positive integer from user input.
     *
     * This function continually prompts the user for input until a valid positive integer is entered.
     * It uses exception handling to manage non-integer inputs.
     *
     * @return The positive integer entered by the user.
     */
    unsigned int read_positive_integer() {
        while (true) {  // Infinite loop to keep asking for input until valid
            std::string input = read_input();  // Reading input as a string
            try {
                unsigned int num = std::stoul(input);  // Trying to convert the string to an unsigned integer
                if (num > 0) {
                    return num;  // Return the number if it's a positive integer
                } else {
                    std::cout << "Please enter a positive integer." << std::endl;  // Prompt if the number is not positive
                }
            } catch (const std::exception&) {  // Catch block for handling conversion errors
                std::cout << "Please enter a positive integer." << std::endl;  // Prompt for valid input on error
            }
        }
    }

    /**
     * @brief Runs the prime number calculation process.
     *
     * This function prompts the user for a start and end range, and the number of threads to use.
     * It then finds all prime numbers in that range using asynchronous tasks (multithreading).
     */
    void run() {
        std::cout << "Enter the start of the range: ";  // Prompt for start of range
        unsigned int start = read_positive_integer();   // Reading start of the range

        std::cout << "Enter the end of the range: ";    // Prompt for end of range
        unsigned int end = read_positive_integer();     // Reading end of the range

        // Check if the end of the range is valid
        if (end <= start) {
            std::cout << "End must be greater than start." << std::endl;
            return;  // Exit the function if the range is invalid
        }

        std::cout << "Enter the number of threads: ";  // Prompt for number of threads
        unsigned int thread_count = read_positive_integer();  // Reading number of threads

        std::vector<std::future<std::vector<unsigned int>>> futures;  // Vector to store future results of asynchronous tasks
        unsigned int range = end - start + 1;                        // Calculating the range of numbers
        unsigned int chunk_size = range / thread_count;              // Determining the size of each chunk for a thread
        unsigned int remaining = range % thread_count;               // Calculating any remaining numbers after chunk division

        // Loop to create asynchronous tasks
        for (unsigned int i = 0; i < thread_count; ++i) {
            // Calculating start and end for each thread's chunk
            unsigned int thread_start = start + i * chunk_size + std::min(i, remaining);
            unsigned int thread_end = thread_start + chunk_size + (i < remaining ? 1 : 0);

            // Launching an asynchronous task to find primes in a chunk of the range
            futures.push_back(std::async(std::launch::async, [thread_start, thread_end]() {
                std::vector<unsigned int> primes;  // Vector to store primes found by this thread
                for (unsigned int j = thread_start; j < thread_end; ++j) {
                    if (is_prime(j)) {
                        primes.push_back(j);  // Add number to vector if it's a prime
                    }
                }
                return primes;  // Return the vector of primes found
            }));
        }

        std::vector<unsigned int> all_primes;  // Vector to store all found primes
        // Loop to wait for each asynchronous task to finish and collect their results
        for (auto &f : futures) {
            std::vector<unsigned int> primes = f.get();  // Getting the result of a future (blocking call)
            all_primes.insert(all_primes.end(), primes.begin(), primes.end());  // Adding the primes found by this task to the main vector
        }

        std::sort(all_primes.begin(), all_primes.end());  // Sorting the vector of all primes
        std::cout << "Prime numbers: ";  // Output the found prime numbers
        for (auto prime : all_primes) {
            std::cout << prime << " ";  // Printing each prime number
        }
        std::cout << std::endl;  // New line after printing all primes
    }

}  // End of namespace oving_1
