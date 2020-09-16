################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/TP_FINAL_ED3.c \
../src/activacion.c \
../src/configuracion.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/lcd.c \
../src/leds.c \
../src/simon.c 

OBJS += \
./src/TP_FINAL_ED3.o \
./src/activacion.o \
./src/configuracion.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/lcd.o \
./src/leds.o \
./src/simon.o 

C_DEPS += \
./src/TP_FINAL_ED3.d \
./src/activacion.d \
./src/configuracion.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/lcd.d \
./src/leds.d \
./src/simon.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"C:\Users\Ezequiel\Documents\MCUXpressoIDE_10.2.1_795\workspace\CMSIS_CORE_LPC17xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


