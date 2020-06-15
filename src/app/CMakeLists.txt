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

include_directories("${LIBFREERTOS_INCLUDE}")
include_directories("${LIBARDUINO_CORE_AVR_INCLUDE}")
include_directories("${LIBPWM_SERVO_INCLUDE}")
link_directories("${LIBFREERTOS_LIBPATH}")
link_directories("${LIBARDUINO_CORE_AVR_LIBPATH}")
link_directories("${LIBPWM_SERVO_LIBPATH}")

find_library(C_LIB c)
message(STATUS "avr-libc: ${C_LIB}")

set(APP_INCLUDE
      "config.h")

set(APP_SRC
      "main.cpp")

add_avr_executable(
   dormouse-car
   ${APP_SRC}
   ${APP_INCLUDE}
)

avr_target_link_libraries(dormouse-car
        "${LIBPWM_SERVO_LIB}"
        "${LIBARDUINO_CORE_AVR_LIB}"
        "${LIBFREERTOS_LIB}"
        "${C_LIB}")
set(dormouse-car-bin "dormouse-car-${AVR_MCU}.elf")

add_dependencies(dormouse-car FreeRTOSAvrEx)
add_dependencies(dormouse-car ArduinoCoreAvrEx)
add_dependencies(dormouse-car PWMServoEx)
add_dependencies(${dormouse-car-bin} FreeRTOSAvrEx)
add_dependencies(${dormouse-car-bin} ArduinoCoreAvrEx)
add_dependencies(${dormouse-car-bin} PWMServoEx)
