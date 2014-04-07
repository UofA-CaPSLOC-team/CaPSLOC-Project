#include "GPSManager.h"
#include <iostream>

GPSManager::GPSManager()
{
	_handle = serialOpen( "/dev/ttyAMA0", 9600 );
	gga = new GGA();
	nss = new NSS();
	_latitude = new FloatAverage(50);
	_longitude = new FloatAverage(50);
	_altitude = new FloatAverage(50);
}

GPSManager::~GPSManager()
{
	serialClose( _handle );
	delete _latitude;
	delete _longitude;
	delete _altitude;
	delete gga;
	delete nss;
}

void GPSManager::UpdateGPSData()
{
	std::string message;
	int i = 0;
	char currentChar = serialGetchar( _handle );
	while (currentChar != '\n')
	{
		message.append(1, currentChar);
		currentChar = serialGetchar( _handle );
		i++;
	}
	updateAppropiateNmeaObject( message );
}

float GPSManager::GetLongitude()
{
	//UpdateGPSData();
	//return _long;
	//TODO: uncomment
	return _longitude->GetAverage();
}

float GPSManager::GetLattitude()
{
	//UpdateGPSData();
	//return _lat;
	//TODO: uncomment
	return _latitude->GetAverage();
}

float GPSManager::GetAltitude()
{
	//UpdateGPSData();
	//return _alt;
	//TODO: uncomment
	return _altitude->GetAverage();
}

void GPSManager::updateAppropiateNmeaObject( std::string message )
{
	std::string type = message.substr(0, message.find_first_of(","));

	if (type == "$GPGGA")
	{
		gga->ParseData( message );
		if (gga->HasFix())
		{
			std::cout << "Longitude: " << gga->GetLongitude() << " Latitude: " << gga->GetLatitude() << std::endl;
			_longitude->Add(gga->GetLongitude());
			_latitude->Add(gga->GetLatitude());
			_altitude->Add(gga->GetAltitude());
			//_long = gga->GetLongitude();
			//_lat = gga->GetLatitude();
			//_alt = gga->GetAltitude();
		}
	}
	else if (type == "$GPGSA")
	{
		nss->ParseData( message );
	}
	else if (type == "$GPGSV")
	{
		//std::cout << "GPGSV" << std::endl;
		//TODO: parse GPGSV data
	}
	else if (type == "$GPRMC")
	{
		//std::cout<< "GPRMC" << std::endl;
		//TODO: parse GPRMC data
	}
	else if (type == "$GPVTG")
	{
		//std::cout << "GPRMC" << std::endl;
		//TODO: parse GPVTG data
	}
	else
	{
		std::cout << type << std::endl;
	}
}
