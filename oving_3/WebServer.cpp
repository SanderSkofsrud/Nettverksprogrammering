#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

void handle_client(SOCKET client_socket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    std::cout << "Request from client:\n" << buffer << std::endl;

    // Building HTTP response
    std::string http_response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n\r\n"
            "<!DOCTYPE html>"
            "<html><body>"
            "<h1>HELLO! You have connected to the web server!</h1>"
            "<h2>Client Headers:</h2>"
            "<ul>";

    // Splitting request to extract headers
    std::istringstream request_stream(buffer);
    std::string line;
    while (std::getline(request_stream, line) && line != "\r") {
        http_response += "<li>" + line + "</li>";
    }
    http_response += "</ul></body></html>";

    // Sending response to the client
    send(client_socket, http_response.c_str(), http_response.length(), 0);

    // Closing connection
    closesocket(client_socket);
}

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return -1;
    }

    SOCKET server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int port = 9050; // Choose a port number above 1024

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    // Optional: Setsockopt may not be needed depending on your requirements
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
        std::cerr << "Setsockopt failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        SOCKET new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (new_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            continue; // Continue to the next iteration to accept new connections
        }
        handle_client(new_socket);
        break; // To only handle one client and then exit
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
