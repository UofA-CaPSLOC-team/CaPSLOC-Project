
#ifndef __NSS_H_
#define __NSS_H_

#include <iostream>
#include <stdlib.h>
#include <string>

class NSS
{
public:
	NSS();
	~NSS();
	void ParseData(std::string data);
	
private:
	std::string _messageID;
	std::string _mode1;
	int _mode2;
	int _sv1;
	int _sv2;
	int _sv3;
	int _sv4;
	int _sv5;
	int _sv6;
	int _sv7;
	int _sv8;
	int _sv9;
	int _sv10;
	int _sv11;
	int _sv12;
	float _pdop;
	float _hdop;
	float _vdop;
	std::string _checksum;
};

#endif