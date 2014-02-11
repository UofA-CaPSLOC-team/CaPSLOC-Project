################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Preprocessor/InProc/CommandID.cpp \
../Preprocessor/InProc/CommandParse.cpp 

OBJS += \
./Preprocessor/InProc/CommandID.o \
./Preprocessor/InProc/CommandParse.o 

CPP_DEPS += \
./Preprocessor/InProc/CommandID.d \
./Preprocessor/InProc/CommandParse.d 


# Each subdirectory must supply rules for building sources it contributes
Preprocessor/InProc/%.o: ../Preprocessor/InProc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


