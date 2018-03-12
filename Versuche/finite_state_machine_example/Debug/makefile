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
finite_state_machine_example.elf.size \

ELFCHECK += \
finite_state_machine_example.elf.elfcheck \


# All Target
all: finite_state_machine_example.elf secondary-outputs

# Tool invocations
finite_state_machine_example.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo Building target: $@
	@echo Invoking: MicroBlaze gcc linker
	mb-gcc -L../../general_bsp/microblaze_0/lib -L/DIST/lehre/lab_mps/pub/libmps/lib -Wl,-T -Wl,../src/lscript.ld -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.10.a -mno-xl-soft-mul -o"finite_state_machine_example.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo Finished building target: $@
	@echo ' '

finite_state_machine_example.elf.size: finite_state_machine_example.elf
	@echo Invoking: MicroBlaze Print Size
	mb-size finite_state_machine_example.elf  |tee "finite_state_machine_example.elf.size"
	@echo Finished building: $@
	@echo ' '

finite_state_machine_example.elf.elfcheck: finite_state_machine_example.elf
	@echo Invoking: Xilinx ELF Check
	elfcheck finite_state_machine_example.elf -hw ../../hwplattform/system.xml -pe microblaze_0  |tee "finite_state_machine_example.elf.elfcheck"
	@echo Finished building: $@
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(ELFCHECK)$(OBJS)$(S_UPPER_DEPS)$(C_DEPS)$(ELFSIZE) finite_state_machine_example.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE) $(ELFCHECK)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
