// making instances of truck
#include <iostream>
#include <unistd.h>
#include <winsock.h>
#include <string>
using namespace std;
 
// https://www.youtube.com/watch?v=CMDBF84vSRk
// https://www.youtube.com/watch?v=LtXEMwSG5-8

// playlist below of sockets on C++
// https://youtu.be/W9b9SaGXIjA?si=yeHrn8aF6ysLnK-X


#define PORT 9909

// to connect to communication socket

class truckComm{
    private:
        int truck_socket;
        struct sockaddr_in communcation_address;
    public:

        // constructor
        truckComm() {
            WSADATA wsaData;
            // init wsa to use socket
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                cerr << "Failed to initialize Winsock" << endl;
                exit(EXIT_FAILURE);
            }
            

            // init socket
            if ((truck_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
                perror("socket failed");
                WSACleanup();
                exit(EXIT_FAILURE);
            }
            communcation_address.sin_family = AF_INET;
            communcation_address.sin_port = htons(PORT);

            // Connect to the server
            if (connect(truck_socket, (struct sockaddr*)&communcation_address, sizeof(communcation_address)) < 0) {
                perror("connection failed");
                WSACleanup();
                exit(EXIT_FAILURE);
            }

        }

        // destructor
        ~truckComm() {
            closesocket(truck_socket);
            WSACleanup();
        }

        void sendMessage(const string& message) {
            // writing functionality should be here?
            send(truck_socket, message.c_str(), message.length(), 0);
        }

        void receiveMessages() {
            // Method to continuously receive messages from the server
            char buffer[1024] = {0};
            int valread;

            while (true) {

                // reading functionality should be here?

                valread = recv(truck_socket, buffer, sizeof(buffer), 0);
                if (valread > 0) {
                    // Process and handle the received message
                    cout << "Truck received: " << buffer << std::endl;
                }
            }
        }
};

int main() {
    truckComm truck1;
    truckComm truck2;
   
}


/*
same error with libraries import.


*/
