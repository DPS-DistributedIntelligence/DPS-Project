// not sure, but maybe it is possible to do 1 class of truck and make instances?
#include <iostream>
#include <unistd.h>
#include <winsock.h>
using namespace std;
 


#define PORT 9909


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
};

