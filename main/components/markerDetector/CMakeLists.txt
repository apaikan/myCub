# Copyright: (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
# Authors: Ali Paikan
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)
set(PROJECTNAME markerDetector)
project(${PROJECTNAME})

find_package(YARP)
set(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

find_package(OpenCV)

file(GLOB folder_source *.cpp)
file(GLOB_RECURSE folder_header *.h)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})



include_directories(${YARP_INCLUDE_DIRS} 
					${OPENCV_INCLUDE_DIR}
                    /usr/local/ARToolkit/include)

FIND_LIBRARY(AR_LIBRARY AR /usr/local/ARToolkit/lib)

add_executable(${PROJECTNAME} ${folder_source} ${folder_header})
target_link_libraries(${PROJECTNAME} ${OpenCV_LIBRARIES} ${YARP_LIBRARIES} ${AR_LIBRARY})

#install(TARGETS ${PROJECTNAME} DESTINATION ${ICUB_DIR}/bin)
#add_subdirectory(app)

