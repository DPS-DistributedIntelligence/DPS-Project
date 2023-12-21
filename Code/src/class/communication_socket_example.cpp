// server to which bind clients and write/read messages
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <winsock.h>


using namespace std;

#define NUM_CONNECTION 4
#define PORT 9909
#define NUM_OF_TRUCKS 4

// only this is possible for windows :-(
// socket.h for linux - even cygwin don't have this lib

// adress[ip], port, family and where to listen (have to init) 
struct sockaddr_in srv;
int communication_socket;
int opt = 1;    
int address_len = sizeof(srv);


pthread_mutex_t mutexMessage = PTHREAD_MUTEX_INITIALIZER;


void writeMessage(string message){
    pthread_mutex_lock(&mutexMessage);
    {
        // message = text 
        // ...
    }
    pthread_mutex_unlock(&mutexMessage);
}

// connections from trucks
void* connection(void* threadid)
{

    int truck_socket;
    char buffer[1024] = { 0 };
    int  valread;
    if ((truck_socket = accept(communication_socket, NULL, NULL) < 0))
    {
        perror("accept");
        WSACleanup();
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
        WSACleanup();
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
        WSACleanup();
        exit(EXIT_FAILURE);
    }


    // waiting for connections (socket passive)
    if (listen(communication_socket, NUM_CONNECTION) < 0)
    {
        perror("listen");
        WSACleanup();
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


/*
Error occurs - problem with libraries including (library A has to be before B - example)


C:/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\topor\AppData\Local\Temp\cc7yf62V.o:communication_socket_example.cpp:(.text+0x50): undefined reference to `__imp_accept'
C:/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\topor\AppData\Local\Temp\cc7yf62V.o:communication_socket_example.cpp:(.text+0xce): undefined reference to `__imp_WSAStartup'
C:/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\topor\AppData\Local\Temp\cc7yf62V.o:communication_socket_example.cpp:(.text+0x10e): undefined reference to `__imp_socket'
C:/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\topor\AppData\Local\Temp\cc7yf62V.o:communication_socket_example.cpp:(.text+0x142): undefined reference to `__imp_setsockopt'
C:/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\topor\AppData\Local\Temp\cc7yf62V.o:communication_socket_example.cpp:(.text+0x17b): undefined reference to `__imp_htons'
C:/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\topor\AppData\Local\Temp\cc7yf62V.o:communication_socket_example.cpp:(.text+0x1ad): undefined reference to `__imp_bind'
C:/msys64/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: C:\Users\topor\AppData\Local\Temp\cc7yf62V.o:communication_socket_example.cpp:(.text+0x1e6): undefined reference to `__imp_listen'


*/

