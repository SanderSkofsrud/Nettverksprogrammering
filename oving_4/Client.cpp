#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return -1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    while (true) {
        char operation;
        int num1, num2;

        std::cout << "Enter your first number: ";
        std::cin >> num1;
        std::cout << "Enter your second number: ";
        std::cin >> num2;
        std::cout << "Enter the operation (+, -, *, /): ";
        std::cin >> operation;

        std::stringstream ss;
        ss << operation << " " << num1 << " " << num2;
        std::string message = ss.str();

        sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        char buffer[1024] = {0};
        int serv_addr_size = sizeof(serv_addr);
        int bytes_received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&serv_addr, &serv_addr_size);
        if (bytes_received == SOCKET_ERROR) {
            std::cerr << "recvfrom failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::cout << "Answer from server: " << buffer << std::endl;

        std::string decision;
        std::cout << "More math? (y/n): ";
        std::cin >> decision;
        if (decision != "y" && decision != "Y") {
            break;
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
