#include <iostream>                     // Including standard IO library
#include <string>                       // Including library for using strings
#include "oving_1/prime_calculations.h" // Including the custom header for prime number calculations

int main() { // Main function, the entry point of the program
    while (true) { // Infinite loop to keep asking for input until the user quits
        // Prompting the user for input and displaying the menu
        std::cout << "\nChoose an exercise to run or press enter to quit:"
                  << "\n1. Exercise 1: Find Prime Numbers"
                  << "\n2. Exercise 2: Yet to be done\n";

        std::string choice; // Declaring a string to store the user's choice
        std::getline(std::cin, choice); // Reading a line from standard input into the string

        // Checking the user's choice and running the corresponding exercise
        if (choice == "1") {
            oving_1::run(); // If the user chooses 1, run the prime number calculation process
        } else if (choice.empty()) {
            break; // If the user presses enter, break out of the loop and quit
        } else {
            std::cout << "Invalid choice, please try again." << std::endl; // If the user enters an invalid choice, prompt again
        }
    }
    return 0; // Return 0 to indicate successful execution
}
