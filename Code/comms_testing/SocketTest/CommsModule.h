//
// Created by leand on 25.12.2023.
//

#ifndef SOCKETTEST_COMMSMODULE_H
#define SOCKETTEST_COMMSMODULE_H

#pragma once
#include <winsock2.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "Message.h"
#include "MessageParser.h"

//Linker should add "ws2_32.lib" to the list of libraries to be linked with the program
#pragma comment(lib, "ws2_32.lib")

using namespace std;

namespace Modules {

    class CommsModule {
    private:
        int ID;
        sockaddr_in serverAddr{};
        SOCKET clientSocket{};
        timeval timeout{};
        vector<Message> tx_messages;
        vector<Message> rx_messages;
        int send_string(const string& message_str);
        int receive_string(string& rx_message);
        int send_message(Message message);
        int send_signup_packet();

    public:
        CommsModule(int id, long timeout);
        int initialize(const string& ip_address, u_short port);
        int connect_to_Server();
        int add_tx_message_to_buffer(Message tx_message);
        int print_rx_messages_from_buffer();
        int send_txBuffer();
        int receive_rxBuffer();
    };
} // Modules

#endif //SOCKETTEST_COMMSMODULE_H
