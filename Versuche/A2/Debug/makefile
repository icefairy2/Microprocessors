################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
A2.elf.size \

ELFCHECK += \
A2.elf.elfcheck \


# All Target
all: A2.elf secondary-outputs

# Tool invocations
A2.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo Building target: $@
	@echo Invoking: MicroBlaze gcc linker
	mb-gcc -L../../general_bsp/microblaze_0/lib -L/DIST/lehre/lab_mps/pub/libmps/lib -Wl,-T -Wl,../src/lscript.ld -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.10.a -mno-xl-soft-mul -o"A2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@
	@echo ' '

A2.elf.size: A2.elf
	@echo Invoking: MicroBlaze Print Size
	mb-size A2.elf  |tee "A2.elf.size"
	@echo Finished building: $@
	@echo ' '

A2.elf.elfcheck: A2.elf
	@echo Invoking: Xilinx ELF Check
	elfcheck A2.elf -hw ../../hwplattform/system.xml -pe microblaze_0  |tee "A2.elf.elfcheck"
	@echo Finished building: $@
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(ELFCHECK)$(OBJS)$(S_UPPER_DEPS)$(C_DEPS)$(ELFSIZE) A2.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
