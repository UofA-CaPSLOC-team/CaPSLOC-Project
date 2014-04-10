#ifndef __VERTICALAXIS_H_
#define __VERTICALAXIS_H_

extern bool vertIsDoneMoving;

#include "PicManager.h"
#include "/opt/WiringPi/wiringPi.h"

class VerticalAxis: public PicManager
{
public:
	VerticalAxis(int spiChannel);//:PicManager(spiChannel);
	//bool IsDoneMoving();
	bool MoveDegrees(int degreesToMove);
private:
	static const int _isDoneMovingPin = 31;
	
	void setupInterrupts();
};

#endif
