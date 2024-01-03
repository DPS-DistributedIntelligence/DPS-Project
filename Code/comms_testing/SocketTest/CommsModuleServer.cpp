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
            std::cerr << "Socket error in select for accepting: " << WSAGetLastError() << "\n";
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
                    std::cout << "Connection accepted from client\n";
                    //clientSockets.push_back(clientSocket);
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
            else if (bytesRead == 0) //Client sent empty data, return 0;
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
        //Initializing an iterator for iterating over the vector of sockets
        //auto curr_sock = clientSockets.begin();
        auto curr_client = clientID_socket_map.begin();
        while(curr_client != clientID_socket_map.end())
        {
            string rx_message;
            if(getMessageFromSocket((curr_client->clientSocket), rx_message) == 1)
            {
                cout << rx_message << "\n";
            }

            curr_client++;
        }
    }

    int CommsModuleServer::relayMessages()
    {
        //Initializing an iterator for iterating over the vector of sockets
        auto curr_sock = clientSockets.begin();
        while (curr_sock != clientSockets.end())
        {
            //Initializing for using the select statement
            fd_set readSet;
            FD_ZERO(&readSet);
            FD_SET(*curr_sock, &readSet);

            //Executing the select statement and storing the result
            int result = select(0, &readSet, nullptr, nullptr, &timeout);

            //Processing the result
            if(result == SOCKET_ERROR) //Socket error detected.
            {
                std::cerr << "Socket error caused by client " << curr_sock - clientSockets.begin() << "\n";

                //Closing and deleting socket, then continuing
                closesocket(*curr_sock);
                curr_sock = clientSockets.erase(curr_sock);
                continue;
            }
            else if (result > 0) //Data seems to be available
            {
                //Preparing a buffer
                char buff[1024];
                int bytesRead;

                //Storing the data in the buffer
                bytesRead = recv(*curr_sock, buff, sizeof(buff), 0);

                //Checking and processing the received data
                if (bytesRead > 0) //Data is available
                {
                    buff[bytesRead] = '\0';
                    std::cout << "Client " << curr_sock - clientSockets.begin() << " says: " << buff << "\n";

                    if(clientSockets.size() >= 2)
                    {
                        string message(buff);

                        //Getting the length in bytes
                        int bytes_length = (int)strlen(message.c_str());

                        SOCKET dest_sock;
                        //Sending and comparing result (if result != bytes_length --> error)
                        if(curr_sock != clientSockets.end())
                        {
                            dest_sock = *(curr_sock + 1);
                        }
                        else
                        {
                            dest_sock = clientSockets.front();
                        }

                        if(send(dest_sock, message.c_str(), bytes_length, 0) == bytes_length)
                        {
                            std::cout << "Successfully forwarded message \"" << message <<"\" from client " << curr_sock - clientSockets.begin() << "\n";
                        }
                    }

                }
                else if (bytesRead == 0)
                {
                    std::cerr << "Client " << curr_sock - clientSockets.begin() << " sent empty data\n";
                    continue;
                }
                else
                {
                    int errorcode = WSAGetLastError();
                    std::cerr << "Client " << curr_sock - clientSockets.begin() << " produced error " << errorcode << "!";

                    if(errorcode == 10054)
                    {
                        std::cerr << " Socket gets closed down and removed!\n";
                        closesocket(*curr_sock);
                        curr_sock = clientSockets.erase(curr_sock);
                        continue;
                    }

                    std::cerr << "\n";
                }
            }
            curr_sock++;
        }
        return 1;
    }

    int CommsModuleServer::getNumOfConnectedClients()
    {
        return (int)clientID_socket_map.size();
    }

} // Module