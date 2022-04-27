################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../sketch/AR_SKETCH.c 

COMPILER_OBJS += \
sketch/AR_SKETCH.obj 

C_DEPS += \
sketch/AR_SKETCH.d 

# Each subdirectory must supply rules for building sources it contributes
sketch/%.obj: ../sketch/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo sketch\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "sketch\cDepSubCommand.tmp"
	ccrl -subcommand="sketch\cDepSubCommand.tmp" -o "$(@:%.obj=%.d)"  -MT="$(@:%.obj=%.obj)"  -MT="$(@:%.obj=%.d)" -msg_lang=japanese "$<"
	@echo sketch\cSubCommand.tmp=
	@sed -e "s/^/ /" "sketch\cSubCommand.tmp"
	ccrl -subcommand="sketch\cSubCommand.tmp" -msg_lang=japanese -o "$(@:%.d=%.obj)" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

