#include "HorizontalAxis.h"

bool horIsDoneMoving = true;
int degreesMovedFromLimitSwitch = 0;

void horIsDoneMovingISR()
{
	cout << "Horizontal Axis Is Done Moving" << endl;
	cout << endl;
	horIsDoneMoving = true;
}

void horLimitSwitchHit()
{
	cout << "Barnacles. We hit the limit switch.!" << endl;
	if (degreesMovedFromLimitSwitch > 180)
		degreesMovedFromLimitSwitch = 360;
	else
	{
		degreesMovedFromLimitSwitch = 0;
	}
}

HorizontalAxis::HorizontalAxis(int spiChannel):PicManager(spiChannel)
{
	setupInterrupts();
}

void HorizontalAxis::setupInterrupts()
{
	wiringPiSetupGpio();

	pinMode(_isDoneMovingPin, INPUT);
	pullUpDnControl(_isDoneMovingPin, PUD_DOWN);
	wiringPiISR(_isDoneMovingPin, INT_EDGE_FALLING, &horIsDoneMovingISR);

	pinMode(_limitSwitchPin, INPUT);
	pullUpDnControl(_limitSwitchPin, PUD_DOWN);
	wiringPiISR(_limitSwitchPin, INT_EDGE_RISING, &horLimitSwitchHit);
}

bool HorizontalAxis::MoveDegrees(int degreesToMove)
{
	bool rVal = false;
	if (abs(degreesToMove) <=360)
	{
		horIsDoneMoving = false;
		PicManager::MoveDegrees(degreesToMove);
		rVal = true;
	}
	return rVal;
}