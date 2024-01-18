//
// Created by Mykyta on 18.01.2024.
//

#ifndef SOCKETTEST_MESSAGE_H
#define SOCKETTEST_MESSAGE_H

#pragma once

#include <string>
#include <cstdint>
#include <vector>

#include "truckRole_e.h"
#include "MovementDirection.h"

class Message {
    private:
        int receiver_id;
        int sender_id;
        uint64_t logicalClock_u64;
        uint8_t controllerSerialNumber_u8;
        truckRole_e role_e;
        int speed;
        MovementDirection direction;
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

        truckRole_e getRole() const;
        void setRole(truckRole_e role);

        int getSpeed() const;
        void setSpeed(int speed);

        MovementDirection getDirection() const;
        void setDirection(MovementDirection direction);
};


class MessageID {
private:
    std::vector<int> receiver_ids;

public:
    MessageID() = default;
    void addReceiverId(int id);
    const std::vector<int>& getReceiverIds() const;

    // These will be implemented in Message.cpp
    std::string toJSON() const;
    static MessageID fromJSON(const std::string& jsonString);
};


#endif //SOCKETTEST_MESSAGE_H
