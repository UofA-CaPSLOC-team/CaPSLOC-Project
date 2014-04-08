
#ifndef __GPSManager_H_
#define __GPSManager_H_

#include "wiringSerial.h"
#include "GGA.h"
#include "NSS.h"
#include <string>
#include <vector>

class FloatAverage
{
public:
	FloatAverage(int recordsToKeep)
	{
		_sizeOfArray = recordsToKeep;
		_numbers = new float[_sizeOfArray];
		_currentIndex = 0;
		_arrayIsFull = false;
	}
	~FloatAverage()
	{
		delete[] _numbers;
	}

	void Add( float floatToAdd )
	{
		_numbers[_currentIndex] = floatToAdd;
		if (_currentIndex == _sizeOfArray-1)
		{
			_currentIndex = 0;
			_arrayIsFull = true;
		}
		else
		{
			_currentIndex++;
		}
	}

	float GetAverage()
	{
		float rVal;
		if (_arrayIsFull)
		{
			for (int i=0; i<_sizeOfArray; i++)
			{
				rVal += _numbers[i];
			}
			rVal = rVal / _sizeOfArray;
		}
		else
		{
			for (int i=0; i<=_currentIndex; i++)
			{
				rVal += _numbers[i];
			}
			rVal = rVal / _currentIndex;
		}
		return rVal;
	}
private:
	float *_numbers;
	int _currentIndex;
	int _sizeOfArray;
	bool _arrayIsFull;
};

class GPSManager
{
public:
	GPSManager();
	~GPSManager();
	void UpdateGPSData();
	float GetLongitude();
	float GetLattitude();
	float GetAltitude();

	bool HasLock(){return gga->HasFix();}
private:
	int _handle;
	GGA *gga;
	NSS *nss;
	int _callCounts;
	FloatAverage *_longitude;
	FloatAverage *_latitude;
	FloatAverage *_altitude;
	float _long;
	float _lat;
	float _alt;

	void updateAppropiateNmeaObject( std::string message );
};

#endif