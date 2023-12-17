//
// Created by sheik on 12/17/2023.
//

#include "Communication.h"

communication::communication()= default;

void communication::openNewChannel(int truck_id){
    /*
     * initialized one of the channel array in array of channel as the truck channel
     * TODO: realize this function in communication component
     */
}

void communication::sendMessageSelfChannel(movement signal){
    /*
     * put the message into truck channel to be read by follower
     * TODO: realize this function in communication component
     */
}

void communication::connectToLeader(int leaderId) {
    /*
     * connect to leader means read from the leader communication channel/ set read point
     * TODO: realize this function in communication component
     */
}

nearbyTruck *communication::getNearbyTruckList() {
    /*
     * return reference to list of nearby truck (the list held by communication component)
     * TODO: realize this function in communication component
     */
    return nearbyTruckList;
}

void communication::sendMessageSelfChannel() {

}
