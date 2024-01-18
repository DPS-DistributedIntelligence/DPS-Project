//
// Created by Mykyta on 18.01.2024.
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

using namespace std;

class MessageParser {
public:
    // Convert a truckRole_e enum value to a string representation
    static string truckRoleToString(truckRole_e role);

    // Convert a string to a truckRole_e enum value
    static truckRole_e stringToTruckRole(const std::string& roleStr);

    // Serialize a Message object to a JSON string
    static string toJSON(const Message& msg);

    // Serialize a MessageID object to a JSON string
    static string toJSONID(const MessageID& messageId);

    static string directionToString(MovementDirection direction);

    static MovementDirection stringToDirection(const std::string& directionStr);

    static string eventToString(event ev);

    static event stringToEvent(const std::string& evStr);

    // Deserialize a JSON string to a Message object
    static Message fromJSON(const std::string& jsonString);

    // Deserialize a MessageID string to MessageID object
    static MessageID fromJSONMessageID(const std::string& jsonString);

    static variant<Message, MessageID> fromJSONVariant (const std::string& jsonString);
};


#endif //SOCKETTEST_MESSAGEPARSER_H
