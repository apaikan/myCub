# Copyright: (C) ....
# Authors: Houman Dallali, Ali Paikan
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.txt

cmake_minimum_required(VERSION 2.8)


FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})
MESSAGE(STATUS "${SWIG_USE_FILE}")

find_package(Lua51 REQUIRED) 
INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} )

#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
#                    /usr/include/linux
#                    /usr/include/x86_64-linux-gnu
#                    /usr/include/x86_64-linux-gnu/asm
#                    /usr/include/c++/4.7
#                    /usr/include
#                    /usr/local/include)

SET(CMAKE_SWIG_FLAGS "-c++;-Wall;-module;serialport")
SET_SOURCE_FILES_PROPERTIES(serialport.i PROPERTIES CPLUSPLUS ON)
#SET_SOURCE_FILES_PROPERTIES(serialport.i PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(serialport lua serialport.i PosixSignalDispatcher.cpp SerialPort.cpp)
SWIG_LINK_LIBRARIES(serialport ${LUA_LIBRARY})

