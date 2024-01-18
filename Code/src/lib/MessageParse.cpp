#include "MessageParse.h"

string MessageParser::truckRoleToString(truckRole role) {
    return "";
}

// Convert a string to a truckRole_e enum value
truckRole MessageParser::stringToTruckRole(const std::string& roleStr) {
    return NOT_SET;
}

// Serialize a Message object to a JSON string
string MessageParser::toJSON(const Message& msg){
    return "";
}

string MessageParser::directionToString(movementDirection direction){
    return "";
}

movementDirection MessageParser::stringToDirection(const std::string& directionStr){
    return MOVE_FORWARD;
}

// Deserialize a JSON string to a Message object
Message MessageParser::fromJSON(const std::string& jsonString){
    return {};
}

