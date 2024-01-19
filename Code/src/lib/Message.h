//
// Created by leand on 16.01.2024.
//

#ifndef SOCKETTEST_MESSAGE_H
#define SOCKETTEST_MESSAGE_H

#pragma once

#include <string>
#include <cstdint>

#include "TruckRole.h"
#include "Movement.h"
#include "Event.h"

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
public:
    event getEvent() const;

    void setEvent(event event);

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

        int getSpeed();
        void setSpeed(int speed);

        movementDirection getDirection() const;
        void setDirection(movementDirection direction);
};


#endif //SOCKETTEST_MESSAGE_H
