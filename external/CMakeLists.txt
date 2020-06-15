cmake_minimum_required(VERSION 3.2)

include(ExternalProject)

set(ARDUINO_BUILD_ROOT "${CMAKE_BINARY_DIR}/thirdparty/arduino_core_avr")
set(ARDUINO_SRC "${ARDUINO_BUILD_ROOT}/source")
set(ARDUINO_BUILD "${ARDUINO_BUILD_ROOT}/build")
set(ARDUINO_LIB_NAME "libarduinocoreavr-${AVR_MCU}.a")

ExternalProject_Add(ArduinoCoreAvrEx
                    PREFIX "${ARDUINO_BUILD_ROOT}"
                    SOURCE_DIR "${ARDUINO_SRC}"
                    BINARY_DIR "${ARDUINO_BUILD}"
                    GIT_REPOSITORY "https://github.com/arduino/ArduinoCore-avr.git"
                    GIT_TAG "1.8.2"
                    UPDATE_COMMAND ""
                    PATCH_COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/external/ArduinoCoreAvr.cmake" "${ARDUINO_SRC}/CMakeLists.txt"
                    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DAVR_UPLOADTOOL_PORT=${AVR_UPLOADTOOL_PORT} -DAVR_UPLOADTOOL=${AVR_UPLOADTOOL} -DAVR_PROGRAMMER=${AVR_PROGRAMMER} -DAVR_MCU=${AVR_MCU} -DAVR_H_FUSE=${AVR_H_FUSE} -DAVR_L_FUSE=${AVR_L_FUSE} -DMCU_SPEED=${MCU_SPEED} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                    BUILD_BYPRODUCTS "${ARDUINO_BUILD}/${ARDUINO_LIB_NAME}"
                    INSTALL_COMMAND "")

add_library(libarduino_core_avr STATIC IMPORTED)
set_target_properties(libarduino_core_avr PROPERTIES
        IMPORTED_LOCATION "${ARDUINO_BUILD}/${ARDUINO_LIB_NAME}"
        INCLUDE_DIRECTORIES "${ARDUINO_BUILD}/include")

set(LIBARDUINO_CORE_AVR_LIB "${ARDUINO_LIB_NAME}")
set(LIBARDUINO_CORE_AVR_LIBPATH "${ARDUINO_BUILD}")
set(LIBARDUINO_CORE_AVR_INCLUDE "${ARDUINO_BUILD}/include")

add_dependencies(libarduino_core_avr ArduinoCoreAvrEx)

#############################################################

set(PWMSERVO_BUILD_ROOT "${CMAKE_BINARY_DIR}/thirdparty/pwm_servo")
set(PWMSERVO_SRC "${PWMSERVO_BUILD_ROOT}/source")
set(PWMSERVO_BUILD "${PWMSERVO_BUILD_ROOT}/build")
set(PWMSERVO_LIB_NAME "libpwmservo-${AVR_MCU}.a")

ExternalProject_Add(PWMServoEx
        DEPENDS ArduinoCoreAvrEx
        PREFIX "${PWMSERVO_BUILD_ROOT}"
        SOURCE_DIR "${PWMSERVO_SRC}"
        BINARY_DIR "${PWMSERVO_BUILD}"
        GIT_REPOSITORY "https://github.com/PaulStoffregen/PWMServo.git"
        GIT_TAG "2.1"
        UPDATE_COMMAND ""
        PATCH_COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/external/PWMServo.cmake" "${PWMSERVO_SRC}/CMakeLists.txt"
        CMAKE_ARGS -DARDUINO_INCLUDE=${ARDUINO_SRC}/cores/arduino -DARDUINO_INCLUDE2=${ARDUINO_BUILD}/include -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DAVR_UPLOADTOOL_PORT=${AVR_UPLOADTOOL_PORT} -DAVR_UPLOADTOOL=${AVR_UPLOADTOOL} -DAVR_PROGRAMMER=${AVR_PROGRAMMER} -DAVR_MCU=${AVR_MCU} -DAVR_H_FUSE=${AVR_H_FUSE} -DAVR_L_FUSE=${AVR_L_FUSE} -DMCU_SPEED=${MCU_SPEED} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        BUILD_BYPRODUCTS "${PWMSERVO_BUILD}/${PWMSERVO_LIB_NAME}"
        INSTALL_COMMAND "")

