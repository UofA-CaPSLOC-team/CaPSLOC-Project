################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Preprocessor/tinyxml/tinystr.cpp \
../Preprocessor/tinyxml/tinyxml.cpp \
../Preprocessor/tinyxml/tinyxmlerror.cpp \
../Preprocessor/tinyxml/tinyxmlparser.cpp \
../Preprocessor/tinyxml/xmltest.cpp 

OBJS += \
./Preprocessor/tinyxml/tinystr.o \
./Preprocessor/tinyxml/tinyxml.o \
./Preprocessor/tinyxml/tinyxmlerror.o \
./Preprocessor/tinyxml/tinyxmlparser.o \
./Preprocessor/tinyxml/xmltest.o 

CPP_DEPS += \
./Preprocessor/tinyxml/tinystr.d \
./Preprocessor/tinyxml/tinyxml.d \
./Preprocessor/tinyxml/tinyxmlerror.d \
./Preprocessor/tinyxml/tinyxmlparser.d \
./Preprocessor/tinyxml/xmltest.d 


# Each subdirectory must supply rules for building sources it contributes
Preprocessor/tinyxml/%.o: ../Preprocessor/tinyxml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


