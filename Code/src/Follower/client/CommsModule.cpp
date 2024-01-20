//
// Created by leand on 25.12.2023.
//

#include "CommsModule.h"

//namespace Modules {
    CommsModule::CommsModule(int id, long timeout_us,TruckMetadata* new_self_truck)
    {
        pthread_mutex_init(&rx_vec_mutex, nullptr);
        pthread_mutex_init(&tx_vec_mutex, nullptr);
        pthread_mutex_init(&client_IDs_vec_mutex, nullptr);

        ID = id;
        //Initializing the timeout
        timeout.tv_sec = 0;
        timeout.tv_usec = timeout_us;

        self_truck = new_self_truck;
        self_truck->truck_id = ID;
        self_truck->surrounding_truck_IDs = &client_IDs;
        self_truck->client_IDs_vec_mutex_ = &client_IDs_vec_mutex;

    }

    int CommsModule::initialize(const std::string& ip_address, u_short port) {

        //Initializing the winsock.dll
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            return -1;
        }

        // Create a socket for the client
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            WSACleanup();
            return -1;
        }

        //Set the socket to non-blocking mode
        u_long mode = 1;
        if (ioctlsocket(clientSocket, FIONBIO, &mode) != 0) {
            closesocket(clientSocket);
            WSACleanup();
            return -1;
        }

        // Set up the server address structure
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ip_address.c_str()); // Server's IP address
        serverAddr.sin_port = htons(port); // Server's port

        return 1;
    }

    int CommsModule::connect_to_Server() {
        // Connect to the server
        int connection_result = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        //Result is error --> connection might be under initialization
        if (connection_result == SOCKET_ERROR) {

            //Getting the exact error
            int error = WSAGetLastError();

            //If connection seems to be in progress, wait with timeout for completion
            if (error == WSAEWOULDBLOCK || error == WSAEINPROGRESS)
            {
                fd_set writeSet;
                FD_ZERO(&writeSet);
                FD_SET(clientSocket, &writeSet);

                //Getting the result
                int select_result = select(0, nullptr, &writeSet, nullptr, &timeout);

                if(select_result == SOCKET_ERROR) //Socket error, something is completely wrong
                {
                    return -1;
                }
                else if(select_result == 0) //Timeout; connection could not be established
                {
                    return 0;
                }
            }
            else //Something else went wrong
            {
                return -1;
            }
        }

        return 1;
    }

    //Internal function used to send the actual string
    int CommsModule::send_string(const std::string &message_str)
    {
        //Getting the length in bytes
        int bytes_length = (int)strlen(message_str.c_str());

        //Sending and comparing result (if result != bytes_length --> error)
        if(send(clientSocket, message_str.c_str(), bytes_length, 0) == bytes_length)
        {
            return 1;
        }
        return -1;
    }

    //Internal function used to receive the actual string
    int CommsModule::receive_string(std::string &rx_message)
    {
        //Initializing for using the select statement
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(clientSocket, &readSet);

        //Executing the select statement and storing the result
        int result = select(0, &readSet, nullptr, nullptr, &timeout);

        //Processing the result
        if(result == SOCKET_ERROR) //Socket error detected.
        {
            return -1;
        }
        else if (result > 0) //Data seems to be available
        {
            //Preparing a buffer
            char buff[1024];
            int bytesRead;

            //Storing the data in the buffer
            bytesRead = recv(clientSocket, buff, sizeof(buff), 0);

            //Checking and processing the received data
            if (bytesRead > 0) //Data is available
            {
                //Adding string terminator
                buff[bytesRead] = '\0';

                //Converting into real string
                rx_message.append(buff, bytesRead);

                return 1;
            }
            else if (bytesRead == 0)
            {
                return 0;
            }
            else
            {
                return -1;
            }
        }

        return 0;
    }

    //Internal function used to send a message
    int CommsModule::send_message(Message message)
    {
        //Setting the ID of the sender to own ID (just to make sure)
        message.setSenderId(ID);

        //Parsing to JSON string and sending
        std::string message_str = MessageParser::toJSON(message);
        message_str += "\n";
        return send_string(message_str);
    }

    //Function to send the whole tx buffer
    //return value 1: packets successfully transmitted
    //return value 0: no packets transmitted
    //return value -1: communication error
    int CommsModule::send_txBuffer()
    {
        //Locking mutex
        pthread_mutex_lock(&tx_vec_mutex);
        //tx_vec_mutex.lock();

        //Checking if there are messages to transmit; if not unlocking mutex and returning 0
        if(tx_messages.empty())
        {
            pthread_mutex_unlock(&tx_vec_mutex);
            //tx_vec_mutex.unlock();
            return 0;
        }

        int result = 1;

        //Iterating over vector
        auto tx_message = tx_messages.begin();
        while(tx_message != tx_messages.end())
        {
            //If sending was successful
            if(send_message(*tx_message) == 1)
            {
                //Removing message from buffer and continue
                tx_message = tx_messages.erase(tx_message);
                continue;
            }

            //Changing result because some error occured and incrementing iterator
            result = -1;
            tx_message++;
        }

        //Unlocking mutex
        pthread_mutex_unlock(&tx_vec_mutex);
        //tx_vec_mutex.unlock();

        return result;
    }

    //Function to receive all waiting messages and adding them to the rx buffer
    //return value 1: packets successfully received
    //return value 0: no packets received
    //return value -1: communication error
    int CommsModule::receive_rxBuffer()
    {
        int result = 1;

        //Locking the mutex
        pthread_mutex_lock(&rx_vec_mutex);
        //rx_vec_mutex.lock();

        while(true)
        {
            //Preparing a string the message could be stored to
            std::string rx_message;

            int receiving_result = receive_string(rx_message);

            //Checking the result
            if(receiving_result == 0) //No messages
            {
                break;
            }
            else if(receiving_result == -1) //Error occured
            {
                result = -1;
                break;
            }

            //Creating a string stream
            std::istringstream rx_message_stream(rx_message);

            //Dissecting messages
            std::string rx_submessage;
            while(getline(rx_message_stream, rx_submessage, '\n'))
            {

                std::variant<Message, MessageID> parsed_message = MessageParser::fromJSONVariant(rx_submessage);

                if(std::holds_alternative<Message>(parsed_message))
                {
                    Message decoded_message = std::get<Message>(parsed_message);
                    rx_messages.push_back(decoded_message);
                }
                else if(std::holds_alternative<MessageID>(parsed_message))
                {
                    std::cout << "[RX] Vector information: " << rx_submessage << std::endl;
                    MessageID decoded_message = std::get<MessageID>(parsed_message);
                }

            }
        }

        //Unlocking the mutex
        pthread_mutex_unlock(&rx_vec_mutex);
        //rx_vec_mutex.unlock();

        return result;
    }

    //Function to add a message to the tx buffer
    int CommsModule::add_tx_message_to_buffer(Message tx_message)
    {
        //Locking the mutex
        pthread_mutex_lock(&tx_vec_mutex);
        //tx_vec_mutex.lock();

        //Adding message to tx buffer
        tx_messages.push_back(tx_message);

        //Unlocking the mutex
        pthread_mutex_unlock(&tx_vec_mutex);
        //tx_vec_mutex.unlock();

        return 1;
    }

    //Function for getting the last message of the vector
    //Argument: bool if element should be deleted afterward
    std::optional<Message> CommsModule::get_last_rx_message_from_buffer(bool del)
    {
        //Locking mutex
        pthread_mutex_lock(&rx_vec_mutex);
        //rx_vec_mutex.lock();

        //If vector is empty, unlock mutex and return empty
        if(rx_messages.empty())
        {
            pthread_mutex_unlock(&rx_vec_mutex);
            //rx_vec_mutex.unlock();
            return{};
        }

        //Getting the last message of the buffer
        Message msg = rx_messages.back();

        //Removing the last element if wanted
        if(del)
        {
            rx_messages.pop_back();
        }

        //Unlocking the mutex
        pthread_mutex_unlock(&rx_vec_mutex);
        //rx_vec_mutex.unlock();

        return msg;
    }

    //Function for getting the message of a certain position from the vector
    //Argument: bool if element should be deleted afterwards
    std::optional<Message> CommsModule::get_rx_message_by_index_from_buffer(int index, bool del)
    {
        //Locking mutex
        pthread_mutex_lock(&rx_vec_mutex);
        //rx_vec_mutex.lock();

        //Unlocking and returning empty if out of bounds
        if(index < 0 || index > (rx_messages.size() - 1))
        {
            pthread_mutex_unlock(&rx_vec_mutex);
            //rx_vec_mutex.unlock();
            return{};
        }

        //Getting the message at the supplied pos
        Message msg = rx_messages[index];

        //Removing the element if wanted
        if(del)
        {
            rx_messages.erase(rx_messages.begin() + index);
        }

        //Unlocking the mutex
        pthread_mutex_unlock(&rx_vec_mutex);
        //rx_vec_mutex.unlock();

        return msg;
    }

    int CommsModule::get_length_of_rx_buffer()
    {
        //Locking mutex
        pthread_mutex_lock(&rx_vec_mutex);
        //rx_vec_mutex.lock();

        //Getting the length
        int length = (int)rx_messages.size();

        //Unlocking the mutex
        pthread_mutex_unlock(&rx_vec_mutex);
        //rx_vec_mutex.unlock();

        return length;
    }

    //Function to get a vector of all connected IDs
    std::vector<int> CommsModule::get_connected_client_IDs()
    {
        //Locking the mutex
        pthread_mutex_lock(&client_IDs_vec_mutex);
        //client_IDs_vec_mutex.lock();

        //Doing a deep copy
        std::vector<int> ret_client_IDs(client_IDs);

        //Unlocking the mutex
        pthread_mutex_unlock(&client_IDs_vec_mutex);
        //client_IDs_vec_mutex.unlock();

        return ret_client_IDs;
    }

    //Function to print all messages from the rx buffer
    int CommsModule::print_rx_messages_from_buffer()
    {
        //Locking mutex
        pthread_mutex_lock(&rx_vec_mutex);
        //rx_vec_mutex.lock();

        //Unlocking and returning 0 if there was no message
        if(rx_messages.empty())
        {
            pthread_mutex_unlock(&rx_vec_mutex);
            //rx_vec_mutex.unlock();
            return 0;
        }

        auto rx_message = rx_messages.begin();
        while(rx_message != rx_messages.end())
        {
            std::cout << "--------------------------------------------------------" << std::endl;
            std::cout << "Receiver ID: " << rx_message->getReceiverId() << std::endl;
            std::cout << "Sender ID: " << rx_message->getSenderId() << std::endl;
            std::cout << "Logical Clock: " << rx_message->getLogicalClock() << std::endl;
            std::cout << "Controller Serial Number: " << static_cast<int>(rx_message->getControllerSerialNumber()) << std::endl;
            std::cout << "Role: " << MessageParser::truckRoleToString(rx_message->getRole()) << std::endl;
            std::cout << "Speed: " << rx_message->getSpeed() << std::endl;
            std::cout << "Direction: " << MessageParser::directionToString(rx_message->getDirection()) << std::endl;
            std::cout << "--------------------------------------------------------" << std::endl;


            rx_message = rx_messages.erase(rx_message);
        }

        //Unlocking mutex
        pthread_mutex_unlock(&rx_vec_mutex);
        //rx_vec_mutex.unlock();

        return 1;
    }



