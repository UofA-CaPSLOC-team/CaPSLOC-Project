
#ifndef __PICMANAGER_H_
#define __PICMANAGER_H_

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <cmath>

using namespace std;

class PicManager
{
public:
	PicManager(int spiDevice);
	virtual ~PicManager();

	virtual bool MoveDegrees(int degreesToMove);
	void Stop();

protected:
	int _spiDevice;
	unsigned char _spiMode;
	unsigned char _bitsPerWord;
	unsigned int _spiSpeed;
	int _spiFD;
	float _shaftDegreesPerPulse;
	float _gearReductionRatio;

	//spiDevice: 0=CS0, 1=CS1
	int openPort();
	int closePort();
	int writeAndRead( unsigned char *data);
};

#endif
