//
// Created by Mykyta on 18.01.2024.
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

std::string MessageParser::eventToString(event ev) {
    switch (ev) {
        case event::ev_any: return "ev_any";
        case event::ev_stop: return "ev_stop";
        case event::ev_reset: return "ev_reset";
        case event::ev_ready: return "ev_ready";
        case event::ev_be_leader: return "ev_be_leader";
        case event::ev_be_follower: return "ev_be_follower";
        case event::ev_leader_found: return "ev_leader_found";
        case event::ev_no_leader_found: return "ev_no_leader_found";
        default: return "unknown_event";
    }
}

event MessageParser::stringToEvent(const std::string& evStr) {
    if (evStr == "ev_any") return event::ev_any;
    if (evStr == "ev_stop") return event::ev_stop;
    if (evStr == "ev_reset") return event::ev_reset;
    if (evStr == "ev_ready") return event::ev_ready;
    if (evStr == "ev_be_leader") return event::ev_be_leader;
    if (evStr == "ev_be_follower") return event::ev_be_follower;
    if (evStr == "ev_leader_found") return event::ev_leader_found;
    if (evStr == "ev_no_leader_found") return event::ev_no_leader_found;
    // Add any additional handling for unknown or default case
    return event::ev_any; // or another appropriate default value
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
       << "\"event\":" << eventToString(msg.getEvent()) << ","
       << "\"role_e\":\"" << truckRoleToString(msg.getRole()) << "\""
       << "}";
    return ss.str();
}

string MessageParser::toJSONID(const MessageID& messageId) {
    std::stringstream ss;
    ss << "{ \"type\": \"MessageID\", \"receiver_ids\": [";
    const auto& ids = messageId.getReceiverIds();
    for (size_t i = 0; i < ids.size(); ++i) {
        ss << ids[i];
        if (i < ids.size() - 1) ss << ", ";
    }
    ss << "] }";
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
        value.erase(remove(value.begin(), value.end(), '\"'), value.end());

        if (key == "\"receiver_id\"") {
            msg.setReceiverId(stoi(value));
        } else if (key == "\"sender_id\"") {
            msg.setSenderId(stoi(value));
        } else if (key == "\"logicalClock_u64\"") {
            msg.setLogicalClock(stoull(value));
        } else if (key == "\"controllerSerialNumber_u8\"") {
            msg.setControllerSerialNumber(static_cast<uint8_t>(stoi(value)));
        } else if (key == "\"role_e\"") {
            msg.setRole(stringToTruckRole(value));
        } else if (key == "\"speed\"") {
            msg.setSpeed(stoi(value));
        } else if (key == "\"direction\"") {
            msg.setDirection(stringToDirection(value));
        } else if (key == "\"event\"") {
            // Parsing the event key if present
            msg.setEvent(stringToEvent(value)); // Ensure Message class has setEvent method to handle this
        }
    }
    return msg;

}


MessageID MessageParser::fromJSONMessageID(const std::string& jsonString) {
    MessageID messageId;
    std::string idListStr = jsonString.substr(jsonString.find("[") + 1);
    idListStr = idListStr.substr(0, idListStr.find("]"));

    std::istringstream iss(idListStr);
    std::string idStr;
    while (std::getline(iss, idStr, ',')) {
        idStr.erase(std::remove_if(idStr.begin(), idStr.end(), [](unsigned char ch) { return std::isspace(ch); }), idStr.end());
        if (!idStr.empty()) {
            messageId.addReceiverId(std::stoi(idStr));
        }
    }
    
    return messageId;
}

// Overloaded fromJSON method to handle both Message and MessageID
std::variant<Message, MessageID> MessageParser::fromJSONVariant(const std::string& jsonString) {
    // Check the JSON string for a key to determine its type
    if (jsonString.find("\"type\": \"MessageID\"") != std::string::npos) {
        return fromJSONMessageID(jsonString);
    } else {
        // Assume it's a Message type
        return fromJSON(jsonString); // This method needs to be implemented
    }
}
