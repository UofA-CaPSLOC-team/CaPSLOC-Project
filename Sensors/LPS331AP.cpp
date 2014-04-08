
#include "LPS331AP.h"

LPS331AP::LPS331AP()
{
	_i2cHandle = wiringPiI2CSetup ( _defaultI2cAddress );

	wiringPiI2CWriteReg8( _i2cHandle, RES_CONF, 0x79);

	//Turn pressure sensor on, 1hz pressure rate.
	wiringPiI2CWriteReg8( _i2cHandle, CTRL_REG1, 0xE0 );
}

uint32_t LPS331AP::getPressureRaw()
{
	uint32_t rVal;
	uint8_t xl = wiringPiI2CReadReg8( _i2cHandle, PRESS_OUT_XL );
	uint8_t l = wiringPiI2CReadReg8( _i2cHandle, PRESS_OUT_L );
	uint8_t h = wiringPiI2CReadReg8( _i2cHandle, PRESS_OUT_H );

	rVal = (h << 16) | (l << 8) | xl;
	return rVal;
}

int16_t LPS331AP::getTemperatureRaw()
{
	int16_t rVal;
	uint8_t l = wiringPiI2CReadReg8( _i2cHandle, TEMP_OUT_L );
	uint8_t h = wiringPiI2CReadReg8( _i2cHandle, TEMP_OUT_H );
	rVal = (h << 8) | l;
	return rVal;
}

float LPS331AP::getPressure() //mBar
{
	float rVal;
	uint32_t raw = getPressureRaw();
	rVal = (float)raw/4096.0;
	return rVal;
}

float LPS331AP::getTemperature()
{
	float rVal;
	int16_t raw = getTemperatureRaw();
	rVal = 42.5 + raw / 480.0;
}

float LPS331AP::getAltitudeInFeet()
{
	float rVal;
	float pressure = getPressure();
	rVal = (1-pow(pressure/1013.25, 0.190284))*145366.45;
	return rVal;
}

float LPS331AP::getAltitudeInMeters()
{
	float rVal;
	float pressure = getAltitudeInFeet();
	rVal = pressure/3.280839895;
	return rVal;
}