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
# Project name
project("Project")

## AVR Chip Configuration
# 8Mhz, this should match the crystal on your board,
# I use 8Mhz and 3.3V for the lowest power consumption
set(F_CPU 8000000UL)
# CPU, you can find the list here:
# https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html
set(MCU atmega2560)
# Default Baudrate for UART, read avr include/util/setbaud.h for usage
set(BAUD 9600)
# The programmer to use, read avrdude manual for list
set(PROG_TYPE usbtiny)

# AVR Fuses, must be in concordance with your hardware and F_CPU
# http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega8
set(E_FUSE 0xff)
set(H_FUSE 0x99)
set(L_FUSE 0xc1)
set(LOCK_BIT 0xff)


# Use AVR GCC toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)


# Pass defines to compiler
add_definitions(
    -DF_CPU=${F_CPU}
    -DBAUD=${BAUD}
)
# mmcu MUST be passed to bot the compiler and linker, this handle the linker
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

add_compile_options(
    -mmcu=${MCU} # MCU
    -Os # optimize
    -Wall # enable warnings
    -Wno-main
    -Wundef
    -pedantic
    # -Wstrict-prototypes
    -Werror
    -Wfatal-errors
    -Wl,--relax,--gc-sections
    -g
    -gdwarf-2
    -funsigned-char # a few optimizations
    -funsigned-bitfields
    -fpack-struct
    -fshort-enums
    -ffunction-sections
    -fdata-sections
    -fno-split-wide-types
    -fno-tree-scev-cprop
)
file(GLOB SRC_FILES "src/*.c") # Load all files in src folder

# Create one target
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Rename the output to .elf as we will create multiple files
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.${MCU}.elf)

# Strip binary for upload
add_custom_target(strip ALL avr-strip ${PROJECT_NAME}.${MCU}.elf DEPENDS ${PROJECT_NAME})

# Transform binary into hex file, we ignore the eeprom segments in the step
add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${PROJECT_NAME}.${MCU}.elf ${PROJECT_NAME}.${MCU}.hex DEPENDS strip)
# Transform binary into hex file, this is the eeprom part (empty if you don't
# use eeprom static variables)
add_custom_target(eeprom avr-objcopy -j .eeprom  --set-section-flags=.eeprom=alloc,load  --no-change-warnings --change-section-lma .eeprom=0 -O ihex ${PROJECT_NAME}.${MCU}.elf ${PROJECT_NAME}.${MCU}.eep DEPENDS strip)

# Upload the firmware with avrdude
add_custom_target(upload avrdude -v -c ${PROG_TYPE} -p ${MCU} -U flash:w:${PROJECT_NAME}.${MCU}.hex DEPENDS hex)

# Upload the eeprom with avrdude
add_custom_target(upload_eeprom avrdude  -v -c ${PROG_TYPE} -p ${MCU}  -U eeprom:w:${PROJECT_NAME}.${MCU}.eep DEPENDS eeprom)

# Burn fuses
add_custom_target(fuses avrdude -v -c ${PROG_TYPE} -p ${MCU}  -U lfuse:w:${L_FUSE}:m -U hfuse:w:${H_FUSE}:m -U efuse:w:${E_FUSE}:m -U lock:w:${LOCK_BIT}:m )

# Clean extra files
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.${MCU}.hex;${PROJECT_NAME}.${MCU}.eeprom;${PROJECT_NAME}.${MCU}.lst")