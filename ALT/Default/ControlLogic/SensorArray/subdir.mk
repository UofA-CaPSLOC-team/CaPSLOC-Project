################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ControlLogic/SensorArray/FloatAverage.cpp \
../ControlLogic/SensorArray/GGA.cpp \
../ControlLogic/SensorArray/GPSManager.cpp \
../ControlLogic/SensorArray/L3GD20.cpp \
../ControlLogic/SensorArray/LPS331AP.cpp \
../ControlLogic/SensorArray/LSM303.cpp \
../ControlLogic/SensorArray/NSS.cpp \
../ControlLogic/SensorArray/SensorManager.cpp 

OBJS += \
./ControlLogic/SensorArray/FloatAverage.o \
./ControlLogic/SensorArray/GGA.o \
./ControlLogic/SensorArray/GPSManager.o \
./ControlLogic/SensorArray/L3GD20.o \
./ControlLogic/SensorArray/LPS331AP.o \
./ControlLogic/SensorArray/LSM303.o \
./ControlLogic/SensorArray/NSS.o \
./ControlLogic/SensorArray/SensorManager.o 

CPP_DEPS += \
./ControlLogic/SensorArray/FloatAverage.d \
./ControlLogic/SensorArray/GGA.d \
./ControlLogic/SensorArray/GPSManager.d \
./ControlLogic/SensorArray/L3GD20.d \
./ControlLogic/SensorArray/LPS331AP.d \
./ControlLogic/SensorArray/LSM303.d \
./ControlLogic/SensorArray/NSS.d \
./ControlLogic/SensorArray/SensorManager.d 


# Each subdirectory must supply rules for building sources it contributes
ControlLogic/SensorArray/%.o: ../ControlLogic/SensorArray/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/opt/local/include -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


