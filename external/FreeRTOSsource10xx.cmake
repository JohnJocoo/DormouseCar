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

set(FREERTOS_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${FREERTOS_INCLUDE})

file(GLOB LIB_HEADERS
      "${FREERTOS_INCLUDE}/*.h")

set(LIB_SRC
      "${SRC_DIR}/croutine.c"
      "${SRC_DIR}/event_groups.c"
      "${SRC_DIR}/hooks.c"
      "${SRC_DIR}/list.c"
      "${SRC_DIR}/queue.c"
      "${SRC_DIR}/stream_buffer.c"
      "${SRC_DIR}/tasks.c"
      "${SRC_DIR}/timers.c"
      "${SRC_DIR}/portable/port.c")
      
if(FREERTOS_MEM_ALLOC_MODE STREQUAL "dynamic")
    list(APPEND LIB_SRC "${SRC_DIR}/MemMang/heap_4.c")
    set(FREERTOS_USE_DYNAMIC "1")
elseif(FREERTOS_MEM_ALLOC_MODE STREQUAL "dynamic_malloc")
    list(APPEND LIB_SRC "${SRC_DIR}/MemMang/heap_3.c")
    set(FREERTOS_USE_DYNAMIC "1")
elseif(FREERTOS_MEM_ALLOC_MODE STREQUAL "dynamic_no_free")
    list(APPEND LIB_SRC "${SRC_DIR}/MemMang/heap_1.c")
    set(FREERTOS_USE_DYNAMIC "1")
elseif(FREERTOS_MEM_ALLOC_MODE STREQUAL "static")
    message(STATUS "Using static-only version of FreeRTOS")
    set(FREERTOS_USE_DYNAMIC "0")
else(FREERTOS_MEM_ALLOC_MODE STREQUAL "dynamic")
    message(FATAL_ERROR "FREERTOS_MEM_ALLOC_MODE is '${FREERTOS_MEM_ALLOC_MODE}', but can only be one of 'static', 'dynamic', 'dynamic_malloc', 'dynamic_no_free'")
endif(FREERTOS_MEM_ALLOC_MODE STREQUAL "dynamic")

configure_file("${FREERTOS_INCLUDE}/FreeRTOSConfig.h.source" "${FREERTOS_INCLUDE}/FreeRTOSConfig.h")

list(APPEND LIB_HEADERS "${FREERTOS_INCLUDE}/FreeRTOSConfig.h")

add_avr_library(
   freertosavr
   ${LIB_SRC}
   ${LIB_HEADERS}
)

add_custom_command(TARGET freertosavr
                   POST_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/include/FreeRTOS"
                   COMMAND "${CMAKE_COMMAND}" -E copy ${LIB_HEADERS} "${CMAKE_BINARY_DIR}/include/FreeRTOS"
                   COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_BINARY_DIR}/freeRTOS10xx/libfreertosavr-${AVR_MCU}.a" "${CMAKE_BINARY_DIR}")
