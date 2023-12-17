//
// Created by Luis on 12/12/2023.
//

#ifndef DPS_PLATOON_TIME_H
#define DPS_PLATOON_TIME_H

#include <cstdint>
#include "src/lib/commonLib.h"

class time {
private:
    uint32_t timestamp_u32 = 0;
public:
    time();
    /*
     * Description:
     * Parameters:
     * 	[in]
     * 	[out]
     */
    uint32_t getTimestamp_U32();
};


#endif //DPS_PLATOON_TIME_H
