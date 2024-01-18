cmake_minimum_required(VERSION 3.26)
project(DPS_Platoon)

set(CMAKE_CXX_STANDARD 23)

add_executable(
        Server

        server/CommsModuleServerMain.cpp
        server/CommsModuleServer.cpp

        lib/MessageParser.cpp
        lib/Message.cpp
)

add_executable(
        Truck main.cpp

        truck/Truck.cpp
        truck/controller/controller.cpp
        truck/interface/interface.cpp

        lib/LogicalClock.cpp
        lib/Movement.cpp
        lib/MessageParser.cpp
        lib/Message.cpp

        client/CommsModule.cpp
)

if(WIN32)
    target_link_libraries(Server wsock32 ws2_32)
    target_link_libraries(Truck wsock32 ws2_32)
endif ()