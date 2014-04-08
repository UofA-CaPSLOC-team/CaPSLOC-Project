#ifndef __SENSORMANAGER_H
#define __SENSORMANAGER_H

#include "L3GD20.h"
#include "LPS331AP.h"
#include "LSM303.h"
#include "GPSManager.h"
#include <math.h>

class SensorManager
{
public:
	SensorManager();

	float GetLongitude();
	float GetLatitude();
	float GetGPSAltitudeInMeters();
	float GetPressureSensorAltitudeInMeters();

	float CalculateHorizontalDegreesOfMovementFromNorth( float targetLatitude, float targetLongitude);
	float CalculateVerticalDegreesOfMovement( float targetLatitude, float targetLongitude, float targetAltitude);
	
	void setAltitudeFromGoogle(float altitudeInMeters){_altitudeFromGoogle = altitudeInMeters;}
	float getAltitudeReterievedFromGoogle(){return _altitudeFromGoogle;}
	void setGPSfromGoogle(float latitude, float longitude)
	{
		_latititudeFromGoogle = latitude;
		_longitutdeFromGoogle = longitude;
	}
	void clearGPSfromGoogle()
	{
		_latititudeFromGoogle = 0;
		_longitutdeFromGoogle = 0;
	}

	float GetPitch();

	int GetMagXRaw();
	int GetMagYRaw();

	bool GPSHasLock()
	{
		return _gps->HasLock();
	}

private:
	GPSManager *_gps;
	L3GD20 *_gyro;
	LSM303 *_accMag;
	LPS331AP *_pressure;
	float _altitudeFromGoogle;
	float _latititudeFromGoogle;
	float _longitutdeFromGoogle;

	float calculateDistanceToTarget( float targetLatitude, float targetLongitude);
	float getAltitudeInMeters();
};

#endif