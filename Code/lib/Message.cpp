#include "Message.h"

// Constructor
Message::Message()
        : receiver_id(0), sender_id(0), logicalClock_u64(0), controllerSerialNumber_u8(0), role_e(truckRole::NOT_SET) , event_(ev_any){
    // Initialize default values for the message attributes
}

// Getter for receiver_id
int Message::getReceiverId() const {
    return receiver_id;
}

// Setter for receiver_id
void Message::setReceiverId(int id) {
    receiver_id = id;
}

// Getter for sender_id
int Message::getSenderId() const {
    return sender_id;
}

// Setter for sender_id
void Message::setSenderId(int id) {
    sender_id = id;
}

// Getter for logicalClock_u64
uint64_t Message::getLogicalClock() const {
    return logicalClock_u64;
}

// Setter for logicalClock_u64
void Message::setLogicalClock(uint64_t clock) {
    logicalClock_u64 = clock;
}

// Getter for controllerSerialNumber_u8
uint8_t Message::getControllerSerialNumber() const {
    return controllerSerialNumber_u8;
}

// Setter for controllerSerialNumber_u8
void Message::setControllerSerialNumber(uint8_t serialNumber) {
    controllerSerialNumber_u8 = serialNumber;
}

// Getter for role_e
truckRole Message::getRole() const {
    return role_e;
}

// Setter for role_e
void Message::setRole(truckRole role) {
    role_e = role;
}

int Message::getSpeed() const {
    return speed;
}

void Message::setSpeed(int speed) {
    speed = speed;
}

movementDirection Message::getDirection() const {
    return direction;
}

void Message::setDirection(movementDirection direction) {
    direction = direction;
}

event Message::getEvent() const{
    return event_;
}
void Message::setEvent(event new_event){
    event_ = new_event;
}

Location Message::getLocation() const{
    return location_;
}

void Message::setLocation(Location new_location) {
    location_ = new_location;
}
