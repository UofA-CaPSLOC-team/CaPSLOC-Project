#ifndef __HORIZONTAL_AXIS_H_
#define __HORIZONTAL_AXIS_H_

extern bool horIsDoneMoving;

#include "PicManager.h"
//TODO CHANGE TO BETTER PATH. Maybe in /opt/WiringPi
#include "/opt/WiringPi/wiringPi.h"

extern int degreesMovedFromLimitSwitch;

class HorizontalAxis: public PicManager
{
public:
	HorizontalAxis(int spiChannel);//:PicManager(spiChannel);
	virtual ~HorizontalAxis(){}
	//bool IsDoneMoving();
	bool MoveDegrees(int degreesToMove);
private:
	static const int _isDoneMovingPin = 28;
	static const int _limitSwitchPin = 29;
	
	void setupInterrupts();
};

#endif
