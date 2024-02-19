#include <iostream>      // Include for input-output stream objects
#include <winsock2.h>    // Include for Windows Socket API functions
#include <ws2tcpip.h>    // Include for additional socket functions and structures
#include <thread>        // Include for std::thread, to handle client connections concurrently
#include <sstream>       // Include for string stream processing

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib, necessary for using Winsock functions

// Function to handle individual client connections
void handle_client(SOCKET client_socket) {
    char buffer[1024];  // Buffer for storing received data from the client
    memset(buffer, 0, sizeof(buffer)); // Initialize buffer to zero
    recv(client_socket, buffer, 1023, 0); // Receive data from client socket

    // Variables to store the parsed operation and numbers
    char operation;
    int num1, num2;
    std::istringstream ss(buffer); // Use string stream to parse the received data
    ss >> operation >> num1 >> num2; // Extract operation and numbers from the received string

    // Print the received message details
    std::cout << "Received message: " << std::endl;
    std::cout << "Num1: " << num1 << std::endl;
    std::cout << "Operation: " << operation << std::endl;
    std::cout << "Num2: " << num2 << std::endl;
    std::cout << "\n" << std::endl;

    int result; // Variable to store the result of the operation
    std::string response; // String to store the response message
    // Perform the operation based on the received operator
    switch(operation) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
            if(num2 == 0) { // Check for division by zero
                response = "Error: Division by zero";
                send(client_socket, response.c_str(), response.length(), 0);
                closesocket(client_socket);
                return;
            }
            result = num1 / num2;
            break;
        default: // Handle invalid operations
            response = "Error: Invalid operation";
            send(client_socket, response.c_str(), response.length(), 0);
            closesocket(client_socket);
            return;
    }

    response = "Result: " + std::to_string(result); // Prepare the response message
    send(client_socket, response.c_str(), response.length(), 0); // Send the response to the client
    closesocket(client_socket); // Close the client socket
}

int main() {
    WSADATA wsaData; // Structure for holding Windows Sockets initialization information
    int result = WSAStartup(MAKEWORD(2,2), &wsaData); // Initialize Winsock 2.2
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return -1;
    }

    // Declare variables for server socket and new client sockets
    SOCKET server_fd, new_socket;
    struct sockaddr_in address; // Structure for the server address
    int addrlen = sizeof(address); // Length of the address
    int port = 9000; // Port number for the server to listen on

    // Create a new socket for the server
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    // Setup the server address structure
    address.sin_family = AF_INET; // Set the address family to IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Use any available interface
    address.sin_port = htons(port); // Set the port number, converting it to network byte order

    // Bind the socket to the server address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    // Server loop to accept incoming connections
    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            continue;
        }

        // Create a new thread to handle the client connection
        std::thread t([=](){ handle_client(new_socket); });
        t.detach(); // Detach the thread to handle the client independently
    }

    // Clean up sockets and Winsock
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
