//
// Created by leand on 27.12.2023.
//

#ifndef SOCKETTEST_COMMSMODULESERVER_H
#define SOCKETTEST_COMMSMODULESERVER_H

#pragma once
#include <winsock.h>
#include <string>
#include <vector>
#include <iostream>

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

    public:
        CommsModuleServer(long timeout);
        int initialize(u_short port);
        int waitForConnections(u_int numConnections);
        int waitForMessagesAndPrint();
        int checkAndAcceptConnection();
        int relayMessages();
    };

} // Module

#endif //SOCKETTEST_COMMSMODULESERVER_H