void *CommsModule::run_thread() {
    {
        if(initialize("127.0.0.1", 8080) == 1)
        {
            std::cout << "intialization worked" << std::endl;
        }
        else
        {
            std::cout << "Initialization failed" << std::endl;

        }

        if(connect_to_Server() == 1)
        {
            std::cout << "Connected!" << std::endl;

            Message msg;
            msg.setReceiverId(self_truck->truck_id+1);
            msg.setLogicalClock(self_truck->truck_logical_clock.get_logicalClock());
            msg.setRole(self_truck->role);
            msg.setSpeed((uint8_t)(self_truck->truckMovement.speed));
            msg.setDirection(self_truck->truckMovement.direction);
            add_tx_message_to_buffer(msg);
            send_txBuffer();
        }
        else
        {
            std::cout << "Connection failed!" << std::endl;
        }

        while(true)
        {
            receive_rxBuffer();

            std::optional<Message> rx_msg;
            rx_msg = get_last_rx_message_from_buffer(false);

            /*
             * Update values of follower
             */
            self_truck->truckMovement.direction = rx_msg->getDirection();
            self_truck->truckMovement.speed = rx_msg->getSpeed();

            if(self_truck->truck_logical_clock.get_logicalClock() < rx_msg->getLogicalClock())
            {
                if(!self_truck->truck_logical_clock.logicalClockTickCompare(rx_msg->getLogicalClock()))
                {
                    self_truck->truck_logical_clock.logicalClockUpdate(rx_msg->getLogicalClock());
                    print_rx_messages_from_buffer();
                }
            }

            usleep(500000);
        }
    }
    return nullptr;
}

void *CommsModule::run(void *context) {
    return ((CommsModule *)context)->run_thread();
    return nullptr;
}
//} // Modules