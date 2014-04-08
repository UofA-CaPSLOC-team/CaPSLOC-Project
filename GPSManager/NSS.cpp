#include "NSS.h"

NSS::NSS()
{
}

NSS::~NSS()
{
}

void NSS::ParseData(std::string data)
{
	int loopCount = 0;

	std::size_t startIndex = 0;
	std::size_t endIndex = data.find_first_of(",");
	std::size_t length = endIndex - startIndex;

	std::size_t f = data.find("*");
	data.replace(f, std::string("*").length(), ",");
	//std::cout << data << std::endl;

	while( endIndex !=std::string::npos )
	{
		std::string subString = data.substr(startIndex, endIndex-startIndex);
		//std::cout << loopCount << " " <<subString << std::endl;
		switch(loopCount)
		{
			case 0: //MessageID
				_messageID = subString;
				break;
			case 1: //Mode1
				_mode1 = subString;
				break;
			case 2: //mode2
				_mode2 = atoi( subString.c_str() );
				break;
			case 3: //SV1
				_sv1 = atoi( subString.c_str() );
				break;
			case 4: //SV2
				_sv2 = atoi( subString.c_str() );
				break;
			case 5: //SV3
				_sv3 = atoi( subString.c_str() );
				break;
			case 6: //SV4
				_sv4 = atoi( subString.c_str() );
				break;
			case 7: //SV5
				_sv5 = atoi( subString.c_str() );
				break;
			case 8: //SV6
				_sv6 = atoi( subString.c_str() );
				break;
			case 9:
				_sv7 = atoi( subString.c_str() );
				break;
			case 10:
				_sv8 = atoi( subString.c_str() );
				break;
			case 11:
				_sv9 = atoi( subString.c_str() );
				break;
			case 12:
				_sv10 = atoi( subString.c_str() );
				break;
			case 13:
				_sv12 = atoi( subString.c_str() );
				break;
			case 14:
				_pdop = atof( subString.c_str() );
				break;
			case 15:
				_hdop = atof( subString.c_str() );
				break;
			case 16:
				_vdop = atof( subString.c_str() );
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
	//std::cout << _checksum <<std::endl;
}

