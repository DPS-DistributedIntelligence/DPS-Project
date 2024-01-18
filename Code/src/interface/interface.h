#ifndef CODE_INTERFACE_H
#define CODE_INTERFACE_H


#include "../controller/controller.h"
#include "../lib/TruckRole.h"

using namespace std;

class interface{
public:
    interface(controller new_con);
    controller* self_controller;
    const int cruiseDriverStep = 10;
    const int cruiseDriverLimit = 210;
    static void *run(void *context);
    void *run_thread();
private:
    //pthread_t thread_interfaceID;
};

#endif
