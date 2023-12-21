// server to which bind clients and write/read messages
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <unistd.h>

#define NUM_CONNECTION 3
#define PORT 9909
#define NUM_OF_TRUCKS 4

// only this is possible for windows :-(
// socket.h for linux - even cygwin don't have this lib

// adress[ip], port, family and where to listen (have to init) 
struct sockaddr_in srv;
int communication_socket;
int opt = 1;    
int address_len = sizeof(srv);


pthread_mutex_t mutexNumberOfVisitor = PTHREAD_MUTEX_INITIALIZER;


// connections from trucks
void* connection(void* threadid)
{

    int truck_socket;
    char buffer[1024] = { 0 };
    int  valread;
    if ((truck_socket = accept(communication_socket, NULL, NULL) < 0))
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }


    while (true) // here will be functionality of read/write message I think
    {
        valread = read(truck_socket, buffer, 1024);
        
        // here...

    }
}


int main() 
{
    // init the WSA variable, to be able use socket
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
    {
        printf("\n winsock not initialized \n");
    } else {
        printf("\n winsock initialized \n");
    }

    // init a socket
    int communication_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (setsockopt(communication_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // IPv4
    srv.sin_family = AF_INET;
    
    // htons convert 16bit from host byte order to network byte order 
    srv.sin_port = htons(PORT);
    
    srv.sin_addr.s_addr = INADDR_ANY;


    // associate a socket with a specific local address 
    if (bind(communication_socket, (struct sockaddr*)&srv, sizeof(srv)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    // waiting for connections (socket passive)
    if (listen(communication_socket, NUM_CONNECTION) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[NUM_OF_TRUCKS];
    {   
        for (int i = 0; i < NUM_OF_TRUCKS; ++i){
            pthread_create(&threads[i], NULL, connection, NULL);
        } 
    }
    pthread_exit(NULL);

}
