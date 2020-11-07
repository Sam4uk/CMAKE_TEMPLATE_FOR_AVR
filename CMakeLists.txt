#======================================================#
#         __    _________________   _______ ______     #
#        / /   /  _/ ____/ ____/ | / / ___// ____/     #
#       / /    / // /   / __/ /  |/ /\__ \/ __/        #
#      / /____/ // /___/ /___/ /|  /___/ / /___        #
#     /_____/___/\____/_____/_/ |_//____/_____/        #
#                                                      #
#               "THE BEER-WARE LICENSE":               #
# <Sam4uk> wrote this file. As long as you retain this #
# notice you can do whatever you want  with this stuff.#
# If we meet some day, and  you  think  this stuff is  #
# worth it, you can buy me a beer in return.           #
#======================================================#

########################################################
#                                                      #
#           cmake template for projects on             #
#                AVR  microcontrollers                 #
#                                                      #
########################################################
cmake_minimum_required(VERSION 3.10)

# customization of project
set(CMAKE_PROGRAMMER "USBtiny")
set(CMAKE_MICROCHIP "atmega48p")
set(CMAKE_PORT "usb")
set(FREQUENCY "1200000") # Hz
# custom fuses  
set(CMAKE_E_FUSE "0xFF") #default FF
set(CMAKE_H_FUSE "0xD9") #default D9
set(CMAKE_L_FUSE "0xE1") #default E1

set(CMAKE_C_COMPILER "avr-gcc")
set(CMAKE_C_STANDARD "11")
set(CMAKE_CXX_COMPILER "avr-g++")
set(CMAKE_CXX_STANDARD "14")
set(CMAKE_OBJCOPY "avr-objcopy")
set(CMAKE_AVRDUDE "avrdude")

project("template")

add_compile_options(
  "-Os"
  "-mmcu=${CMAKE_MICROCHIP}"
)
add_compile_definitions(
  "F_CPU=${FREQUENCY}UL"
)

set(SOURCES "src/main.c")

add_executable(${PROJECT_NAME}
               ${SOURCES})

add_custom_target(download
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_AVRDUDE} -p ${CMAKE_MICROCHIP} -c ${CMAKE_PROGRAMMER} -P${CMAKE_PORT} -Uflash:w:./${PROJECT_NAME}.hex:i
        )
add_custom_target(BURN_FUSE
        COMMAND ${CMAKE_AVRDUDE} -p ${CMAKE_MICROCHIP} -c ${CMAKE_PROGRAMMER} -P${CMAKE_PORT} -Ulfuse:w:${CMAKE_L_FUSE}:m
        COMMAND ${CMAKE_AVRDUDE} -p ${CMAKE_MICROCHIP} -c ${CMAKE_PROGRAMMER} -P${CMAKE_PORT} -Uhfuse:w:${CMAKE_H_FUSE}:m
        COMMAND ${CMAKE_AVRDUDE} -p ${CMAKE_MICROCHIP} -c ${CMAKE_PROGRAMMER} -P${CMAKE_PORT} -Uefuse:w:${CMAKE_E_FUSE}:m
)
