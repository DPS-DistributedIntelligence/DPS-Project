//
// Created by sheik on 12/17/2023.
//

#ifndef TRUCK_H
#define TRUCK_H

#include "lib/commonLib.h"

class communication;
class controller;
class Truck{
public:
    Location location{0,0};
    int truck_id = -1;
    int leader_id = -1 ;
    explicit Truck(int id, Location location);
    Truck();

private:

    //communication main_communication;
    //controller main_controller = new controller(&truck_id, &leader_id, &location, &main_communication);
};
#endif //TRUCK_H
