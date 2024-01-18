#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include "Message.h"

using namespace std;

class MessageParser {
public:
    // Convert a truckRole_e enum value to a string representation
    static string truckRoleToString(truckRole role);

    // Convert a string to a truckRole_e enum value
    static truckRole stringToTruckRole(const std::string& roleStr);

    // Serialize a Message object to a JSON string
    static string toJSON(const Message& msg);

    static string directionToString(movementDirection direction);

    static movementDirection stringToDirection(const std::string& directionStr);

    // Deserialize a JSON string to a Message object
    static Message fromJSON(const std::string& jsonString);
};


#endif