add_library(libpwm_servo STATIC IMPORTED)
set_target_properties(libpwm_servo PROPERTIES
        IMPORTED_LOCATION "${PWMSERVO_BUILD}/${PWMSERVO_LIB_NAME}"
        INCLUDE_DIRECTORIES "${PWMSERVO_BUILD}/include")

set(LIBPWM_SERVO_LIB "${PWMSERVO_LIB_NAME}")
set(LIBPWM_SERVO_LIBPATH "${PWMSERVO_BUILD}")
set(LIBPWM_SERVO_INCLUDE "${PWMSERVO_BUILD}/include")

add_dependencies(libpwm_servo PWMServoEx)

#############################################################

set(FREERTOS_BUILD_ROOT "${CMAKE_BINARY_DIR}/thirdparty/free_rtos")
set(FREERTOS_SRC "${FREERTOS_BUILD_ROOT}/source")
set(FREERTOS_BUILD "${FREERTOS_BUILD_ROOT}/build")
set(FREERTOS_LIB_NAME "libfreertosavr-${AVR_MCU}.a")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(FREERTOS_MEM_ALLOC_MODE "dynamic")
else(CMAKE_BUILD_TYPE MATCHES Debug)
    set(FREERTOS_MEM_ALLOC_MODE "dynamic")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

ExternalProject_Add(FreeRTOSAvrEx
        PREFIX "${FREERTOS_BUILD_ROOT}"
        SOURCE_DIR "${FREERTOS_SRC}"
        BINARY_DIR "${FREERTOS_BUILD}"
        GIT_REPOSITORY "https://github.com/feilipu/avrfreertos.git"
        GIT_TAG "v10.3.0-1"
        UPDATE_COMMAND ""
        PATCH_COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/external/FreeRTOSRoot.cmake" "${FREERTOS_SRC}/CMakeLists.txt"
            COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/external/FreeRTOSsource10xx.cmake" "${FREERTOS_SRC}/freeRTOS10xx/CMakeLists.txt"
            COMMAND "${CMAKE_COMMAND}" -E copy "${FREERTOS_SRC}/freeRTOS10xx/include/FreeRTOSConfig.h" "${FREERTOS_SRC}/freeRTOS10xx/include/FreeRTOSConfig.h.original"
            COMMAND "${CMAKE_COMMAND}" -E copy "${FREERTOS_SRC}/freeRTOS10xx/include/FreeRTOSBoardDefs.h" "${FREERTOS_SRC}/freeRTOS10xx/include/FreeRTOSBoardDefs.h.original"
            COMMAND "${CMAKE_COMMAND}" -E remove "${FREERTOS_SRC}/freeRTOS10xx/include/FreeRTOSConfig.h"
            COMMAND "${CMAKE_COMMAND}" -E remove "${FREERTOS_SRC}/freeRTOS10xx/include/FreeRTOSBoardDefs.h"
            COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/external/FreeRTOSConfig.h.source" "${FREERTOS_SRC}/freeRTOS10xx/include/FreeRTOSConfig.h.source"
            COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/external/FreeRTOSBoardDefs.h" "${FREERTOS_SRC}/freeRTOS10xx/include/FreeRTOSBoardDefs.h"
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DAVR_UPLOADTOOL_PORT=${AVR_UPLOADTOOL_PORT} -DAVR_UPLOADTOOL=${AVR_UPLOADTOOL} -DAVR_PROGRAMMER=${AVR_PROGRAMMER} -DAVR_MCU=${AVR_MCU} -DAVR_H_FUSE=${AVR_H_FUSE} -DAVR_L_FUSE=${AVR_L_FUSE} -DMCU_SPEED=${MCU_SPEED} -DFREERTOS_MEM_ALLOC_MODE=${FREERTOS_MEM_ALLOC_MODE} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        BUILD_BYPRODUCTS "${FREERTOS_BUILD}/${FREERTOS_LIB_NAME}"
        INSTALL_COMMAND "")

add_library(libfree_rtos STATIC IMPORTED)
set_target_properties(libfree_rtos PROPERTIES
        IMPORTED_LOCATION "${FREERTOS_BUILD}/${FREERTOS_LIB_NAME}"
        INCLUDE_DIRECTORIES "${FREERTOS_BUILD}/include")

set(LIBFREERTOS_LIB "${FREERTOS_LIB_NAME}")
set(LIBFREERTOS_LIBPATH "${FREERTOS_BUILD}")
set(LIBFREERTOS_INCLUDE "${FREERTOS_BUILD}/include")

add_dependencies(libfree_rtos FreeRTOSAvrEx)
