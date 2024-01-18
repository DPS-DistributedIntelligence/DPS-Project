//
// Created by leand on 25.12.2023.
//
#include "CommsModule.h"
#include "Message.h"

#include <iostream>
#include <random>
#include <vector>

using namespace Modules;

int main() {
    //Setting up random generator
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 999);

    //Asking for an ID
    int ID = 0;
    std::cout << "Enter an ID! Should be 100, 200 or 300 for now" << endl;
    std::cout << "Enter ID: ";
    std::cin >> ID;
    std::cout << "Input ID: " << ID << "\n";

    CommsModule cm = CommsModule(ID, 1000);

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

    vector<int> destIDs {100, 200, 300};

    while(true)
    {
        //Iterating over the destination IDs
        auto dest_it = destIDs.begin();
        while(dest_it != destIDs.end())
        {
            //Only queue a packet if it is not my own ID
            if(*dest_it != ID)
            {
                //Making an example message with random logical clock content
                Message msg;
                msg.setReceiverId(*dest_it);
                msg.setSenderId(ID);
                msg.setLogicalClock(distribution(generator));
                msg.setControllerSerialNumber(255);
                msg.setRole(truckRole_e::LEADER);
                msg.setSpeed(75);
                msg.setDirection(MovementDirection::MOVE_FORWARD);

                //Adding it to the tx_buffer
                cm.add_tx_message_to_buffer(msg);
            }

            dest_it++;
        }

        //Sending the tx buffer
        cm.send_txBuffer();

        //Receiving the rx buffer
        cm.receive_rxBuffer();

        //Printing all the messages
        cm.print_rx_messages_from_buffer();

        //Sleeping for 1s
        Sleep(1000);
    }

    return 0;
}