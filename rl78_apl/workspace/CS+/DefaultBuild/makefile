################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp *.h *.bls *.libelf *.rel *.lbk *.abs *.prn *.mi *.fsy 1st/*.abs 1st/*.fsy

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include sketch/subdir.mk
-include AR_LIB/subdir.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif 
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif 
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif 
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif 
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif 
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif 
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif 
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif 
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif 
endif

ifneq ($(strip $(SRC_UPPER_SRCS)),)
ALL_ASMS += $(SRC_UPPER_SRCS) 
endif
ifneq ($(strip $(SRC_SRCS)),)
ALL_ASMS += $(SRC_SRCS) 
endif
ifneq ($(strip $(S_UPPER_SRCS)),)
ALL_ASMS += $(S_UPPER_SRCS) 
endif
ifneq ($(strip $(S_SRCS)),)
ALL_ASMS += $(S_SRCS) 
endif


-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables
LINKER_OUTPUT_OUTPUTS += \
AR_DIO_2.abs

CONVERTOR_OUTPUT_OUTPUTS += \
AR_DIO_2.mot

OBJS := \
$(COMPILER_OBJS) \
$(INDIVIDUAL_COMPILER_OBJS) \
$(ASSEMBLER_OBJS) \
$(CUSTOM_OBJS)

# All Target
all: $(CONVERTOR_OUTPUT_OUTPUTS) AR_DIO_2.x

	@echo 'Build complete.'

AR_DIO_2.mot: $(LINKER_OUTPUT_OUTPUTS) 
	@echo 'Invoking: Converter'
	@echo 'Building target:'
	@echo ConverterAR_DIO_2.tmp=
	@sed -e "s/^/    /" "ConverterAR_DIO_2.tmp"
	rlink   "AR_DIO_2.abs" -subcommand="ConverterAR_DIO_2.tmp"
	@echo 'Finished building target:'
	@echo.


AR_DIO_2.abs: $(OBJS)  $(ALL_ASMS)
	@echo 'Invoking: Linker'
	@echo LinkerAR_DIO_2.tmp=
	@sed -e "s/^/    /" "LinkerAR_DIO_2.tmp"
	rlink -subcommand="LinkerAR_DIO_2.tmp"
	@echo 'Finished building:'
	@echo.


AR_DIO_2.x: AR_DIO_2.abs
		-C:\Renesas\e2_studio\Utilities\\ccrl\renesas_cc_converter.exe AR_DIO_2.abs AR_DIO_2.x

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS) $(CONVERTOR_OUTPUT_OUTPUTS)
	-rm -rf /*.obj /*.d /*.asm /*.p /*.pp /*.src /*.jmp /*.lst /*.prn /*.i 
	-rm -rf sketch/*.obj sketch/*.d sketch/*.asm sketch/*.p sketch/*.pp sketch/*.src sketch/*.jmp sketch/*.lst sketch/*.prn sketch/*.i 
	-rm -rf AR_LIB/*.obj AR_LIB/*.d AR_LIB/*.asm AR_LIB/*.p AR_LIB/*.pp AR_LIB/*.src AR_LIB/*.jmp AR_LIB/*.lst AR_LIB/*.prn AR_LIB/*.i 
	@echo
	@echo 'Clean complete.'

.PHONY: all clean 
-include ../makefile.targets