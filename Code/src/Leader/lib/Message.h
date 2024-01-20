//
// Created by leand on 16.01.2024.
//

#ifndef SOCKETTEST_MESSAGE_H
#define SOCKETTEST_MESSAGE_H

#pragma once

#include <string>
#include <cstdint>
#include <vector>

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
    event getEvent();

    void setEvent(event event);

public:
        Message();

        // Getters and setters for each attribute
        int getReceiverId();
        void setReceiverId(int id);

        int getSenderId();
        void setSenderId(int id);

        uint64_t getLogicalClock();
        void setLogicalClock(uint64_t clock);

        uint8_t getControllerSerialNumber();
        void setControllerSerialNumber(uint8_t serialNumber);

        truckRole getRole();
        void setRole(truckRole role);

        int getSpeed();
        void setSpeed(int varSpeed);

        movementDirection getDirection();
        void setDirection(movementDirection varDirection);
};


class MessageID {
private:
    std::vector<int> receiver_ids;

public:
    MessageID() = default;
    void addReceiverId(int id);
    const std::vector<int>& getReceiverIds();
};

#endif //SOCKETTEST_MESSAGE_H
