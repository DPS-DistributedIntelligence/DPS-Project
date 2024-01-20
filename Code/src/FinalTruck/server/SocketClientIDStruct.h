//
// Created by leand on 03.01.2024.
//

#ifndef SOCKETTEST_SOCKETCLIENTIDSTRUCT_H
#define SOCKETTEST_SOCKETCLIENTIDSTRUCT_H

#pragma once
#include <winsock2.h>

struct SocketClientID
{
    SOCKET clientSocket;
    int ID;
};

#endif //SOCKETTEST_SOCKETCLIENTIDSTRUCT_H
