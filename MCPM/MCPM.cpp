
#include "MCPM.h"

MCPM::MCPM()
{
	gps = new GPSManager();
}

MCPM::~MCPM()
{
	delete gps;
}

GPSCoordinates MCPM::getGPSCoordinate()
{
	float longitutde = gps->GetLongitude();
	float latitude = gps->GetLattitude();
	return GPSCoordinates(longitutde, latitude);
}