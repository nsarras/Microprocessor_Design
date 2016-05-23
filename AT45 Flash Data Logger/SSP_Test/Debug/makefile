################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SSP_Test.axf

# Tool invocations
SSP_Test.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\nsarras\Documents\LPCXpresso_8.0.0_526\workspace\CMSIS_CORE_LPC17xx\Debug" -Xlinker -Map="SSP_Test.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "SSP_Test_Debug.ld" -o "SSP_Test.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) SSP_Test.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "SSP_Test.axf"; # arm-none-eabi-objcopy -v -O binary "SSP_Test.axf" "SSP_Test.bin" ; # checksum -p LPC1769 -d "SSP_Test.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
