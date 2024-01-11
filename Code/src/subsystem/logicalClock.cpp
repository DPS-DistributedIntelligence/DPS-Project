//
// Created by Luis on 1/7/2024.
//

#include "logicalClock.h"

void logicalClock::logicalClockTick()
{
    this->logicalClock_u64++;
}

bool logicalClock::logicalClockTickCompare(uint64_t varGlobalLogicalClock)
{
    return (this->logicalClock_u64 == varGlobalLogicalClock);
}

void logicalClock::logicalClockUpdate(uint64_t varGlobalLogicalClock)
{
    this->logicalClock_u64 = (this->logicalClock_u64 > varGlobalLogicalClock) ? (this->logicalClock_u64) : (varGlobalLogicalClock);
    printf(RED_TEXT "Event: Logical clock differs from Leader! \n" RESET_COLOR);
}

uint64_t logicalClock::get_logicalClock()
{
    return this->logicalClock_u64;
}