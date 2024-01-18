cmake_minimum_required(VERSION 3.27)
project(Code)

set(CMAKE_CXX_STANDARD 17)

add_executable( Code
        src/main.cpp
        src/truck/controller/controller.cpp
        src/lib/Message.cpp
        src/truck/decryptor/decryptor.cpp
        src/truck/Truck.cpp
        src/lib/LogicalClock.cpp
        src/truck/interface/interface.cpp
        src/truck/interface/interface.h
        src/lib/MessageParser.cpp
        src/lib/Movement.cpp
        src/client/CommsModule.cpp
)

add_executable(
        Server

        src/server/CommsModuleServerMain.cpp
        src/server/CommsModuleServer.cpp

        src/lib/MessageParser.cpp
        src/lib/Message.cpp
)

if(WIN32)
    target_link_libraries(Server wsock32 ws2_32)
    target_link_libraries(Code wsock32 ws2_32)
endif ()