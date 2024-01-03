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
#include "SocketClientIDStruct.h"

//Linker should add "ws2_32.lib" to the list of libraries to be linked with the program
#pragma comment(lib, "ws2_32.lib")

using namespace std;

namespace Modules {

    class CommsModuleServer {
    private:
        sockaddr_in serverAddr{};
        SOCKET serverSocket{};
        vector<SOCKET> clientSockets;
        timeval timeout{};
        vector<string> rxPackets;
        vector<string> txPackets;
        vector<SocketClientID> clientID_socket_map;

    public:
        CommsModuleServer(long timeout_us);
        int initialize(u_short port);
        int checkAndAcceptConnection();
        int checkAndReceiveMessages();
        int relayMessages();
        int getNumOfConnectedClients();
        int getMessageFromSocket(SOCKET& rxSocket, string& rx_message);
        void getMessagesFromAllSockets();
    };

} // Module

#endif //SOCKETTEST_COMMSMODULESERVER_H
