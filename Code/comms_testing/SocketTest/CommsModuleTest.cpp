//
// Created by leand on 25.12.2023.
//
#include "CommsModule.h"
#include <iostream>
#include <random>
#include <vector>

using namespace Modules;

CommsModule cm = CommsModule(1000);

int main() {
    //Generate random ID between 0 and 999
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 999);

    int ID = 0;

    std::cout << "Enter ID: ";
    std::cin >> ID;
    std::cout << "Input ID: " << ID << "\n";

    //std::cout << "Generated random ID: " << ID << "\n";
    std::cout << "Proceeding with setting up socket and connecting to server...\n";

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

    std::cout << "Sending signup packet with ID: " << ID << "\n";
    while (cm.send_message("signup-ID:" + std::to_string(ID)) != 1)
    {
        std::cerr << "Sending signup packet failed!\n";
    }
    std::cout << "Signup packet was sent successfully\n";

    vector<int> destIDs {100, 200, 300};

    while(true)
    {
        auto dest_it = destIDs.begin();
        while(dest_it != destIDs.end())
        {
            if(*dest_it != ID)
            {
                string message;
                message += "dest-ID:" + std::to_string(*dest_it);
                message += " ";
                message += "Message with random int: " + std::to_string(distribution(generator)) + "\n";

                if(cm.send_message(message) != 1)
                {
                    std::cout << "Sending failed\n";
                    return -1;
                }
                else {
                    std::cout << "Sending successful\n";
                }
            }

            dest_it++;
        }

        cm.poll_message();

        Sleep(1000);
    }


    system("pause");
    return 0;
}