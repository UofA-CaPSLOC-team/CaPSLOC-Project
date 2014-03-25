
#include "GPS.h"

GPS::GPS()
{
	const char *device = "/dev/ttyAMA0";
	_handle = serialOpen(device, 9600);
}

GPS::~GPS()
{
	serialClose(_handle);
}

void GPS::ReadGPS()
{
	int length = 500;
	char *message = new char[length];
	char currentChar = (char)serialGetchar( _handle );
	int i=0;
	while (currentChar != '\r')
	{
		message[i] = currentChar;
		currentChar = (char)serialGetchar( _handle );
		i++;
	}
	std::string rVal = std::string(message);
	NmeaData *data = NmeaData::ParseData( rVal );

	delete[] message;
}

int GPS::dataAvailable()
{
	return serialDataAvail(_handle);
}

