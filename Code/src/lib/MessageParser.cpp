//
// Created by leand on 16.01.2024.
//

#include "MessageParser.h"

// Convert truckRole enum to string representation
std::string MessageParser::truckRoleToString(truckRole role) {
    switch (role) {
        case truckRole::LEADER: return "LEADER";
        case truckRole::FOLLOWER: return "FOLLOWER";
        default: return "UNKNOWN";
    }
}

// Convert string to truckRole enum
truckRole MessageParser::stringToTruckRole(const std::string& roleStr) {
    if (roleStr == "LEADER") return truckRole::LEADER;
    if (roleStr == "FOLLOWER") return truckRole::FOLLOWER;
    return truckRole::NOT_SET;
}

std::string MessageParser::directionToString(movementDirection direction) {
    switch (direction) {
        case movementDirection::MOVE_FORWARD: return "MOVE_FORWARD";
        case movementDirection::MOVE_BACK: return "MOVE_BACK";
        case movementDirection::MOVE_LEFT: return "MOVE_LEFT";
        case movementDirection::MOVE_RIGHT: return "MOVE_RIGHT";
        case movementDirection::MOVE_EMERGENCY_STOP: return "MOVE_EMERGENCY_STOP";
        case movementDirection::MOVE_STOP: return "MOVE_STOP";
        default: return "UNKNOWN_DIRECTION";
    }
}

movementDirection MessageParser::stringToDirection(const std::string& directionStr) {
    if (directionStr == "MOVE_FORWARD") return movementDirection::MOVE_FORWARD;
    if (directionStr == "MOVE_BACK") return movementDirection::MOVE_BACK;
    if (directionStr == "MOVE_LEFT") return movementDirection::MOVE_LEFT;
    if (directionStr == "MOVE_RIGHT") return movementDirection::MOVE_RIGHT;
    if (directionStr == "MOVE_EMERGENCY_STOP") return movementDirection::MOVE_EMERGENCY_STOP;
    if (directionStr == "MOVE_STOP") return movementDirection::MOVE_STOP;
    return movementDirection::MOVE_STOP; // Assuming MOVE_STOP is your default case
}

// Serialize Message object to JSON string
std::string MessageParser::toJSON(Message msg) {
    std::stringstream ss;
    ss << "{"
       << "\"receiver_id\":" << msg.getReceiverId() << ","
       << "\"sender_id\":" << msg.getSenderId() << ","
       << "\"logicalClock_u64\":" << msg.getLogicalClock() << ","
       << "\"speed\":" << msg.getSpeed() << ","
       << "\"direction\":" << directionToString(msg.getDirection()) << ","
       << "\"role_e\":\"" << truckRoleToString(msg.getRole()) << "\""
       << "}";
    return ss.str();
}

// Deserialize JSON string to Message object
Message MessageParser::fromJSON(const std::string& jsonString) {
    Message msg;
    std::istringstream jsonStream(jsonString);
    std::string temp;

    while (getline(jsonStream, temp, ',')) {
        std::istringstream tempStream(temp);
        std::string key, value;

        getline(tempStream, key, ':');
        getline(tempStream, value);

        key.erase(remove(key.begin(), key.end(), ' '), key.end());
        value.erase(remove(value.begin(), value.end(), ' '), value.end());

        if (key.find("receiver_id") != std::string::npos) {
            msg.setReceiverId(stoi(value));
        } else if (key.find("sender_id") != std::string::npos) {
            msg.setSenderId(stoi(value));
        } else if (key.find("logicalClock_u64") != std::string::npos) {
            msg.setLogicalClock(stoull(value));
        } else if (key.find("speed") != std::string::npos) {
            msg.setSpeed(stoi(value));
        } else if (key.find("role_e") != std::string::npos) {
            value.erase(remove(value.begin(), value.end(), '"'), value.end()); // Remove quotes
            msg.setRole(stringToTruckRole(value));
        } else if (key.find("direction") != std::string::npos) {
            value.erase(remove(value.begin(), value.end(), '\"'), value.end()); // Remove quotes
            msg.setDirection(stringToDirection(value));
        }
    }
    return msg;

}