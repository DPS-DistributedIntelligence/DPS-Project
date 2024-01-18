cmake_minimum_required(VERSION 3.26)
project(DPS_Platoon)

set(CMAKE_CXX_STANDARD 23)

add_executable(
        Code main.cpp
        controller/controller.cpp
        controller/controller.h
        lib/Message.cpp
        decryptor/decryptor.cpp
        Truck/Truck.cpp
        lib/LogicalClock.cpp
        interface/interface.h
        interface/interface.cpp
        lib/Movement.cpp
)
