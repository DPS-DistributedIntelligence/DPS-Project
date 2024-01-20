#ifndef MESSAGESTRUCT_H
#define MESSAGESTRUCT_H

#pragma once
#include <string>

struct Message
{
    int sourceID;
    int destID;
    std::string messagestr;
};
#endif