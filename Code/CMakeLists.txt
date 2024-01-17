cmake_minimum_required(VERSION 3.27)
project(Code)

set(CMAKE_CXX_STANDARD 17)

add_executable(Code main.cpp
        lib/lib.h
        controller/controller.cpp
        controller/controller.h
        lib/MessageStruct.h
        lib/SocketClientIDStruct.h
        lib/Movement.h
        lib/MessageParse.h
        lib/Message.cpp
        server/CommsModuleServer.h
        server/CommsModuleServer.cpp
        communication/CommsModule.cpp
        communication/CommsModule.cpp
        lib/Event.h
        lib/TruckMetadata.h
        truck/Truck.h
        truck/Truck.cpp
        decryptor/decryptor.h
        decryptor/decryptor.cpp
        lib/SurroundingTruck.h
        lib/Location.h)
