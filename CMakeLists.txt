cmake_minimum_required(VERSION 3.22)
project(Meeting)

set(CMAKE_CXX_STANDARD 14)

#include file
include_directories(include)
include_directories(src)


add_executable(Meeting src/main.cpp
        include/block_epoll_net.h
        include/clogic.h
        include/err_str.h
        include/Mysql.h
        include/packdef.h
        include/TCPKernel.h
        include/Thread_pool.h

        src/block_epoll_net.cpp
        src/clogic.cpp
        src/err_str.cpp
        src/Mysql.cpp
        src/TCPKernel.cpp
        src/Thread_pool.cpp)
target_link_libraries(Meeting -lpthread -lmysqlclient)