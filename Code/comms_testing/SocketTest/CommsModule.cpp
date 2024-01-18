//
// Created by leand on 25.12.2023.
//

#include "CommsModule.h"

namespace Modules {
    CommsModule::CommsModule(int id, long timeout_us)
    {
        ID = id;

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
            }
            else //Something else went wrong
            {
                return -1;
            }
        }

        /*
        //Sending signup packet
        if(send_signup_packet() != 1)
        {
            return -1;
        }
        */

        return 1;
    }

    //Internal function used to send the actual string
    int CommsModule::send_string(const std::string &message_str)
    {
        //Getting the length in bytes
        int bytes_length = (int)strlen(message_str.c_str());

        //Sending and comparing result (if result != bytes_length --> error)
        if(send(clientSocket, message_str.c_str(), bytes_length, 0) == bytes_length)
        {
            return 1;
        }
        return -1;
    }

    //Internal function used to receive the actual string
    int CommsModule::receive_string(std::string &rx_message)
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
                //Adding string terminator
                buff[bytesRead] = '\0';

                //Converting into real string
                rx_message.append(buff, bytesRead);

                return 1;
            }
            else if (bytesRead == 0)
            {
                return 0;
            }
            else
            {
                return -1;
            }
        }

        return 0;
    }

    int CommsModule::send_signup_packet()
    {
        string packet_content = "signup-ID:" + std::to_string(ID) + "\n";
        return send_string(packet_content);
    }


    //Internal function used to send a message
    int CommsModule::send_message(Message message)
    {
        //Parsing to JSON string and sending
        string message_str = MessageParser::toJSON(message);
        message_str += "\n";
        return send_string(message_str);
    }

    //Function to send the whole tx buffer
    int CommsModule::send_txBuffer()
    {
        int result = 1;

        //Iterating over vector
        auto tx_message = tx_messages.begin();
        while(tx_message != tx_messages.end())
        {
            //If sending was successful
            if(send_message(*tx_message) == 1)
            {
                //Removing message from buffer and continue
                tx_message = tx_messages.erase(tx_message);
                continue;
            }

            //Changing result because some error occured and incrementing iterator
            result = -1;
            tx_message++;
        }

        return result;
    }

    //Function to receive all waiting messages and adding them to the rx buffer
    int CommsModule::receive_rxBuffer()
    {
        while(true)
        {
            //Preparing a string the message could be stored to
            string rx_message;

            int receiving_result = receive_string(rx_message);

            //Checking the result
            if(receiving_result == 0) //No messages
            {
                return 1;
            }
            else if(receiving_result == -1) //Error occured
            {
                return -1;
            }

            //Creating a string stream
            istringstream rx_message_stream(rx_message);

            //Dissecting messages
            string rx_submessage;
            while(getline(rx_message_stream, rx_submessage, '\n'))
            {
                //Parsing message from JSON to Message
                Message parsed_message = MessageParser::fromJSON(rx_submessage);

                //Adding parsed message to buffer
                rx_messages.push_back(parsed_message);
            }
        }

        return 1;
    }

    int CommsModule::add_tx_message_to_buffer(Message tx_message)
    {
        //Adding message to tx buffer
        tx_messages.push_back(tx_message);

        return 1;
    }

    int CommsModule::print_rx_messages_from_buffer()
    {
        if(rx_messages.empty())
        {
            return 0;
        }

        auto rx_message = rx_messages.begin();
        while(rx_message != rx_messages.end())
        {
            cout << "--------------------------------------------------------" << endl;
            cout << "Receiver ID: " << rx_message->getReceiverId() << endl;
            cout << "Sender ID: " << rx_message->getSenderId() << endl;
            cout << "Logical Clock: " << rx_message->getLogicalClock() << endl;
            cout << "Controller Serial Number: " << static_cast<int>(rx_message->getControllerSerialNumber()) << endl;
            cout << "Role: " << MessageParser::truckRoleToString(rx_message->getRole()) << endl;
            cout << "Speed: " << rx_message->getSpeed() << endl;
            cout << "--------------------------------------------------------" << endl;

            rx_message = rx_messages.erase(rx_message);
        }

        return 1;
    }
} // Modules