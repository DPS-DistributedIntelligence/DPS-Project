//
// Created by leand on 25.12.2023.
//

#include "CommsModule.h"

namespace Modules {
    int CommsModule::initialize(const string& ip_address, u_short port) {

        //Initializing the winsock.dll
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            return -1;
        }

        // Create a socket for the client
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            WSACleanup();
            return -1;
        }

        // Set up the server address structure
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ip_address.c_str()); // Server's IP address
        serverAddr.sin_port = htons(port); // Server's port

        return 1;
    }

    int CommsModule::connect_to_Server() {
        // Connect to the server
        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(clientSocket);
            WSACleanup();
            return -1;
        }

        return 1;
    }

    int CommsModule::send_message(const std::string& message) {
        //Getting the length in bytes
        int bytes_length = (int)strlen(message.c_str());

        //Sending and comparing result (if result != bytes_length --> error)
        if(send(clientSocket, message.c_str(), bytes_length, 0) == bytes_length)
        {
            return 1;
        }
        return -1;
    }

} // Modules