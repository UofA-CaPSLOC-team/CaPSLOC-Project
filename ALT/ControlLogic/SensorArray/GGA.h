
#ifndef __GGA_H_
#define __GGA_H_

#include <string>
#include <stdlib.h>
#include <iostream>

class GGA
{
public:
	GGA();
	~GGA();
	void ParseData(std::string data);

	float GetUTCTime();
	float GetLatitude();
	float GetLongitude();
	float GetAltitude();
	bool HasFix();

private:
	std::string _messageID;
	float _utcTime;
	float _latitude;
	std::string _nsIndicator;
	float _longitude;
	std::string _ewIndicator;
	int _positionIndicator;
	int _satellitesUsed;
	float _hdop;
	float _mslAltitude;
	std::string _mslUnits;
	float _geoidalSeparation;
	std::string _geoidalUnits;
	float _ageOfDiffCorrection;
	std::string _checksum;
};
#endif