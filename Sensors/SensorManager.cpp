#include "SensorManager.h"

SensorManager::SensorManager()
{
	_gps = new GPSManager();
	_gyro = new L3GD20();
	_accMag = new LSM303();
	_pressure = new LPS331AP();
	_altitudeFromGoogle = -1;
	_longitutdeFromGoogle = 0.0;
	_latititudeFromGoogle = 0.0;
}

float SensorManager::GetLongitude()
{
	if (_longitutdeFromGoogle != 0.0)
	{
		return _longitutdeFromGoogle;
	}
	else
	{
		return _gps->GetLongitude();
	}
}

float SensorManager::GetLatitude()
{
	if (_latititudeFromGoogle != 0.0)
	{
		return _latititudeFromGoogle;
	}
	else
	{
		return _gps->GetLattitude();
	}
}

float SensorManager::GetGPSAltitudeInMeters()
{
	return _gps->GetAltitude();
}

float SensorManager::GetPressureSensorAltitudeInMeters()
{
	return _pressure->getAltitudeInMeters();
}

float SensorManager::GetPitch()
{
	float x = 0.0;
	float z = 0.0;
	
	for (int i = 0; i<10; i++)
	{
		x += _accMag->accGetXAxisG();
		z += _accMag->accGetZAxisG();
	}
	x = x/10;
	z = z/10;
	
	float angle;
	if (z>0 && x <0)
		angle = atan2f(z, x) * 180/3.141592654 - 270;
	else
		angle = 90 + atan2f(z, x)*180/3.141592654;
	
	return angle;
}

float SensorManager::CalculateHorizontalDegreesOfMovementFromNorth( float targetLatitdue, float targetLongitude)
{
	float y = targetLongitude - GetLongitude();
	float x = targetLatitdue - GetLatitude();

	float angle = fabs(atan2f(y, x) * 180 / 3.141592654);
	if (angle > 90)
	{
		angle = angle - 90;
	}
	std::cout << "X: " << x << " Y: " << y << "  HorDegMovementAngle: " << angle << std::endl;
	if ( x > 0 )
	{
		if ( y > 0 )
		{
			std::cout << " X > 0 Y > 0 " << std::endl;
			angle = angle;
		}
		else if ( y < 0 )
		{
			std::cout << " X > 0 Y < 0 " << std::endl;
			angle = 360 - angle;
		}
	}
	else if ( x < 0)
	{
		if ( y < 0 )
		{
			std::cout << " X < 0 Y < 0 " << std::endl;
			angle = 270-angle;
		}
		else if ( y > 0 ) //Bottom right
		{
			std::cout << " X < 0 Y > 0 " << std::endl;
			angle = angle + 90;
		}
	}
	return angle;
}

float SensorManager::CalculateVerticalDegreesOfMovement( float targetLatitude, float targetLongitude, float targetAltidue)
{
	float rVal = 0.0;
	float distanceToTarget = calculateDistanceToTarget( targetLatitude, targetLongitude );
	float atitudeDifference = targetAltidue - getAltitudeInMeters();
	std::cout << "Current Altitude: " << getAltitudeInMeters() << std::endl;
	rVal = atan2f(atitudeDifference, distanceToTarget)*180/M_PI;

	return rVal;
}

float SensorManager::getAltitudeInMeters()
{
	float rVal= 0.0;
	if (_altitudeFromGoogle >= 0)
		rVal = _altitudeFromGoogle;
	else if (_gps->HasLock())
	{
		rVal = _gps->GetAltitude();
	}
	else
	{
		rVal = _pressure->getAltitudeInMeters();
	}
	return rVal;
}

float SensorManager::calculateDistanceToTarget( float targetLatitude, float targetLongitude)
{
	/*
	double dlong = (long2 - long1) * d2r;
    double dlat = (lat2 - lat1) * d2r;
    double a = pow(sin(dlat/2.0), 2) + cos(lat1*d2r) * cos(lat2*d2r) * pow(sin(dlong/2.0), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double d = 6367 * c;

    return d;*/

	float R = 6367;
	float rVal = 0.0;
	float lat1 = GetLatitude();
	float lon1 = GetLongitude();
	float lat2 = targetLatitude;
	float lon2 = targetLongitude;

	double dlon = (lon2 - lon1) * M_PI / 180;
	double dlat = (lat2 - lat1) * M_PI / 180;
	double a = powf(sinf(dlat/2.0), 2) + cosf(lat1*M_PI / 180) * cosf(lat2*M_PI / 180) * powf(sinf(dlon/2.0), 2);
    double c = 2 * atan2f(sqrtf(a), sqrtf(1-a));
    double d = 6367 * c;

    return d * 1000;
}

int SensorManager::GetMagXRaw()
{
	int rVal = 0;
	for (int i=0; i<50; i++)
	{
		rVal += _accMag->magGetXAxisRaw();
		_accMag->magGetZAxisRaw();
	}
	return rVal/50;
}

int SensorManager::GetMagYRaw()
{
	int rVal = 0;
	for (int i=0; i<50; i++)
	{
		rVal += _accMag->magGetYAxisRaw();
		_accMag->magGetZAxisRaw();
	}
	return rVal/50;
}