//
// Created by leand on 25.12.2023.
//
#include "CommsModule.h"
#include <iostream>

using namespace Modules;

CommsModule cm = CommsModule(1000);

int main() {
    if(cm.initialize("127.0.0.1", 8080) != 1)
    {
        std::cout << "Initialization failed\n";
        return -1;
    }
    else
    {
        std::cout << "Initialization successful\n";
    }

    if(cm.connect_to_Server() != 1)
    {
        std::cout << "Connecting failed\n";
        return -1;
    }
    else
    {
        std::cout << "Connecting successful\n";
    }

    while(true)
    {
        if(cm.send_message("Test!") != 1)
        {
            std::cout << "Sending failed\n";
            return -1;
        }
        else
        {
            std::cout << "Sending successful\n";
        }

        cm.poll_message();

        Sleep(1000);
    }


    system("pause");
    return 0;
}