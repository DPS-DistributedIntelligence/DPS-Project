//
// Created by leand on 12.01.2024.
//

#ifndef SOCKETTEST_MESSAGESTRUCT_H
#define SOCKETTEST_MESSAGESTRUCT_H

#pragma once
#include <string>

struct Message
{
    int sourceID;
    int destID;
    std::string messagestr;
};
#endif //SOCKETTEST_MESSAGESTRUCT_H
