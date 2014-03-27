################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DPU/SendToCTRL.cpp 

OBJS += \
./DPU/SendToCTRL.o 

CPP_DEPS += \
./DPU/SendToCTRL.d 


# Each subdirectory must supply rules for building sources it contributes
DPU/%.o: ../DPU/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/opt/local/include -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


