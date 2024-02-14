#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <sstream>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

void handle_client(SOCKET client_socket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, 1023, 0);

    char operation;
    int num1, num2;
    std::istringstream ss(buffer);
    ss >> operation >> num1 >> num2;

    std::cout << "Received message: " << std::endl;
    std::cout << "Num1: " << num1 << std::endl;
    std::cout << "Operation: " << operation << std::endl;
    std::cout << "Num2: " << num2 << std::endl;
    std::cout << "\n" << std::endl;

    int result;
    std::string response;
    switch(operation) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
            if(num2 == 0) {
                response = "Error: Division by zero";
                send(client_socket, response.c_str(), response.length(), 0);
                closesocket(client_socket);
                return;
            }
            result = num1 / num2;
            break;
        default:
            response = "Error: Invalid operation";
            send(client_socket, response.c_str(), response.length(), 0);
            closesocket(client_socket);
            return;
    }

    response = "Result: " + std::to_string(result);
    send(client_socket, response.c_str(), response.length(), 0);
    closesocket(client_socket);
}

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return -1;
    }

    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int port = 9000;

    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
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
        std::cout << "Waiting for a client to connect..." << std::endl;
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::cout << "Client connected" << std::endl;

        std::thread t([=](){ handle_client(new_socket); });
        t.detach();
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
