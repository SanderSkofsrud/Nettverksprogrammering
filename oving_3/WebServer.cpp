#include <iostream>          // Includes the Standard C++ Library for input/output operations
#include <string>            // Includes the Standard C++ Library for string handling
#include <winsock2.h>        // Includes Winsock API for network operations
#include <ws2tcpip.h>        // Includes extensions to the Winsock API
#include <sstream>           // Includes the Standard C++ Library for string stream operations

#pragma comment(lib, "ws2_32.lib") // Automatically link with ws2_32.lib for network functionalities

void handle_client(SOCKET client_socket) { // Function to handle client requests
    char buffer[1024];                     // Buffer to store client request
    memset(buffer, 0, sizeof(buffer));      // Initialize buffer to zeroes
    int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0); // Read data from client into buffer

    std::cout << "Request from client:\n" << buffer << std::endl; // Output client request to console

    // Building an HTTP response string
    std::string http_response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\n"
            "<!DOCTYPE html>"
            "<html><body>"
            "<h1>HELLO! You have connected to the web server!</h1>"
            "<h2>Client Headers:</h2>"
            "<ul>";

    // Extract and parse client headers from request
    std::istringstream request_stream(buffer); // Stream to process the buffer
    std::string line;                          // String to store each line of the request
    while (std::getline(request_stream, line) && line != "\r") { // Parse each line of the request
        http_response += "<li>" + line + "</li>";                // Append each line to the response
    }
    http_response += "</ul></body></html>"; // Close HTML tags in the response

    // Send the constructed response back to the client
    send(client_socket, http_response.c_str(), http_response.length(), 0);

    // Close the client socket
    closesocket(client_socket);
}

int main() {
    WSADATA wsaData;                                      // Structure for holding Windows Sockets initialization data
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);     // Initialize Winsock 2.2
    if (result != 0) {                                    // Check for errors in initialization
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return -1;
    }

    SOCKET server_fd;                               // Socket descriptor for the server
    struct sockaddr_in address;                     // Structure for holding internet address
    int opt = 1;                                    // Option for socket configuration
    int addrlen = sizeof(address);                  // Length of the address structure
    int port = 9050;                                // Port number for the server to listen on

    server_fd = socket(AF_INET, SOCK_STREAM, 0);    // Create a socket for the server
    if (server_fd == INVALID_SOCKET) {              // Check for socket creation errors
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    // Optional: Set socket options, here enabling address reuse
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
        std::cerr << "Setsockopt failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    address.sin_family = AF_INET;                   // Set the address family to AF_INET (IPv4)
    address.sin_addr.s_addr = INADDR_ANY;           // Listen on any IP address
    address.sin_port = htons(port);                 // Set the port number, converting to network byte order

    // Bind the server socket to the specified IP address and port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
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

    std::cout << "Server listening on port " << port << std::endl; // Indicate that the server is listening

    while (true) { // Infinite loop to handle incoming connections
        SOCKET new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen); // Accept a new connection
        if (new_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            continue; // Continue to the next iteration if accept fails
        }
        handle_client(new_socket); // Handle the client request
        break; // Break the loop to only handle one client and then exit
    }

    closesocket(server_fd); // Close the server socket
    WSACleanup();           // Clean up Winsock
    return 0;
}
