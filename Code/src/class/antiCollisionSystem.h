//
// Created by Luis on 12/12/2023.
//

#ifndef DPS_PLATOON_ANTICOLLISIONSYSTEM_H
#define DPS_PLATOON_ANTICOLLISIONSYSTEM_H

#include "commonLib.h"

class antiCollisionSystem {

private:
    float emergencyStopDistance_float = 0;
public:
    bool emergencyStop_b = false;
    antiCollisionSystem(float varEmergencyStopDistance_float);
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    float getEmergencyStopDistance_Float();
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    void setEmergencyStopDistance_Float(float emergencyStopDistanceFloat);
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    bool isEmergencyStop_B();
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    void setEmergencyStop_B(bool emergencyStopB);
};


#endif //DPS_PLATOON_ANTICOLLISIONSYSTEM_H
