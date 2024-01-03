//
// Created by leand on 25.12.2023.
//

#include "CommsModule.h"

namespace Modules {
    CommsModule::CommsModule(long timeout_us)
    {
        //Initializing the timeout
        timeout.tv_sec = 0;
        timeout.tv_usec = timeout_us;
    }

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

        //Set the socket to non-blocking mode
        u_long mode = 1;
        if (ioctlsocket(clientSocket, FIONBIO, &mode) != 0) {
            closesocket(clientSocket);
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
        int connection_result = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        //Result is error --> connection might be under initialization
        if (connection_result == SOCKET_ERROR) {

            //Getting the exact error
            int error = WSAGetLastError();

            //If connection seems to be in progress, wait with timeout for completion
            if (error == WSAEWOULDBLOCK || error == WSAEINPROGRESS)
            {
                fd_set writeSet;
                FD_ZERO(&writeSet);
                FD_SET(clientSocket, &writeSet);

                //Getting the result
                int select_result = select(0, nullptr, &writeSet, nullptr, &timeout);

                if(select_result == SOCKET_ERROR) //Socket error, something is completely wrong
                {
                    return -1;
                }
                else if(select_result == 0) //Timeout; connection could not be established
                {
                    return 0;
                }
                else //Connection could be established;
                {
                    return 1;
                }
            }
            else //Something else went wrong
            {
                return -1;
            }
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

    int CommsModule::poll_message()
    {
        //Initializing for using the select statement
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(clientSocket, &readSet);

        //Executing the select statement and storing the result
        int result = select(0, &readSet, nullptr, nullptr, &timeout);

        //Processing the result
        if(result == SOCKET_ERROR) //Socket error detected.
        {
            std::cerr << "Socket error\n";
            return -1;
        }
        else if (result > 0) //Data seems to be available
        {
            //Preparing a buffer
            char buff[1024];
            int bytesRead;

            //Storing the data in the buffer
            bytesRead = recv(clientSocket, buff, sizeof(buff), 0);

            //Checking and processing the received data
            if (bytesRead > 0) //Data is available
            {
                buff[bytesRead] = '\0';
                std::cout << "Received: " << buff << "\n";
            }
            else if (bytesRead == 0)
            {
                std::cerr << "Received empty data\n";
                return 0;
            }
            else
            {
                int errorcode = WSAGetLastError();
                std::cerr << "Produced error " << errorcode << "!\n";
                return -1;
            }
        }
        return 1;
    }

} // Modules