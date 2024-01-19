
#ifndef DECRYPTOR_H
#define DECRYPTOR_H

#include "../../lib/TruckMetadata.h"

class Decryptor{
    TruckMetadata* self_truck;

public:
    Decryptor(TruckMetadata*);
    Decryptor();
    void run();
    static void* run(void* context);
    void* run_thread();
};

#endif
