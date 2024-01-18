cmake_minimum_required(VERSION 3.27)
project(Code)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        Code main.cpp
        truck/controller/controller.cpp
        lib/Message.cpp
        decryptor/decryptor.cpp
        Truck/Truck.cpp
        lib/LogicalClock.cpp
        truck/interface/interface.cpp
        truck/interface/interface.h
)
