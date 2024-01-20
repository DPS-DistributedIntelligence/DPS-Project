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

    int CommsModuleServer::sendMessage(SOCKET &txSocket, std::string& tx_message)
    {
        //Getting the length in bytes
        int bytes_length = (int)strlen(tx_message.c_str());

        //Sending and comparing result (if result != bytes_length --> error)
        if(send(txSocket, tx_message.c_str(), bytes_length, 0) == bytes_length)
        {
            return 1;
        }
        return -1;
    }

    void CommsModuleServer::getMessagesFromAllSockets()
    {
        //Initializing an iterator for iterating over the vector of sockets
        auto curr_client = clientID_socket_map.begin();
        while(curr_client != clientID_socket_map.end())
        {
            int result;
            string rx_message;

            //Getting the message from the socket and storing the result
            result = getMessageFromSocket((curr_client->clientSocket), rx_message);

            //Checking result
            if (result == -1) //Socket error occcured
            {
                //Close socket
                closesocket(curr_client->clientSocket);

                //Remove client from list and continuing
                curr_client = clientID_socket_map.erase(curr_client);
                continue;
            }
            else if(result == 0) //Empty message
            {
                curr_client++;
                continue;
            }

            //Separating all the messages
            istringstream rx_message_stream(rx_message);
            string rx_submessage;
            while(getline(rx_message_stream, rx_submessage, '\n'))
            {
                cout << "Received: " << rx_submessage << endl;

                //Parsing the message
                Message msg;
                msg = MessageParser::fromJSON(rx_submessage);

                //Checking if current socket has no associated ID
                if(curr_client->ID == 0)
                {
                    //Associating ID
                    curr_client->ID = msg.getSenderId();
                }

                //Adding the parsed message to the buffer
                messageBuffer.push_back(msg);
            }

            //Getting the next client
            if(curr_client != clientID_socket_map.end())
            {
                curr_client++;
            }
        }
    }

    void CommsModuleServer::forwardPackets()
    {
        //Initalizing an iterator to iterate over the messages
        auto curr_message = messageBuffer.begin();
        while(curr_message != messageBuffer.end())
        {
            //Getting the destination socket
            SOCKET *destSocket = nullptr;

            //Initializing an iterator to iterate over the sockets
            auto client = clientID_socket_map.begin();
            while(client != clientID_socket_map.end())
            {
                //Checking if IDs are matching
                if(client->ID == curr_message->getReceiverId())
                {
                    //Storing a pointer to the destination Socket if it was found
                    destSocket = &client->clientSocket;
                    break;
                }

                client++;
            }

            //If destination socket was not found, erase packet
            if(destSocket == nullptr)
            {
                curr_message = messageBuffer.erase(curr_message);
                continue;
            }

            //Parsing the message to JSON and adding the separator
            string message_to_tx = MessageParser::toJSON(*curr_message);
            message_to_tx += "\n";

            //Sending the message and removing it if it was successfully sent
            if(sendMessage(*destSocket, message_to_tx) == 1)
            {
                curr_message = messageBuffer.erase(curr_message);
                continue;
            }

            curr_message++;
        }
    }

    void CommsModuleServer::sendClientIDVector()
    {
        MessageID msg;
        auto client = clientID_socket_map.begin();
        while(client != clientID_socket_map.end())
        {
            if(client->ID == 0)
            {
                client++;
                continue;
            }

            msg.addReceiverId(client->ID);
            client++;
        }

        std::string msg_str = MessageParser::toJSONID(msg);
        msg_str += "\n";

        client = clientID_socket_map.begin();
        while(client != clientID_socket_map.end())
        {
            sendMessage(client->clientSocket, msg_str);
            client++;
        }
    }

    //Public function that returns the number of the connected clients
    int CommsModuleServer::getNumOfConnectedClients()
    {
        return (int)clientID_socket_map.size();
    }

} // Module