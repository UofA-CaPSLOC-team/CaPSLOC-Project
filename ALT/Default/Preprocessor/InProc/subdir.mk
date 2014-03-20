################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Preprocessor/InProc/BoostParse.cpp \
../Preprocessor/InProc/CommandList.cpp \
../Preprocessor/InProc/CommandNode.cpp \
../Preprocessor/InProc/Config.cpp 

OBJS += \
./Preprocessor/InProc/BoostParse.o \
./Preprocessor/InProc/CommandList.o \
./Preprocessor/InProc/CommandNode.o \
./Preprocessor/InProc/Config.o 

CPP_DEPS += \
./Preprocessor/InProc/BoostParse.d \
./Preprocessor/InProc/CommandList.d \
./Preprocessor/InProc/CommandNode.d \
./Preprocessor/InProc/Config.d 


# Each subdirectory must supply rules for building sources it contributes
Preprocessor/InProc/%.o: ../Preprocessor/InProc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/opt/local/include -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


