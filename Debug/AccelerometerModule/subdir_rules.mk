################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
AccelerometerModule/%.obj: ../AccelerometerModule/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/loowe/workspace_v9/ultrasonic_project" --include_path="C:/ti/ccs930/ccs/ccs_base/arm/include/" --include_path="C:/ti/ccs930/ccs/ccs_base/arm/include/CMSIS" --include_path="C:/Users/loowe/Desktop/University/Modules/ICT 2104/Documents/msp432_driverlib_3_21_00_05/driverlib/MSP432P4xx" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/include/" --include_path="C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="AccelerometerModule/$(basename $(<F)).d_raw" --obj_directory="AccelerometerModule" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


