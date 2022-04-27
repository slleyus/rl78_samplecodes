################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
ASM_SRCS += \
../cstart.asm \
../stkinit.asm 

C_SRCS += \
../r_cg_cgc.c \
../r_cg_timer.c \
../r_main.c \
../r_systeminit.c 

ASSEMBLER_OBJS += \
cstart.obj \
stkinit.obj 

ASM_DEPS += \
cstart.d \
stkinit.d 

COMPILER_OBJS += \
r_cg_cgc.obj \
r_cg_timer.obj \
r_main.obj \
r_systeminit.obj 

C_DEPS += \
r_cg_cgc.d \
r_cg_timer.d \
r_main.d \
r_systeminit.d 

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.asm 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Assembler'
	@echo asmDepSubCommand.tmp=
	@sed -e "s/^/ /" "asmDepSubCommand.tmp"
	ccrl -subcommand="asmDepSubCommand.tmp" -asmopt=-MF="$(@:%.obj=%.d)" -asmopt=-MT="$(@:%.obj=%.obj)" -asmopt=-MT="$(@:%.obj=%.d)" -msg_lang=japanese "$<"
	@echo asmSubCommand.tmp=
	@sed -e "s/^/ /" "asmSubCommand.tmp"
	ccrl -subcommand="asmSubCommand.tmp" -msg_lang=japanese -o "$(@:%.d=%.obj)" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

%.obj: ../%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo cDepSubCommand.tmp=
	@sed -e "s/^/ /" "cDepSubCommand.tmp"
	ccrl -subcommand="cDepSubCommand.tmp" -o "$(@:%.obj=%.d)"  -MT="$(@:%.obj=%.obj)"  -MT="$(@:%.obj=%.d)" -msg_lang=japanese "$<"
	@echo cSubCommand.tmp=
	@sed -e "s/^/ /" "cSubCommand.tmp"
	ccrl -subcommand="cSubCommand.tmp" -msg_lang=japanese -o "$(@:%.d=%.obj)" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

