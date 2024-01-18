cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)

project(SocketTest)


add_executable(CommsModuleTest CommsModuleTest.cpp client/CommsModule.cpp lib/MessageParser.cpp lib/Message.cpp)
add_executable(CommsModuleServerTest server/CommsModuleServerMain.cpp server/CommsModuleServer.cpp lib/MessageParser.cpp lib/Message.cpp)


if(WIN32)
    target_link_libraries(CommsModuleTest wsock32 ws2_32)
    target_link_libraries(CommsModuleServerTest wsock32 ws2_32)
endif()
