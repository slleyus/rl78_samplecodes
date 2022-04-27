################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../AR_LIB/AR_LIB_PORT.c \
../AR_LIB/AR_LIB_TIME.c 

COMPILER_OBJS += \
AR_LIB/AR_LIB_PORT.obj \
AR_LIB/AR_LIB_TIME.obj 

C_DEPS += \
AR_LIB/AR_LIB_PORT.d \
AR_LIB/AR_LIB_TIME.d 

# Each subdirectory must supply rules for building sources it contributes
AR_LIB/%.obj: ../AR_LIB/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo AR_LIB\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "AR_LIB\cDepSubCommand.tmp"
	ccrl -subcommand="AR_LIB\cDepSubCommand.tmp" -o "$(@:%.obj=%.d)"  -MT="$(@:%.obj=%.obj)"  -MT="$(@:%.obj=%.d)" -msg_lang=japanese "$<"
	@echo AR_LIB\cSubCommand.tmp=
	@sed -e "s/^/ /" "AR_LIB\cSubCommand.tmp"
	ccrl -subcommand="AR_LIB\cSubCommand.tmp" -msg_lang=japanese -o "$(@:%.d=%.obj)" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

