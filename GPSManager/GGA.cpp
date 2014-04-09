#include "GGA.h"

GGA::GGA()
{
}

GGA::~GGA()
{
}

void GGA::ParseData(std::string data)
{
	int loopCount = 0;
	std::size_t startIndex = 0;
	std::size_t endIndex = data.find_first_of(",");
	std::size_t length = endIndex - startIndex;
	//std::cout << data << std::endl;
	while( endIndex !=std::string::npos )
	{
		std::string subString = data.substr(startIndex, endIndex-startIndex);
		//std::cout << loopCount << " " <<subString << std::endl;
		switch(loopCount)
		{
			case 0: //MessageID
				{
					_messageID = subString;
				}
				break;
			case 1: //UTCTime
				{
					_utcTime = atof( subString.c_str() );
				}
				break;
			case 2: //Latitude
				{
					if (subString.length() > 0)
					{
						float d = atof( subString.substr(0, 2).c_str() );
						std::size_t decimalLocation = subString.find_first_of(".");
						std::string minutes = subString.substr(decimalLocation-2, subString.length()-(decimalLocation - 2));
						float m = atof( minutes.c_str()) / 60;
						_latitude = d + m;
					}
				}
				break;
			case 3: //North/South indicator
				{
					_nsIndicator = subString;
				}
				break;
			case 4: //Longitude
				{
					if (subString.length() > 0)
					{
						float d = atof( subString.substr(0, 3).c_str() );
						std::size_t decimalLocation = subString.find_first_of(".");
						std::string minutes = subString.substr(decimalLocation-2, subString.length()-(decimalLocation-2));
						float m = atof( minutes.c_str()) / 60;
						_longitude = d + m;
					}
				}
				break;
			case 5:// East/West indicator
				{
					_ewIndicator = subString;
				}
				break;
			case 6: //position indicator
				{
					_positionIndicator = atoi( subString.c_str() );
				}
				break;
			case 7: //Satellites Used
				{
					_satellitesUsed = atoi( subString.c_str() );
				}
				break;
			case 8: //hdop
				{
					_hdop = atof( subString.c_str() );
				}
				break;
			case 9: //MSL altitude
				{
					_mslAltitude = atof( subString.c_str() );
				}
				break;
			case 10: //units
				{
					_mslUnits = subString;
				}
				break;
			case 11: //geoidal separation
				{
					_geoidalSeparation = atof( subString.c_str() );
				}
				break;
			case 12: //units
				{
					_geoidalUnits = subString;
				}
				break;
			case 13: //AgeofDiffCorrection
				{
					_ageOfDiffCorrection = atof (subString.c_str() );
				}
				break;
		}
		startIndex = endIndex + 1;
		endIndex = data.find_first_of(',', endIndex+1);
		length = endIndex - startIndex;
		loopCount++;
	}
	//Special case because last data element does not end with a ,
	endIndex = data.find_first_of('\r', endIndex+1);
	_checksum = data.substr(startIndex, endIndex);
	//std::cout << _checksum << std::endl;
}

float GGA::GetLatitude()
{
	if (_nsIndicator == "N")
		return _latitude;
	else 
		return -_latitude;
}

float GGA::GetLongitude()
{
	if (_ewIndicator == "E")
		return _longitude;
	else
		return -_longitude;
}

float GGA::GetAltitude()
{
	return _mslAltitude;
}

bool GGA::HasFix()
{
	return _positionIndicator > 0;
}

float GGA::GetUTCTime()
{
	return _utcTime;
}