################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ControlLogic/MCPM/MCPM.cpp 

OBJS += \
./ControlLogic/MCPM/MCPM.o 

CPP_DEPS += \
./ControlLogic/MCPM/MCPM.d 


# Each subdirectory must supply rules for building sources it contributes
ControlLogic/MCPM/%.o: ../ControlLogic/MCPM/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


