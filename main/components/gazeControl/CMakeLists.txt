# Generated by "yarp cmake"

# A cmake file to get started with for new YARP projects.
# It assumes you want to build an executable from source code in 
# the current directory.
# Replace "yarpy" with whatever your executable should be called.

cmake_minimum_required(VERSION 2.6)


# Find YARP.  Point the YARP_DIR environment variable at your build.
find_package(YARP REQUIRED)


file(GLOB folder_source *.cpp)
file(GLOB_RECURSE folder_header *.h)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})


include_directories(${YARP_INCLUDE_DIRS})


# Set up our main executable.
add_executable(gazeControl ${folder_source} ${folder_header} )
target_link_libraries(gazeControl ${YARP_LIBRARIES})


