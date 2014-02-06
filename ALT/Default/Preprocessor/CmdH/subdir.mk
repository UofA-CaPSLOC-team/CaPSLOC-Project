################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Preprocessor/CmdH/CommandHandler.cpp 

OBJS += \
./Preprocessor/CmdH/CommandHandler.o 

CPP_DEPS += \
./Preprocessor/CmdH/CommandHandler.d 


# Each subdirectory must supply rules for building sources it contributes
Preprocessor/CmdH/%.o: ../Preprocessor/CmdH/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


