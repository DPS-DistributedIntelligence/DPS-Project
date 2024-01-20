//
// Created by leand on 16.01.2024.
//

#ifndef SOCKETTEST_MESSAGEPARSER_H
#define SOCKETTEST_MESSAGEPARSER_H

#pragma once

#include "Message.h"
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <variant>



class MessageParser {
public:
    // Convert a truckRole enum value to a string representation
    static std::string truckRoleToString(truckRole role);

    // Convert a string to a truckRole enum value
    static truckRole stringToTruckRole(const std::string& roleStr);

    // Serialize a Message object to a JSON string
    static std::string toJSON(Message msg);

    // Serialize a MessageID object to a JSON string
    static std::string toJSONID(MessageID& messageId);

    static std::string directionToString(movementDirection direction);

    static movementDirection stringToDirection(std::string& directionStr);

    // Deserialize a JSON string to a Message object
    static Message fromJSON(const std::string& jsonString);

    // Deserialize a MessageID string to MessageID object
    static MessageID fromJSONMessageID(const std::string& jsonString);

    static std::variant<Message, MessageID> fromJSONVariant (const std::string& jsonString);
};


#endif //SOCKETTEST_MESSAGEPARSER_H
