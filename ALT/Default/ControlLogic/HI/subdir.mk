################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ControlLogic/HI/CameraControl.cpp \
../ControlLogic/HI/HorizontalAxis.cpp \
../ControlLogic/HI/PicManager.cpp \
../ControlLogic/HI/VerticalAxis.cpp 

OBJS += \
./ControlLogic/HI/CameraControl.o \
./ControlLogic/HI/HorizontalAxis.o \
./ControlLogic/HI/PicManager.o \
./ControlLogic/HI/VerticalAxis.o 

CPP_DEPS += \
./ControlLogic/HI/CameraControl.d \
./ControlLogic/HI/HorizontalAxis.d \
./ControlLogic/HI/PicManager.d \
./ControlLogic/HI/VerticalAxis.d 


# Each subdirectory must supply rules for building sources it contributes
ControlLogic/HI/%.o: ../ControlLogic/HI/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/opt/local/include -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


