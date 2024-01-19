//
// Created by leand on 25.12.2023.
//

#ifndef SOCKETTEST_COMMSMODULE_H
#define SOCKETTEST_COMMSMODULE_H

#pragma once
#include <winsock.h>
#include <winsock2.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <mutex>
#include <pthread.h>
#include <optional>

#include "../lib/Message.h"
#include "../lib/MessageParser.h"

#include "../lib/TruckMetadata.h"

//Linker should add "ws2_32.lib" to the list of libraries to be linked with the program
#pragma comment(lib, "ws2_32.lib")

//using namespace std;

//namespace Modules {

    class CommsModule {
    private:
        int ID;
        pthread_mutex_t client_IDs_vec_mutex;
        sockaddr_in serverAddr{};
        SOCKET clientSocket{};
        timeval timeout{};
        std::vector<Message> tx_messages;
        std::vector<Message> rx_messages;
        pthread_mutex_t tx_vec_mutex;
        pthread_mutex_t rx_vec_mutex;
        int send_string(const std::string& message_str);
        int receive_string(std::string& rx_message);
        int send_message(Message message);

    public:
        std::vector<int> client_IDs;
        TruckMetadata* self_truck;

        static void* run(void* context);
        void* run_thread();

        CommsModule(int id, long timeout, TruckMetadata* self_truck);

        CommsModule();
        int initialize(const std::string& ip_address, u_short port);
        int connect_to_Server();
        int send_txBuffer();
        int receive_rxBuffer();
        int add_tx_message_to_buffer(Message tx_message);
        std::optional<Message> get_last_rx_message_from_buffer(bool del);
        std::optional<Message> get_rx_message_by_index_from_buffer(int index, bool del);
        int get_length_of_rx_buffer();
        std::vector<int> get_connected_client_IDs();
        int print_rx_messages_from_buffer();


    };
//} // Modules

#endif
