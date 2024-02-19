#include <iostream>      // Include for input-output stream objects
#include <winsock2.h>    // Include for Windows Socket API functions
#include <ws2tcpip.h>    // Include for additional socket functions and structures
#include <sstream>       // Include for string stream processing

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib, necessary for using Winsock functions

int main() {
    WSADATA wsaData; // Structure for holding Windows Sockets initialization information
    // Initialize Winsock 2.2
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return -1;
    }

    // Client main loop
    while(true) {
        SOCKET sock = INVALID_SOCKET; // Declare a socket for the client
        struct sockaddr_in serv_addr; // Structure for the server address
        char buffer[1024] = {0}; // Buffer for storing server response

        // Create a new socket
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return -1;
        }

        // Setup the server address structure
        serv_addr.sin_family = AF_INET; // Set the address family to IPv4
        serv_addr.sin_port = htons(9000); // Set the port number (9000) in network byte order

        // Convert IPv4 address from text to binary form
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            std::cerr << "Invalid address/ Address not supported" << std::endl;
            closesocket(sock);
            WSACleanup();
            return -1;
        }

        // Connect to the server
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Connection Failed with error: " << WSAGetLastError() << std::endl;
            closesocket(sock);
            WSACleanup();
            return -1;
        }

        // Variables to store user input
        char operation;
        int num1, num2;

        // Get user input
        std::cout << "Enter your first number: ";
        std::cin >> num1;
        std::cout << "Enter your second number: ";
        std::cin >> num2;
        std::cout << "Enter the operation (+, -, *, /): ";
        std::cin >> operation;
        std::cin.ignore(); // Clear the input buffer

        // Create a stringstream and compose the message to be sent
        std::stringstream ss;
        ss << operation << " " << num1 << " " << num2;
        std::string message = ss.str();

        // Send the composed message to the server
        send(sock, message.c_str(), message.length(), 0);
        // Receive the response from the server
        int valread = recv(sock, buffer, 1024, 0);
        std::cout << "Answer from server: " << buffer << std::endl;

        closesocket(sock); // Close the client socket

        // Ask the user if they want to continue
        std::string decision;
        std::cout << "More math? (y/n): ";
        std::getline(std::cin, decision);
        if (decision != "y" && decision != "Y") {
            break; // Break the loop if the user does not want to continue
        }
    }

    // Cleanup and exit
    WSACleanup();
    return 0;
}
