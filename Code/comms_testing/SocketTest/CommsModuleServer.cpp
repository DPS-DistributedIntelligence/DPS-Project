//
// Created by leand on 27.12.2023.
//

#include "CommsModuleServer.h"

namespace Modules {
    CommsModuleServer::CommsModuleServer(long timeout_us)
    {
        //Initializing the timeout
        timeout.tv_sec = 0;
        timeout.tv_usec = timeout_us;
    }

    int CommsModuleServer::initialize(u_short port)
    {
        //Initializing the winsock.dll
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
            return -1;
        }

        // Create a socket for the server to listen for incoming connections
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            WSACleanup();
            return -1;
        }

        //Set the socket to non-blocking mode
        u_long mode = 1;
        if (ioctlsocket(serverSocket, FIONBIO, &mode) != 0) {
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }

        // Set up the server address structure
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port); // You can use any port you like

        // Bind the socket
        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }

        // Listen for incoming connections
        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }

        return 1;
    }


    int CommsModuleServer::checkAndAcceptConnection()
    {
        // Set up the fd_set for select
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(serverSocket, &readSet);

        //Using select to later check if there is an incoming connection and storing the result
        int result = select(0, &readSet, nullptr, nullptr, &timeout);

        //Processing the result
        if (result == SOCKET_ERROR)
        {
            return -1;
        }
        else if (result > 0)
        {
            //Checking if there is an incoming connection
            if (FD_ISSET(serverSocket, &readSet))
            {
                SocketClientID incomingClient{};

                //Trying to accept the new incoming connection
                //SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
                incomingClient.clientSocket = accept(serverSocket, nullptr, nullptr);

                //If the new socket was successfully accepted, add it to the vector and returning 1
                if (incomingClient.clientSocket != INVALID_SOCKET)
                {
                    clientID_socket_map.push_back(incomingClient);
                    return 1;
                }
                else //Close socket if something went wrong
                {
                    closesocket(incomingClient.clientSocket);
                    return -1;
                }
            }
        }

        //Returning 0 if no new connection was accepted
        return 0;
    }

    int CommsModuleServer::getMessageFromSocket(SOCKET& rxSocket, string& rx_message)
    {
        //Initializing for using the select statement
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(rxSocket, &readSet);

        //Executing the select statement and storing the result
        int result = select(0, &readSet, nullptr, nullptr, &timeout);

        //Processing the result
        if(result == SOCKET_ERROR) //Socket error detected --> return -1
        {
            return -1;
        }
        else if (result > 0) //Data seems to be available
        {
            //Preparing a buffer
            char buff[1024];
            int bytesRead;

            //Storing the data in the buffer
            bytesRead = recv(rxSocket, buff, sizeof(buff), 0);

            //Checking and processing the received data
            if (bytesRead > 0) //Data is available
            {
                //Store the buffer in the string
                rx_message.append(buff, bytesRead);
                return 1;
            }
            else if (bytesRead == 0) //Client sent empty data --> return 0
            {
                return 0;
            }
            else //Something went wrong --> return -1
            {
                return -1;
            }
        }
        else //No data received --> return 0
        {
            return 0;
        }
    }

    void CommsModuleServer::getMessagesFromAllSockets()
    {
        int result;

        //Initializing an iterator for iterating over the vector of sockets
        auto curr_client = clientID_socket_map.begin();
        while(curr_client != clientID_socket_map.end())
        {
            while(true) {
                string rx_message;

                //Getting the message from the socket and storing the result
                result = getMessageFromSocket((curr_client->clientSocket), rx_message);

                //Break if no message with content was received (result 0 or -1)
                if (result != 1) {
                    break;
                }

                //If current client has no id (==0) and the packet starts with "signup-ID" (--> signup packet was detected)
                if (curr_client->ID == 0 && rx_message.compare(0, 10, "signup-ID:") == 0) {
                    //Extract the ID and store it in the current client
                    string extracted_id = rx_message.substr(10);
                    curr_client->ID = stoi(extracted_id);

                    cout << "Signup detected with ID: " << curr_client->ID << "\n";
                } else //Otherwise it is a normal packet
                {
                    cout << "[RX] from client " << curr_client->ID << ": \"" << rx_message << "\"\n";

                    //TODO: Store it in the vector of RX packets
                }
            }
            curr_client++;
        }
    }


    int CommsModuleServer::getNumOfConnectedClients()
    {
        return (int)clientID_socket_map.size();
    }

} // Module