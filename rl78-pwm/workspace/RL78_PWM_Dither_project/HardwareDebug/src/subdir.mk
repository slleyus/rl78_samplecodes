################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
..\src/cstart.asm \
..\src/stkinit.asm 

C_SRCS += \
..\src/pwm_dithering.c \
..\src/pwmout_to_input_adc.c \
..\src/r_cg_adc.c \
..\src/r_cg_adc_user.c \
..\src/r_cg_cgc.c \
..\src/r_cg_cgc_user.c \
..\src/r_cg_dtc.c \
..\src/r_cg_dtc_user.c \
..\src/r_cg_it.c \
..\src/r_cg_it_user.c \
..\src/r_cg_port.c \
..\src/r_cg_port_user.c \
..\src/r_cg_serial.c \
..\src/r_cg_serial_user.c \
..\src/r_cg_timer.c \
..\src/r_cg_timer_user.c \
..\src/r_main.c \
..\src/r_systeminit.c \
..\src/utility.c 

C_DEPS += \
./src/pwm_dithering.d \
./src/pwmout_to_input_adc.d \
./src/r_cg_adc.d \
./src/r_cg_adc_user.d \
./src/r_cg_cgc.d \
./src/r_cg_cgc_user.d \
./src/r_cg_dtc.d \
./src/r_cg_dtc_user.d \
./src/r_cg_it.d \
./src/r_cg_it_user.d \
./src/r_cg_port.d \
./src/r_cg_port_user.d \
./src/r_cg_serial.d \
./src/r_cg_serial_user.d \
./src/r_cg_timer.d \
./src/r_cg_timer_user.d \
./src/r_main.d \
./src/r_systeminit.d \
./src/utility.d 

OBJS += \
./src/cstart.obj \
./src/pwm_dithering.obj \
./src/pwmout_to_input_adc.obj \
./src/r_cg_adc.obj \
./src/r_cg_adc_user.obj \
./src/r_cg_cgc.obj \
./src/r_cg_cgc_user.obj \
./src/r_cg_dtc.obj \
./src/r_cg_dtc_user.obj \
./src/r_cg_it.obj \
./src/r_cg_it_user.obj \
./src/r_cg_port.obj \
./src/r_cg_port_user.obj \
./src/r_cg_serial.obj \
./src/r_cg_serial_user.obj \
./src/r_cg_timer.obj \
./src/r_cg_timer_user.obj \
./src/r_main.obj \
./src/r_systeminit.obj \
./src/stkinit.obj \
./src/utility.obj 

ASM_DEPS += \
./src/cstart.d \
./src/stkinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.asm
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Assembler'
	ccrl  -asmopt=-MM -asmopt=-MP -asmopt=-MF="$(@:%.obj=%.d)" -asmopt=-MT="$(@:%.obj=%.obj)" -asmopt=-MT="$(@:%.obj=%.d)" -o "$(@:%.d=%.obj)" -cpu=S3 -c -dev="C:/Renesas/e2_studio_5_4_0_015/DebugComp/RL78\Common\DR5F104PJ.DVF"  -msg_lang=english -g   "$<"
	ccrl -o "$(@:%.d=%.obj)" -cpu=S3 -c -dev="C:/Renesas/e2_studio_5_4_0_015/DebugComp/RL78\Common\DR5F104PJ.DVF"  -msg_lang=english -g   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

src/%.obj: ../src/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrl  -MM -MP -o "$(@:%.obj=%.d)" -MT="$(@:%.obj=%.obj)" -MT="$(@:%.obj=%.d)"  -cpu=S3 -dev="C:/Renesas/e2_studio_5_4_0_015/DebugComp/RL78\Common\DR5F104PJ.DVF"  -msg_lang=english -I "C:\PROGRA~2\Renesas\RL78\1_5_0/inc" -I "C:\workspace_g\Project_Folder\RL78_PWM_Dither_project\src\inc" -g -Onothing -Odelete_static_func=off -Otail_call=off   "$<"
	ccrl -o "$(@:%.d=%.obj)" -cpu=S3 -c -dev="C:/Renesas/e2_studio_5_4_0_015/DebugComp/RL78\Common\DR5F104PJ.DVF"  -msg_lang=english -I "C:\PROGRA~2\Renesas\RL78\1_5_0/inc" -I "C:\workspace_g\Project_Folder\RL78_PWM_Dither_project\src\inc" -g -Onothing -Odelete_static_func=off -Otail_call=off   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

