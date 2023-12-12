

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum
{
    next_state_0,
    next_state_1,
    next_state_2,
    next_state_3,
    next_state_4
}next_state_e;

/*
 * Description:
 * Parameters:
 * 	[in]
 * 	[out]
 */
next_state_e SM_Initialize_System(void);
/*
 * Description:
 * Parameters:
 * 	[in]
 * 	[out]
 */
next_state_e SM_Waiting_System(void);
/*
 * Description:
 * Parameters:
 * 	[in]
 * 	[out]
 */
next_state_e SM_Moving_System(void);
/*
 * Description:
 * Parameters:
 * 	[in]
 * 	[out]
 */
next_state_e SM_Aligning_System(void);
/*
 * Description:
 * Parameters:
 * 	[in]
 * 	[out]
 */
next_state_e SM_Stop_System(void);
/*
 * Description:
 * Parameters:
 * 	[in]
 * 	[out]
 */
next_state_e SM_EmergencyStop_System(void);
/*
 * Description:
 * Parameters:
 * 	[in]
 * 	[out]
 */
next_state_e SM_ErrorHandling_System(void);
/*
 * Description:
 * Parameters:
 * 	[in]
 * 	[out]
 */
next_state_e SM_System_Stop(void);
/*
* Description: This function sets the control of the system via pointers from one function to another
* Parameters:
* 	[in]
* 	[out]
*/
void SM_state_machine(void);

#endif /* STATE_MACHINE_H_ */
