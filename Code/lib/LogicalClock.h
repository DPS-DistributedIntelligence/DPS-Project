#ifndef LOGICALCLOCK_H
#define LOGICALCLOCK_H


#include <iostream>
class logicalClock {
private:
    uint64_t logicalClock_u64;
public:
    //constructor
    logicalClock();
    /*
     * Description: Increase the amount of tick for the logical clock in a unit of one
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    void logicalClockTick();

    /*
     * Description: Compare 2 logical clocks, the local and a received one, if they are different, return false.
     * Parameters:
     * 	[in] uint64_t logicalClock
     * 	[out] bool
     */
    bool logicalClockTickCompare(uint64_t varGlobalLogicalClock);

    /*
     * Description: Compares to values for the logical clock, current and received one, it updates the current if the
     * received one is greater than the local.
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    void logicalClockUpdate(uint64_t varGlobalLogicalClock);

    /*
     * Description: Return the value of the current clock
     * Parameters:
     * 	[in] null
     * 	[out] null
     */
    uint64_t get_logicalClock();
};


#endif