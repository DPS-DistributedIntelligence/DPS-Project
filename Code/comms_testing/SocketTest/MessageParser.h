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

using namespace std;

class MessageParser {
public:
    // Convert a truckRole_e enum value to a string representation
    static string truckRoleToString(truckRole_e role);

    // Convert a string to a truckRole_e enum value
    static truckRole_e stringToTruckRole(const std::string& roleStr);

    // Serialize a Message object to a JSON string
    static string toJSON(const Message& msg);

    static string directionToString(MovementDirection direction);

    static MovementDirection stringToDirection(const std::string& directionStr);

    // Deserialize a JSON string to a Message object
    static Message fromJSON(const std::string& jsonString);
};


#endif //SOCKETTEST_MESSAGEPARSER_H
