################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Hello_world.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c 

OBJS += \
./src/Hello_world.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o 

C_DEPS += \
./src/Hello_world.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"C:\Users\Dragon\Documents\LPCXpresso_7.4.0_229\workspace\CMSIS_CORE_LPC17xx\inc" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


