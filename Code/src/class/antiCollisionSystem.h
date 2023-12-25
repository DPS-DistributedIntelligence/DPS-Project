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
    antiCollisionSystem();
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    float get_EmergencyStopDistance();
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    void set_EmergencyStopDistance(float emergencyStopDistanceFloat);
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    void measureDistance(float varActualDistance);
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    bool isEmergencyStop();
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    void set_EmergencyStop(bool emergencyStopB);
};


#endif //DPS_PLATOON_ANTICOLLISIONSYSTEM_H
