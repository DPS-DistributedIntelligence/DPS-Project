cmake_minimum_required(VERSION 3.26)
project(DPS_Platoon)

set(CMAKE_CXX_STANDARD 23)

add_executable(
        Code main.cpp
        truck/controller/controller.cpp
        truck/controller/controller.h
        lib/Message.cpp
        truck/decryptor/decryptor.cpp

        Truck/Truck.cpp
        lib/LogicalClock.cpp
        truck/interface/interface.h
        truck/interface/interface.cpp
        lib/Movement.cpp
)
