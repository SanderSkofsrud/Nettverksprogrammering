#include <iostream>                     // Including standard IO library
#include <string>                       // Including library for using strings
#include "oving_1/prime_calculations.h" // Including the custom header for prime number calculations

int main() {
    while (true) {
        std::cout << "\nChoose an exercise to run or press enter to quit:"
                  << "\n1. Exercise 1: Find Prime Numbers"
                  << "\n2. Exercise 2: Yet to be done\n";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            oving_1::run(); // Execute the prime number calculation function
        } else if (choice.empty()) {
            break; // Exit the loop and end the program
        } else {
            std::cout << "Invalid choice, please try again." << std::endl;
        }
    }
    return 0; // Return 0 to indicate successful execution
}
