// Client.cpp
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return -1;
    }

    // Create a socket for the client
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return -1;
    }

    // Set up the server address structure
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server's IP address
    serverAddr.sin_port = htons(8080); // Server's port

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed.\n";
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to the server.\n";

    // Send and receive messages
    char message[1024];
    while (true) {
        std::cout << "Enter a message to send to the other client (or type 'exit' to quit): ";
        std::cin.getline(message, sizeof(message));

        // Send message to the server
        send(clientSocket, message, strlen(message), 0);

        if (strcmp(message, "exit") == 0) {
            break; // Exit the loop if the user types 'exit'
        }

        // Receive and print the echoed message from the server
        int bytesReceived = recv(clientSocket, message, sizeof(message), 0);
        if (bytesReceived > 0) {
            message[bytesReceived] = '\0'; // Null-terminate the received data
            std::cout << "Received from the other client: " << message << "\n";
        }
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
