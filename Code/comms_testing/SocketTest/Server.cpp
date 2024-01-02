// Server.cpp
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

    // Create a socket for the server to listen for incoming connections
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return -1;
    }

    // Set up the server address structure
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080); // You can use any port you like

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server listening for incoming connections...\n";

    // Accept two client connections
    SOCKET clientSocket1 = accept(serverSocket, NULL, NULL);
    SOCKET clientSocket2 = accept(serverSocket, NULL, NULL);

    if (clientSocket1 == INVALID_SOCKET || clientSocket2 == INVALID_SOCKET) {
        std::cerr << "Accept failed.\n";
        closesocket(clientSocket1);
        closesocket(clientSocket2);
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Two clients connected.\n";

    // Relay messages between clients
    char buffer[1024];
    int bytesReceived;
    do {
        // Receive from client 1
        bytesReceived = recv(clientSocket1, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Client 1 says: " << buffer << "\n";

            // Send to client 2
            send(clientSocket2, buffer, bytesReceived, 0);
        }

        // Receive from client 2
        bytesReceived = recv(clientSocket2, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Client 2 says: " << buffer << "\n";

            // Send to client 1
            send(clientSocket1, buffer, bytesReceived, 0);
        }

    } while (bytesReceived > 0);

    std::cout << "Connection closed by clients.\n";

    // Cleanup
    closesocket(clientSocket1);
    closesocket(clientSocket2);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
