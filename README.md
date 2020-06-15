# DormouseCar
Self-driving toy car based on Elegoo Smart Car platform (Arduino Uno)

## Building
Init submodule with AVR cmake toolchain

    git submodule init
    git submodule update
    
Configure project with Cmake (use your AVR build tools path instead of /usr/bin)

    cmake -DCMAKE_C_COMPILER=/usr/bin/avr-gcc -DCMAKE_CXX_COMPILER=/usr/bin/avr-g++ -DCMAKE_TOOLCHAIN_FILE=/home/anton/Workspace/cmake-avr/generic-gcc-avr.cmake -DAVR_UPLOADTOOL_PORT=/dev/ttyACM0

Build project

    cmake --build path_to_build_dir

Or

    cmake --build path_to_build_dir --target dormouse-car-atmega328p.elf

Upload to car's Arduino Uno

    cmake --build path_to_build_dir --target upload_dormouse-car

