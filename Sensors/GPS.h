
#include <stdint.h>
#include <cmath>
#include "wiringSerial.h"
#include <iostream>

class GPS
{
public:
	GPS();
	~GPS();
	void ReadGPS();
	int dataAvailable();
private:
	int _handle;
};

class NmeaData
{
public:
	NmeaData();
	virtual ~NmeaData();
	virtual std::string GetType();
	virtual NmeaData* ParseData(std::string data);
};

class GGA : NmeaData
{
public:
	GGA();
	~GGA();

private:
	std::string _type;
	float _utcTime;
	float _lattitude;
	char _nsIndicator;
	float _longitude;
	char _ewIndicator;
	int _positionInidicator;
	int _satellitesUsed;
	float _hdop;
	float _mslAltitude;
	char _altitudeUnits;
	float _geoidalSpearation;
	char _geodialUnits;

};

