/*
 * LSM303DLHC.cpp
 *
 *  Created on: Feb 17, 2014
 *      Author: curtis
 */

#include "LSM303.h"
#include <cmath>
#include <iostream>

LSM303::LSM303()
{
	_accI2CHandle = wiringPiI2CSetup(_accDefaultI2CAddress);
	_magI2CHandle = wiringPiI2CSetup(_magDefaultI2CAddress);

	//Turn on Accelerometer, 10hz, all axis enabled.
	wiringPiI2CWriteReg8(_accI2CHandle, CTRL_REG1_A, 0x7F);
	//Continous update, little endian, +-2G, high res enabled.
	wiringPiI2CWriteReg8(_accI2CHandle, CTRL_REG4_A, 0x04);
	//TODO: remove magic number.
	_accFullScale = _2g;

	_accxAxisOffset = 0.254312;
	_accyAxisOffset = 0.336203;
	_acczAxisOffset = -0.493141;

	//calibrate();

	//Temp enable, 3.0hz out on the gyro.
	wiringPiI2CWriteReg8( _magI2CHandle, CRA_REG_M, 0x9C);//0b1001 1100);

	//Gain configuration set to +- 1.3
	wiringPiI2CWriteReg8( _magI2CHandle, CRB_REG_M, 0x20);//0b0010 0000);

	//Mode selection set to continuous
	wiringPiI2CWriteReg8( _magI2CHandle, MR_REG_M, 0x00);
}

void LSM303::calibrate()
{

	float accx;
	float accy;
	float accz;
	for (int i=0; i<1000; i++)
	{
		accx += accGetXAxisG();
		accy += accGetYAxisG();
		accz += accGetZAxisG();
	}

	_accxAxisOffset = accx/1000;
	_accyAxisOffset = accy/1000;
	_acczAxisOffset = 1 - fabs((accz/1000));

	std::cout << _accxAxisOffset << " " << _accyAxisOffset << " " << _acczAxisOffset << std::endl;
}

uint8_t LSM303::accGetI2CAddress()
{
	return _accDefaultI2CAddress;
}

int16_t LSM303::accGetXAxisRaw()
{
	int16_t rVal = 0;
	uint8_t lower = wiringPiI2CReadReg8(_accI2CHandle, OUT_X_L_A);
	uint8_t upper = wiringPiI2CReadReg8(_accI2CHandle, OUT_X_H_A);
	rVal = ((upper << 8) | lower);
	return rVal;
}

int16_t LSM303::accGetYAxisRaw()
{
	int16_t rVal = 0;
	uint8_t lower = wiringPiI2CReadReg8(_accI2CHandle, OUT_Y_L_A);
	uint8_t upper = wiringPiI2CReadReg8(_accI2CHandle, OUT_Y_H_A);
	rVal = ((upper << 8) | lower);
	return rVal;
}

int16_t LSM303::accGetZAxisRaw()
{
	int16_t rVal = 0;
	uint8_t lower = wiringPiI2CReadReg8(_accI2CHandle, OUT_Z_L_A);
	uint8_t upper = wiringPiI2CReadReg8(_accI2CHandle, OUT_Z_H_A);
	rVal = ((upper << 8) | lower);
	return rVal;
}

float LSM303::accGetXAxisG()
{
	float rVal = 0.0;
	int16_t rawXAxis = accGetXAxisRaw();
	rVal = ((float)rawXAxis * 2.0 / 32768.0) - _accxAxisOffset;
	return rVal;
}

float LSM303::accGetYAxisG()
{
	float rVal = 0.0;
	int16_t rawYAxis = accGetYAxisRaw();
	rVal = ((float)rawYAxis * 2.0 / 32768.0) - _accyAxisOffset;
	return rVal;
}

float LSM303::accGetZAxisG()
{
	float rVal = 0.0;
	int16_t rawZAxis = accGetZAxisRaw();
	rVal = ((float)rawZAxis * 2.0 / 32768.0) - _acczAxisOffset;
	return rVal;
}

uint8_t LSM303::magGetI2CAddress()
{
	return _magDefaultI2CAddress;
}

int16_t LSM303::magGetXAxisRaw()
{
	int16_t rVal;
	uint8_t lower = wiringPiI2CReadReg8(_magI2CHandle, OUT_X_L_M);
	uint8_t upper = wiringPiI2CReadReg8(_magI2CHandle, OUT_X_H_M);
	rVal = ((upper << 8) | lower);
	return rVal;
}

int16_t LSM303::magGetYAxisRaw()
{
	int16_t rVal;
	uint8_t lower = wiringPiI2CReadReg8(_magI2CHandle, OUT_Y_L_M);
	uint8_t upper = wiringPiI2CReadReg8(_magI2CHandle, OUT_Y_H_M);
	rVal = ((upper << 8) | lower);
	return rVal;
}

int16_t LSM303::magGetZAxisRaw()
{
	int16_t rVal;
	uint8_t lower = wiringPiI2CReadReg8(_magI2CHandle, OUT_Z_L_M);
	uint8_t upper = wiringPiI2CReadReg8(_magI2CHandle, OUT_Z_H_M);
	rVal = ((upper << 8) | lower);
	return rVal;
}

float LSM303::magGetXAxisGaus()
{
	float rVal;
	int16_t rawXAxis= magGetXAxisRaw();
	rVal = (float)rawXAxis / 1100;
	return rVal;
}

float LSM303::magGetYAxisGaus()
{
	float rVal;
	int16_t rawYAxis= magGetYAxisRaw();
	rVal = (float)rawYAxis / 1100;
	return rVal;
}

float LSM303::magGetZAxisGaus()
{
	float rVal;
	int16_t rawZAxis= magGetZAxisRaw();
	rVal = (float)rawZAxis / 980;
	return rVal;
}

void LSM303::magUnlock()
{
	wiringPiI2CWriteReg8(_magI2CHandle, SR_REG_M, 0x00);
}




