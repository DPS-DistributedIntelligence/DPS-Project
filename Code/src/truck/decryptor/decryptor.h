
#ifndef DECRYPTOR_H
#define DECRYPTOR_H

#include "../../lib/TruckMetadata.h"

class Decryptor{
    TruckMetadata* self_truck;

public:
    Decryptor(TruckMetadata*);
    void run();
};

#endif
