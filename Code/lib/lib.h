#ifndef LIB
#define LIB

// value
enum truckRole{NOT_SET, LEADER, FOLLOWER};
enum controllerState{initial, waiting, leader, follower, moving, align, stop, system_stop};
enum movementDirection{ forward, backward, left, right};
enum event{ ev_any, ev_stop, ev_reset, ev_ready, ev_be_leader, ev_be_follower, ev_leader_found, ev_no_leader_found };

// type definitions
typedef struct movement{
    movementDirection direction;
    int speed;
}movement;

typedef struct truck_metadata
{
    bool have_ignition_key = false;
    bool have_communication = false;
    bool have_logical_clock = false;
    bool have_anti_collision_system = false;
    // message


    int truck_id = -1;
    truckRole role = NOT_SET;
    int logical_clock = -1;
    int truck_leader_id = -1;
}truck_metadata;


#endif