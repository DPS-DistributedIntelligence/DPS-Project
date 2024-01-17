#ifndef TRUCKMETADATA_H
#define TRUCKMETADATA_H

#pragma once

#include <vector>
#include "TruckRole.h"
#include "Message.h"
#include "SurroundingTruck.h"
#include "location.h"

using namespace std;
typedef struct TruckMetadata
{
    bool have_ignition_key = false;
    bool have_communication = false;
    bool have_logical_clock = false;
    bool have_anti_collision_system = false;
    // message


    int truck_id = -1;
    truckRole role = NOT_SET;
    int logical_clock = -1;
    int truck_leader_id = -1;
    event event_handler = ev_any;
    Location truck_location = {0,0};



    // Message need to be sent
    vector<Message> pending_send_message;

    // Message received
    vector<Message> received_message;

    // for movement (follower)
    vector<movement> movement_leader;

    // only when the role is not set this vector is used
    vector<SurroundingTruck> surrounding_truck;



}TruckMetadata;

#endif
