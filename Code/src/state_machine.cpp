
#include "state_machine.h"

#define SM_MAX_STATES       (10)

typedef enum
{
    sm_init_state,          //0
    sm_waiting_state,       //1
    sm_moving_state,        //2
    sm_aligning_state,      //3
    sm_stop_state,          //4
    sm_emergencyStop_state, //5
    sm_errorHandling_state, //6
    sm_systemStop_state     //7
}sm_states_e;

typedef struct
{
    next_state_e (*stateFunc)();
    sm_states_e const *next;
}state_machine_str;

const sm_states_e sm_init_next = sm_waiting_state;

const sm_states_e sm_waiting_next[2] =
{
        sm_waiting_state,
        sm_moving_state
};

const sm_states_e sm_moving_next[5] =
{
        sm_moving_state,
        sm_emergencyStop_state,
        sm_aligning_state,
        sm_errorHandling_state,
        sm_stop_state
};

const sm_states_e sm_aligning_next[2] =
{
        sm_moving_state,
        sm_errorHandling_state
};

const sm_states_e sm_stop_next = sm_init_state;

const sm_states_e sm_emergencyStop_next[2] =
{
        sm_waiting_state,
        sm_errorHandling_state
};

const sm_states_e sm_errorHandling_next[2] =
{
        sm_moving_state,
        sm_emergencyStop_state
};

const state_machine_str state_machine[SM_MAX_STATES] =
{
    /***** Initialization of the system  *****/
    {SM_Initialize_System, &sm_init_next},
    {SM_Waiting_System, sm_waiting_next},
    {SM_Moving_System, sm_moving_next},
    {SM_Aligning_System, sm_aligning_next},
    {SM_Stop_System, &sm_stop_next},
    {SM_EmergencyStop_System, sm_emergencyStop_next},
    {SM_ErrorHandling_System, sm_errorHandling_next},

    {SM_System_Stop, NULL},
};

sm_states_e sm_iterator = sm_init_state;

void SM_state_machine(void)
{
    while(1)
    {
        sm_iterator = *(state_machine[sm_iterator].next
                + state_machine[sm_iterator].stateFunc());
    }
}


