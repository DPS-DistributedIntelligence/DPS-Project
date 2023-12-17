cmake_minimum_required(VERSION 3.27)
project(Code)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(src)
include_directories(src/class)
include_directories(src/controller)

add_executable(Code
        src/component/subsystem/antiCollisionSystem.cpp
        src/component/subsystem/antiCollisionSystem.h
        src/lib/commonLib.h
        src/component/subsystem/driverInterface.cpp
        src/component/subsystem/driverInterface.h
        src/lib/licensePlate.cpp
        src/lib/licensePlate.h
        src/class/trash/time.cpp
        src/class/trash/time.h
        src/component/controller/controller.cpp
        src/component/controller/controller.h
        src/main.c)
