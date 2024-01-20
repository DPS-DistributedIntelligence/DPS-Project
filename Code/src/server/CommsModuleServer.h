//
// Created by leand on 27.12.2023.
//

#ifndef SOCKETTEST_COMMSMODULESERVER_H
#define SOCKETTEST_COMMSMODULESERVER_H

#pragma once
#include <winsock2.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "SocketClientIDStruct.h"
#include "../lib/Message.h"
#include "../lib/MessageParser.h"

//Linker should add "ws2_32.lib" to the list of libraries to be linked with the program
#pragma comment(lib, "ws2_32.lib")

using namespace std;

namespace Modules {

    class CommsModuleServer {
    private:
        sockaddr_in serverAddr{};
        SOCKET serverSocket{};
        timeval timeout{};
        vector<Message> messageBuffer;
        vector<SocketClientID> clientID_socket_map;

    public:
        CommsModuleServer(long timeout_us);
        int initialize(u_short port);
        int checkAndAcceptConnection();
        int getMessageFromSocket(SOCKET& rxSocket, string& rx_message);
        int sendMessage(SOCKET& txSocket, string& tx_message);
        void getMessagesFromAllSockets();
        void forwardPackets();
        void sendClientIDVector();
        int getNumOfConnectedClients();
    };

} // Module

#endif //SOCKETTEST_COMMSMODULESERVER_H
