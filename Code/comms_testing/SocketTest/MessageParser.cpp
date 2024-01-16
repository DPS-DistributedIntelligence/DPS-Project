//
// Created by leand on 16.01.2024.
//

#include "MessageParser.h"

// Convert truckRole_e enum to string representation
string MessageParser::truckRoleToString(truckRole_e role) {
    switch (role) {
        case truckRole_e::LEADER: return "LEADER";
        case truckRole_e::FOLLOWER: return "FOLLOWER";
        default: return "UNKNOWN";
    }
}

// Convert string to truckRole_e enum
truckRole_e MessageParser::stringToTruckRole(const string& roleStr) {
    if (roleStr == "LEADER") return truckRole_e::LEADER;
    if (roleStr == "FOLLOWER") return truckRole_e::FOLLOWER;
    return truckRole_e::UNKNOWN;
}

string MessageParser::directionToString(MovementDirection direction) {
    switch (direction) {
        case MovementDirection::MOVE_FORWARD: return "MOVE_FORWARD";
        case MovementDirection::MOVE_BACK: return "MOVE_BACK";
        case MovementDirection::MOVE_LEFT: return "MOVE_LEFT";
        case MovementDirection::MOVE_RIGHT: return "MOVE_RIGHT";
        case MovementDirection::MOVE_EMERGENCY_STOP: return "MOVE_EMERGENCY_STOP";
        case MovementDirection::MOVE_STOP: return "MOVE_STOP";
        default: return "UNKNOWN_DIRECTION";
    }
}

MovementDirection MessageParser::stringToDirection(const std::string& directionStr) {
    if (directionStr == "MOVE_FORWARD") return MovementDirection::MOVE_FORWARD;
    if (directionStr == "MOVE_BACK") return MovementDirection::MOVE_BACK;
    if (directionStr == "MOVE_LEFT") return MovementDirection::MOVE_LEFT;
    if (directionStr == "MOVE_RIGHT") return MovementDirection::MOVE_RIGHT;
    if (directionStr == "MOVE_EMERGENCY_STOP") return MovementDirection::MOVE_EMERGENCY_STOP;
    if (directionStr == "MOVE_STOP") return MovementDirection::MOVE_STOP;
    return MovementDirection::MOVE_STOP; // Assuming MOVE_STOP is your default case
}

// Serialize Message object to JSON string
string MessageParser::toJSON(const Message& msg) {
    stringstream ss;
    ss << "{"
       << "\"receiver_id\":" << msg.getReceiverId() << ","
       << "\"sender_id\":" << msg.getSenderId() << ","
       << "\"logicalClock_u64\":" << msg.getLogicalClock() << ","
       << "\"controllerSerialNumber_u8\":" << static_cast<int>(msg.getControllerSerialNumber()) << ","
       << "\"speed\":" << msg.getSpeed() << ","
       << "\"direction\":" << directionToString(msg.getDirection()) << ","
       << "\"role_e\":\"" << truckRoleToString(msg.getRole()) << "\""
       << "}";
    return ss.str();
}

// Deserialize JSON string to Message object
Message MessageParser::fromJSON(const string& jsonString) {
    Message msg;
    istringstream jsonStream(jsonString);
    string temp;

    while (getline(jsonStream, temp, ',')) {
        istringstream tempStream(temp);
        string key, value;

        getline(tempStream, key, ':');
        getline(tempStream, value);

        key.erase(remove(key.begin(), key.end(), ' '), key.end());
        value.erase(remove(value.begin(), value.end(), ' '), value.end());

        if (key.find("receiver_id") != string::npos) {
            msg.setReceiverId(stoi(value));
        } else if (key.find("sender_id") != string::npos) {
            msg.setSenderId(stoi(value));
        } else if (key.find("logicalClock_u64") != string::npos) {
            msg.setLogicalClock(stoull(value));
        } else if (key.find("controllerSerialNumber_u8") != string::npos) {
            msg.setControllerSerialNumber(static_cast<uint8_t>(stoi(value)));
        } else if (key.find("role_e") != string::npos) {
            value.erase(remove(value.begin(), value.end(), '"'), value.end()); // Remove quotes
            msg.setRole(stringToTruckRole(value));
        } else if (key.find("speed") != string::npos) {
            msg.setSpeed(stoi(value));
        } else if (key.find("direction") != string::npos) {
            value.erase(remove(value.begin(), value.end(), '\"'), value.end()); // Remove quotes
            msg.setDirection(stringToDirection(value));
        }
    }
    return msg;

}