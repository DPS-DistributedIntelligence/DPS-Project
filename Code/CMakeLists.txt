cmake_minimum_required(VERSION 3.27)
project(Code)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(src)
include_directories(src/class)
include_directories(src/controller)

add_executable(Code
        src/class/antiCollisionSystem.cpp
        src/class/antiCollisionSystem.h
        src/class/commonLib.h
        src/class/Controller.cpp
        src/class/Controller.h
        src/class/driverInterface.cpp
        src/class/driverInterface.h
        src/class/licensePlate.cpp
        src/class/licensePlate.h
        src/class/time.cpp
        src/class/time.h
        src/controller/controller.cpp
        src/controller/controller.h
        src/main.c)
