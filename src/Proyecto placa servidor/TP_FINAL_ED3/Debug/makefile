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
all: TP_FINAL_ED3.axf

# Tool invocations
TP_FINAL_ED3.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Ezequiel\Documents\MCUXpressoIDE_10.2.1_795\workspace\CMSIS_CORE_LPC17xx\Debug" -Xlinker -Map="TP_FINAL_ED3.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "TP_FINAL_ED3_Debug.ld" -o "TP_FINAL_ED3.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) TP_FINAL_ED3.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "TP_FINAL_ED3.axf"; # arm-none-eabi-objcopy -v -O binary "TP_FINAL_ED3.axf" "TP_FINAL_ED3.bin" ; # checksum -p LPC1769 -d "TP_FINAL_ED3.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
