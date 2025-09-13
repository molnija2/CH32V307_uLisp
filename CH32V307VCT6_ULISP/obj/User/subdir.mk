################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v30x_it.c \
../User/diskio.c \
../User/ff.c \
../User/ffsystem.c \
../User/ffunicode.c \
../User/system_ch32v30x.c 

CPP_SRCS += \
../User/board_pins.cpp \
../User/filesys.cpp \
../User/main.cpp \
../User/rtc.cpp \
../User/ulisp-extensions.cpp \
../User/ulisp.cpp 

OBJS += \
./User/board_pins.o \
./User/ch32v30x_it.o \
./User/diskio.o \
./User/ff.o \
./User/ffsystem.o \
./User/ffunicode.o \
./User/filesys.o \
./User/main.o \
./User/rtc.o \
./User/system_ch32v30x.o \
./User/ulisp-extensions.o \
./User/ulisp.o 

C_DEPS += \
./User/ch32v30x_it.d \
./User/diskio.d \
./User/ff.d \
./User/ffsystem.d \
./User/ffunicode.d \
./User/system_ch32v30x.d 

CPP_DEPS += \
./User/board_pins.d \
./User/filesys.d \
./User/main.d \
./User/rtc.d \
./User/ulisp-extensions.d \
./User/ulisp.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C++ Compiler'
	riscv-none-embed-g++ -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"/home/sap/Projects/CH32/Projects/CH32V307VCT6_ULISP/Debug" -I"/home/sap/Projects/CH32/Projects/CH32V307VCT6_ULISP/Core" -I"/home/sap/Projects/CH32/Projects/CH32V307VCT6_ULISP/User" -I"/home/sap/Projects/CH32/Projects/CH32V307VCT6_ULISP/Peripheral/inc" -std=gnu++11 -fabi-version=0 -Os -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

User/%.o: ../User/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"/home/sap/Projects/CH32/Projects/CH32V307VCT6_ULISP/Debug" -I"/home/sap/Projects/CH32/Projects/CH32V307VCT6_ULISP/Core" -I"/home/sap/Projects/CH32/Projects/CH32V307VCT6_ULISP/User" -I"/home/sap/Projects/CH32/Projects/CH32V307VCT6_ULISP/Peripheral/inc" -std=gnu99 -Os -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


