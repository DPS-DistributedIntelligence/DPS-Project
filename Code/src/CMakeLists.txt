add_executable(CommsModuleServerTest server/CommsModuleServerMain.cpp server/CommsModuleServer.cpp lib/MessageParser.cpp lib/Message.cpp)
if(WIN32)
    target_link_libraries(CommsModuleTest wsock32 ws2_32)
    target_link_libraries(CommsModuleServerTest wsock32 ws2_32)
endif()
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
