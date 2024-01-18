#include "LogicalClock.h"

void logicalClock::logicalClockTick()
{
    this->logicalClock_u64++;
}

bool logicalClock::logicalClockTickCompare(int varGlobalLogicalClock)
{
    return (this->logicalClock_u64 == varGlobalLogicalClock);
}

void logicalClock::logicalClockUpdate(int varGlobalLogicalClock)
{
    this->logicalClock_u64 = (this->logicalClock_u64 > varGlobalLogicalClock) ? (this->logicalClock_u64) : (varGlobalLogicalClock);
    //printf(RED_TEXT "Event: Logical clock differs from Leader! \n" RESET_COLOR);
}

int logicalClock::get_logicalClock()
{
    return this->logicalClock_u64;
}

logicalClock::logicalClock() {
    logicalClock_u64 = 0;
}
