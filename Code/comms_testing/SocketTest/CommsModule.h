//
// Created by leand on 25.12.2023.
//

#ifndef SOCKETTEST_COMMSMODULE_H
#define SOCKETTEST_COMMSMODULE_H

#pragma once
#include <winsock2.h>
#include <string>
#include <iostream>

//Linker should add "ws2_32.lib" to the list of libraries to be linked with the program
#pragma comment(lib, "ws2_32.lib")

using namespace std;

namespace Modules {

    class CommsModule {
    private:
        sockaddr_in serverAddr{};
        SOCKET clientSocket{};
        timeval timeout{};

    public:
        CommsModule(long timeout);
        int initialize(const string& ip_address, u_short port);
        int connect_to_Server();
        int send_message(const string& message);
        int poll_message();
    };
} // Modules

#endif //SOCKETTEST_COMMSMODULE_H
