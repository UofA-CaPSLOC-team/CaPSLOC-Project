/*
 * L3GD20.cpp
 *
 *  Created on: Feb 17, 2014
 *      Author: curtis
 */

#include "L3GD20.h"

using namespace std;

L3GD20::L3GD20()
{
	_i2cAddress = _defaultI2cAddress;
	_i2cHandle = wiringPiI2CSetup(_i2cAddress);
	//Turns X, Y, Z axis on.  95hz data rate.  12.5 cutt-off.
	wiringPiI2CWriteReg8(_i2cHandle, CTRL_REG1, 0x0F);
	//Enable High Pass Filter.  
	wiringPiI2CWriteReg8(_i2cHandle, CTRL_REG2, 0x20);

	setSensitivity(_250dps);
	calibrate();
}

void L3GD20::calibrate()
{
	float xTotal = 0.0;
	float yTotal = 0.0;
	float zTotal = 0.0;
	for (int i=0; i<1000; i++)
	{
		xTotal += getXAxisRotationSpeed();
		yTotal += getYAxisRotationSpeed();
		zTotal += getZAxisRotationSpeed();
		usleep(500);
	}
	_xAxisOffset = xTotal/1000;
	_yAxisOffset = yTotal/1000;
	_zAxisOffset = zTotal/1000;
}

float L3GD20::getFullScaleDegreesPerSecond()
{
	float rVal = 0.0;
	switch(_sensitivity)
	{
	case _250dps:
		rVal = 0.00875;
		break;
	case _500dps:
		rVal = 0.01750;
		break;
	case _2000dps:
		rVal = 0.070;
		break;
	}
	return rVal;
}

bool L3GD20::isI2CHandleValid()
{
	bool rVal = false;
	if (_i2cHandle > 0)
	{
		rVal = true;
	}
	return rVal;
}

bool L3GD20::isXAxisEnabled()
{
	bool rVal = false;
	uint8_t controlRegister = wiringPiI2CReadReg8(_i2cHandle, CTRL_REG1);
	if ((controlRegister & 0x01))
	{
		rVal = true;
	}
	return rVal;
}

bool L3GD20::isYAxisEnabled()
{
	bool rVal = false;
	uint8_t controlRegister = wiringPiI2CReadReg8(_i2cHandle, CTRL_REG1);
	if ((controlRegister & 0x02))
	{
		rVal = true;
	}
	return rVal;
}

bool L3GD20::isZAxisEnabled()
{
	bool rVal = false;
	uint8_t controlRegister = wiringPiI2CReadReg8(_i2cHandle, CTRL_REG1);
	if ((controlRegister && 0x04))
	{
		rVal = true;
	}
	return rVal;
}

bool L3GD20::isGyroscopeOn()
{
	bool rVal = false;
	uint8_t controlRegister = wiringPiI2CReadReg8(_i2cHandle, CTRL_REG1);
	if ((controlRegister && 0x08))
	{
		rVal = true;
	}
	return rVal;
}

uint8_t L3GD20::getI2CAddress()
{
	return _i2cAddress;
}

L3GD20::FullScale L3GD20::getSensitivity()
{
	return _sensitivity;
}

void L3GD20::setSensitivity(FullScale fullScale)
{
	uint8_t reg4 = wiringPiI2CReadReg8(_i2cHandle, CTRL_REG4);
	uint8_t writeBack = (reg4 | (fullScale << 4));
	wiringPiI2CWriteReg8(_i2cHandle, CTRL_REG4, writeBack);

	uint8_t readBack = wiringPiI2CReadReg8(_i2cHandle, CTRL_REG4);
	_sensitivity = (FullScale)((0x30 & readBack)>>4);
}

uint16_t L3GD20::getXAxisRaw()
{
	uint16_t rVal = 0;
	uint8_t lower = wiringPiI2CReadReg8(_i2cHandle, OUT_X_L);
	uint8_t upper = wiringPiI2CReadReg8(_i2cHandle, OUT_X_H);
	rVal = ( lower | (upper << 8));
	return rVal;
}

uint16_t L3GD20::getYAxisRaw()
{
	uint16_t rVal = 0;
	uint8_t lower = wiringPiI2CReadReg8(_i2cHandle, OUT_Y_L);
	uint8_t upper = wiringPiI2CReadReg8(_i2cHandle, OUT_Y_H);
	rVal = (lower | (upper << 8));
	return rVal;
}

uint16_t L3GD20::getZAxisRaw()
{
	uint16_t rVal = 0;
	uint8_t lower = wiringPiI2CReadReg8(_i2cHandle, OUT_Z_L);
	uint8_t upper = wiringPiI2CReadReg8(_i2cHandle, OUT_Z_H);
	rVal = ( lower | (upper << 8));
	return rVal;
}

float L3GD20::getXAxisRotationSpeed()
{
	float rVal = 0.0;
	int16_t rawXAxis = getXAxisRaw();
	rVal = (float(rawXAxis) * 250.0 / 32768.0)- _xAxisOffset;
	return rVal;
}

float L3GD20::getYAxisRotationSpeed()
{
	float rVal = 0.0;
	int16_t rawYAxis = getYAxisRaw();
	rVal = (float(rawYAxis) * 250.0 / 32768.0) - _yAxisOffset;
	return rVal;
}

float L3GD20::getZAxisRotationSpeed()
{
	float rVal = 0.0;
	int16_t  rawZAxis = getZAxisRaw();
	rVal = (float(rawZAxis) * 250.0 / 32768.0) - _zAxisOffset;
	return rVal;
}


