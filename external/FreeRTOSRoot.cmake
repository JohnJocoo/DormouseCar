##########################################################################
# "THE ANY BEVERAGE-WARE LICENSE" (Revision 42 - based on beer-ware
# license):
# <dev@layer128.net> wrote this file. As long as you retain this notice
# you can do whatever you want with this stuff. If we meet some day, and
# you think this stuff is worth it, you can buy me a be(ve)er(age) in
# return. (I don't like beer much.)
#
# Matthias Kleemann
##########################################################################

cmake_minimum_required(VERSION 3.2)

### TOOLCHAIN SETUP AREA #################################################
# Set any variables used in the toolchain prior project() call. In that
# case they are already set and used.
##########################################################################

##########################################################################
# tools to beused for programming the AVR
##########################################################################
set(AVR_UPLOADTOOL "avrdude" CACHE STRING "Uploader program")
set(AVR_PROGRAMMER "arduino" CACHE STRING "Programmer (ex. avrdude -c ?)")
set(AVR_UPLOADTOOL_PORT ""   CACHE STRING "Port (/dev/usb2 or COM1, etc.)")

##########################################################################
# AVR and fuses needs to be set
##########################################################################
set(AVR_MCU "atmega328p" CACHE STRING "Microcontroller model")
set(AVR_H_FUSE 0xde      CACHE STRING "High fuse")
set(AVR_L_FUSE 0xff      CACHE STRING "Low fuse")

### END TOOLCHAIN SETUP AREA #############################################

# FREERTOS memory allocation options (pvPortMalloc, vPortFree):
# "static" - do not allocate dynamic memory (use only static functions)
# "dynamic" - allocate dynamic memory (use both static and dynamic functions), use 'optimal' memory management algorithm
# "dynamic_malloc" - same as "dynamic", use wrapper around libc "malloc"
# "dynamic_no_free" - same as "dynamic", use tiny and simple implementation without deallocation
set(FREERTOS_MEM_ALLOC_MODE "dynamic" CACHE STRING "Memory allocation mode for FREERTOS. Options : 'static', 'dynamic', 'dynamic_malloc', 'dynamic_no_free'")

# Intentionally left blank, due to a different approach of using the
# toolchain file via -DCMAKE_TOOLCHAIN_FILE=path/to/generic-gcc-avr.cmake
# at the cmake command line call
#
# include(../generic-gcc-avr.cmake)

##########################################################################
# name your project
##########################################################################
project(FreeRTOS LANGUAGES C)

##########################################################################
# status messages
##########################################################################
message(STATUS "Current uploadtool is: ${AVR_UPLOADTOOL}")
message(STATUS "Current programmer is: ${AVR_PROGRAMMER}")
message(STATUS "Current upload port is: ${AVR_UPLOADTOOL_PORT}")
message(STATUS "Current uploadtool options are: ${AVR_UPLOADTOOL_OPTIONS}")
message(STATUS "Current MCU is set to: ${AVR_MCU}")
message(STATUS "Current H_FUSE is set to: ${AVR_H_FUSE}")
message(STATUS "Current L_FUSE is set to: ${AVR_L_FUSE}")
message(STATUS "FreeRTOS memory allocation is set to: ${FREERTOS_MEM_ALLOC_MODE}")

##########################################################################
# set build type
##########################################################################
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

##########################################################################
# needs to be defined for AVR toolchain
##########################################################################
set(MCU_SPEED "16000000UL" CACHE STRING "Controller speed")

##########################################################################
# some cmake cross-compile necessities
##########################################################################
if(DEFINED ENV{AVR_FIND_ROOT_PATH})
    set(CMAKE_FIND_ROOT_PATH $ENV{AVR_FIND_ROOT_PATH})
else(DEFINED ENV{AVR_FIND_ROOT_PATH})
    if(EXISTS "/opt/local/avr")
      set(CMAKE_FIND_ROOT_PATH "/opt/local/avr")
    elseif(EXISTS "/usr/avr")
      set(CMAKE_FIND_ROOT_PATH "/usr/avr")
    elseif(EXISTS "/usr/lib/avr")
      set(CMAKE_FIND_ROOT_PATH "/usr/lib/avr")
    elseif(EXISTS "/usr/local/CrossPack-AVR")
      set(CMAKE_FIND_ROOT_PATH "/usr/local/CrossPack-AVR")
    else(EXISTS "/opt/local/avr")
      message(FATAL_ERROR "Please set AVR_FIND_ROOT_PATH in your environment.")
    endif(EXISTS "/opt/local/avr")
endif(DEFINED ENV{AVR_FIND_ROOT_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# not added automatically, since CMAKE_SYSTEM_NAME is "generic"
set(CMAKE_SYSTEM_INCLUDE_PATH "${CMAKE_FIND_ROOT_PATH}/include")
set(CMAKE_SYSTEM_LIBRARY_PATH "${CMAKE_FIND_ROOT_PATH}/lib")

##########################################################################
# status messages for generating
##########################################################################
message(STATUS "Set CMAKE_FIND_ROOT_PATH to ${CMAKE_FIND_ROOT_PATH}")
message(STATUS "Set CMAKE_SYSTEM_INCLUDE_PATH to ${CMAKE_SYSTEM_INCLUDE_PATH}")
message(STATUS "Set CMAKE_SYSTEM_LIBRARY_PATH to ${CMAKE_SYSTEM_LIBRARY_PATH}")

##########################################################################
# set compiler options for build types
##########################################################################
if(CMAKE_BUILD_TYPE MATCHES Release)
   set(CMAKE_C_FLAGS_RELEASE "-Os")
   set(CMAKE_CXX_FLAGS_RELEASE "-Os")
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
   set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
   set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -save-temps -g -gdwarf-3 -gstrict-dwarf")
endif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

if(CMAKE_BUILD_TYPE MATCHES Debug)
   set(CMAKE_C_FLAGS_DEBUG "-O0")
   set(CMAKE_CXX_FLAGS_DEBUG "-O0")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

##########################################################################
# compiler options for all build types
##########################################################################
add_definitions("-DF_CPU=${MCU_SPEED}")
add_definitions("-fpack-struct")
add_definitions("-fshort-enums")
add_definitions("-Wall")
# TODO turn on -Werror by ignoring all False Positives
#add_definitions("-Werror")
add_definitions("-pedantic")
# TODO turn on -pedantic-errors by ignoring all False Positives
#add_definitions("-pedantic-errors")
add_definitions("-funsigned-char")
add_definitions("-funsigned-bitfields")
add_definitions("-ffunction-sections")
add_definitions("-c")
add_definitions("-std=gnu99")

##########################################################################
# include search paths
##########################################################################

##########################################################################
# building library and application in their subdirectories
##########################################################################
add_subdirectory(freeRTOS10xx)

##########################################################################
# testing functions w/o source files - gets FATAL_ERROR
##########################################################################
# add_avr_executable(test_breaks_generation)
# add_avr_library(mytestlib_breaks_generation)

##########################################################################
# use default documentation target
##########################################################################


