#ifndef __HMC6352_H_
#define __HMC6352_H_

#include <stdint.h>
#include <unistd.h>
#include "wiringPiI2C.h"

class HMC6352
{
public:
	float GetHeading();

private:
	const uint8_t _readI2CAddr = 0x43;
	const uint8_t _writeI2CAddr = 0x42;

	int _readI2Chandle;
	int _writeI2Chandle;

}

#endif