#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return -1;
    }

    while(true) {
        SOCKET sock = INVALID_SOCKET;
        struct sockaddr_in serv_addr;
        char buffer[1024] = {0};

        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return -1;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(9000);

        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            std::cerr << "Invalid address/ Address not supported" << std::endl;
            closesocket(sock);
            WSACleanup();
            return -1;
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Connection Failed with error: " << WSAGetLastError() << std::endl;
            closesocket(sock);
            WSACleanup();
            return -1;
        }

        char operation;
        int num1, num2;
        std::cout << "Enter your first number: ";
        std::cin >> num1;
        std::cout << "Enter your second number: ";
        std::cin >> num2;
        std::cout << "Enter the operation (+, -, *, /): ";
        std::cin >> operation;
        std::cin.ignore();

        std::stringstream ss;
        ss << operation << " " << num1 << " " << num2;
        std::string message = ss.str();

        send(sock, message.c_str(), message.length(), 0);
        int valread = recv(sock, buffer, 1024, 0);
        std::cout << "Answer from server: " << buffer << std::endl;

        closesocket(sock);

        std::string decision;
        std::cout << "More math? (y/n): ";
        std::getline(std::cin, decision);
        if (decision != "y" && decision != "Y") {
            break;
        }
    }

    WSACleanup();
    return 0;
}
