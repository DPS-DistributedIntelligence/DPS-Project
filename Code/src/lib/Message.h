
#ifndef MESSAGE_H
#define MESSAGE_H

#pragma once

#include <string>
#include <cstdint>

#include "TruckRole.h"
#include "Movement.h"
#include "Event.h"
#include "Location.h"

class Message {
private:
    int receiver_id;
    int sender_id;
    uint64_t logicalClock_u64;
    uint8_t controllerSerialNumber_u8;
    truckRole role_e;
    int speed;
    movementDirection direction;
    event event_;
    Location location_;


public:
    Message();

    // Getters and setters for each attribute
    int getReceiverId() const;
    void setReceiverId(int id);

    int getSenderId() const;
    void setSenderId(int id);

    uint64_t getLogicalClock() const;
    void setLogicalClock(uint64_t clock);

    uint8_t getControllerSerialNumber() const;
    void setControllerSerialNumber(uint8_t serialNumber);

    truckRole getRole() const;
    void setRole(truckRole role);

    int getSpeed() const;
    void setSpeed(int speed);

    event getEvent() const;
    void setEvent(event);

    movementDirection getDirection() const;
    void setDirection(movementDirection direction);

    Location getLocation() const;
    void setLocation(Location);
};

#endif
