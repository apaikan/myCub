# Copyright: (C) ....
# Authors: Ali Paikan
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.txt

cmake_minimum_required(VERSION 2.8.7)
project(FUNKOS)

# Pick up our CMake scripts - they are all in the conf subdirectory.
set(FUNKOS_MODULE_PATH ${PROJECT_SOURCE_DIR}/conf)
set(CMAKE_MODULE_PATH ${FUNKOS_MODULE_PATH})

find_package(AvrGcc REQUIRED)

set(
    SYS_FREQUENCY 16000000
    CACHE STRING
    "System frequency (in Hz)")

set(
    SYS_TICK 1000
    CACHE STRING
    "Frequency (in Hz) that the system tick is called")


# add some property if needed
set_property(GLOBAL PROPERTY FUNKOS_INCLUDE_DIRS)
set_property(GLOBAL PROPERTY FUNKOS_LIBS)

ADD_DEFINITIONS( -DF_CPU=${SYS_FREQUENCY} )
ADD_DEFINITIONS( -DTICK_FREQ=${SYS_TICK} )
ADD_DEFINITIONS( -DARCH_AVR )

include_directories(./kernel 
                    ./kernel/atmega328p
                    ./drivers/ardulite
                    ./drivers/serial)

# for cmake bug on '-rdynamic'
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

add_avr_executable( srf04
                    ./kernel/atmega328p/kernelswi.c
                    ./kernel/atmega328p/kerneltimer.c
                    ./kernel/atmega328p/kernelwdt.c
                    ./kernel/atmega328p/taskport.c                     
                    ./kernel/driver.c  
                    ./kernel/heap.c
                    ./kernel/message.c
                    ./kernel/mutex.c
                    ./kernel/semaphore.c
                    ./kernel/task.c
                    ./kernel/timer.c
                    ./kernel/watchdog.c
                    ./drivers/ardulite/pins_arduino.c  
                    ./drivers/ardulite/wiring_analog.c  
                    ./drivers/ardulite/wiring.c  
                    ./drivers/ardulite/wiring_digital.c  
                    ./drivers/ardulite/wiring_pulse.c  
                    ./drivers/ardulite/wiring_shift.c
                    ./drivers/serial/uart_async.c
                    ./projects/srf04/main.c )
