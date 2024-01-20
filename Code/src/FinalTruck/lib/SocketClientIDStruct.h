#ifndef SOCKETCLIENTIDSTRUCT_H
#define SOCKETCLIENTIDSTRUCT_H

#pragma once
#include <winsock2.h>

struct SocketClientID
{
    SOCKET clientSocket;
    int ID;
};

#endif