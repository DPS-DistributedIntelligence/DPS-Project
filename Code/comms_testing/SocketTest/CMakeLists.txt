cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)

project(SocketTest)


add_executable(CommsModuleTest CommsModuleTest.cpp CommsModule.cpp MessageParser.cpp Message.cpp)
add_executable(CommsModuleServerTest CommsModuleServerTest.cpp CommsModuleServer.cpp MessageParser.cpp Message.cpp)


if(WIN32)
    target_link_libraries(CommsModuleTest wsock32 ws2_32)
    target_link_libraries(CommsModuleServerTest wsock32 ws2_32)
endif()
