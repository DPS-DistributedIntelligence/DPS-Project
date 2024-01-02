//
// Created by leand on 27.12.2023.
//
#include "CommsModuleServer.h"
#include <iostream>

using namespace Modules;

CommsModuleServer server = CommsModuleServer(1000);

int main()
{
    if(server.initialize(8080) != 1)
    {
        std::cout << "Initialization failed\n";
        return -1;
    }
    else
    {
        std::cout << "Initialization successful\n";
    }

    std::cout << "Waiting for connection...\n";


    while(true)
    {
        server.checkAndAcceptConnection();
        server.relayMessages();
    }

    system("pause");

    return 0;
}