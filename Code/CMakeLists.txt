cmake_minimum_required(VERSION 3.27)
project(Code)

set(CMAKE_CXX_STANDARD 17)

add_executable( Code
        src/main.cpp
        src/controller/controller.cpp
        src/lib/Message.cpp
        src/decryptor/decryptor.cpp
        src/Truck/Truck.cpp
        src/lib/LogicalClock.cpp
        src/interface/interface.cpp
        src/interface/interface.h
        src/lib/MessageParse.cpp
        src/lib/Movement.cpp
)
