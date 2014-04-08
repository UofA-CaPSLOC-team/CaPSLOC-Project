#include "VerticalAxis.h"

bool vertIsDoneMoving = true;

void verticalAxisDoneISR()
{
	cout << "Vertical Axis Is Done Moving" << endl;
	vertIsDoneMoving = true;
}

VerticalAxis::VerticalAxis(int spiChannel):PicManager(spiChannel)
{
	setupInterrupts();
}

void VerticalAxis::setupInterrupts()
{
	wiringPiSetupGpio();
	pinMode(_isDoneMovingPin, INPUT);
	pullUpDnControl(_isDoneMovingPin, PUD_OFF);
	wiringPiISR(_isDoneMovingPin, INT_EDGE_FALLING, &verticalAxisDoneISR);
}

bool VerticalAxis::MoveDegrees(int degreesToMove)
{
	bool rVal = false;
	if (abs(degreesToMove <=180))
	{
		vertIsDoneMoving = false;
		PicManager::MoveDegrees(degreesToMove);
		rVal = true;
	}
	return rVal;
}