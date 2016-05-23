################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SSP_Test.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/extint.c \
../src/ssp.c 

OBJS += \
./src/SSP_Test.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/extint.o \
./src/ssp.o 

C_DEPS += \
./src/SSP_Test.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/extint.d \
./src/ssp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -I"C:\Users\nsarras\Documents\LPCXpresso_8.0.0_526\workspace\CMSIS_CORE_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


