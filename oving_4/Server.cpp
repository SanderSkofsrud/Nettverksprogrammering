#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

void handle_client(const std::string& message, struct sockaddr_in client_addr, SOCKET server_socket) {
    char operation;
    int num1, num2, result = 0;
    std::istringstream ss(message);
    ss >> operation >> num1 >> num2;

    std::string response;
    switch (operation) {
        case '+':
            result = num1 + num2;
            response = "Result: " + std::to_string(result);
            break;
        case '-':
            result = num1 - num2;
            response = "Result: " + std::to_string(result);
            break;
        case '*':
            result = num1 * num2;
            response = "Result: " + std::to_string(result);
            break;
        case '/':
            if (num2 == 0) {
                response = "Error: Division by zero";
            } else {
                result = num1 / num2;
                response = "Result: " + std::to_string(result);
            }
            break;
        default:
            response = "Error: Invalid operation";
            break;
    }

    sendto(server_socket, response.c_str(), response.length(), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
}

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return -1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    struct sockaddr_in server_addr;
    int port = 9000;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    char buffer[1024];
    struct sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_size);
        if (bytes_received == SOCKET_ERROR) {
            std::cerr << "Error in recvfrom: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::string message(buffer, bytes_received);
        handle_client(message, client_addr, server_socket);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
