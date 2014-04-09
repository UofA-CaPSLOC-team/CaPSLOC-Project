#include "MCPM.h"

MCPM::MCPM()
{
	cout << "Initializing MCPM..." << endl;
	vertical = new VerticalAxis(0);
	horizontal = new HorizontalAxis(1);
	sensors = new SensorManager();
	findLimitSwitch();
	//while(!(abs(sensors->GetMagYRaw()) < 15))
	//{
	//	findLimitSwitch();
	//	findSouth();
	//}
	_degreesFromNorth = 0;
}

MCPM::~MCPM()
{
	delete sensors;
	delete vertical;
	delete horizontal;
}

GPSCoordinates MCPM::getGPSCoordinate()
{
	float longitutde = sensors->GetLongitude();
	float latitude = sensors->GetLatitude();
	return GPSCoordinates(longitutde, latitude);
}

bool MCPM::gotoLocation( double dLatitude, double dLongitude, double dAltitude )
{
	float tPosNorth = sensors->CalculateHorizontalDegreesOfMovementFromNorth(dLatitude, dLongitude);

	float verticalOffsetInDegrees = sensors->CalculateVerticalDegreesOfMovement(dLatitude, dLongitude, dAltitude);

	float changeInVert = -(sensors->GetPitch() - verticalOffsetInDegrees);

	float horDegreesToMove = tPosNorth - _degreesFromNorth;

	cout <<"Horizontal degrees to move: " << horDegreesToMove << endl;
	cout <<"Vertical degrees to move: " << changeInVert << endl;

	bool hor = moveHorizontalDegrees(horDegreesToMove);
	bool vert = moveVerticalDegrees(changeInVert);
	cout <<"Horizontal can move: " << hor << endl;
	cout <<"Vertical can move: " << vert << endl;

	return (hor & vert);
}

bool MCPM::relativeMotion( RelativeDirection tRelDir, double nDegrees)
{
	bool rVal = false;
	if (horIsDoneMoving) 
	{
		if (tRelDir == UP)
		{
			rVal = moveVerticalDegrees(nDegrees);
		}
		else if (tRelDir == DOWN)
		{
			rVal = moveVerticalDegrees(-nDegrees);
		}
	}
	if (horIsDoneMoving)
	{
		if (tRelDir == LEFT)
		{
			rVal = moveHorizontalDegrees(nDegrees);
		}
		else if (tRelDir == RIGHT)
		{
			rVal = moveHorizontalDegrees(-nDegrees);
		}
	}
	return rVal;
}

void MCPM::SetGPSCoordinatesFromGoogle(float latitude, float longitude)
{
	if (sensors != NULL)
	{
		sensors->setGPSfromGoogle(latitude, longitude);
	}
}

void MCPM::SetAltitudeFromGoogle(float altitudeInMeters)
{
	if (sensors != NULL)
	{
		sensors->setAltitudeFromGoogle(altitudeInMeters);
	}
}

bool MCPM::isReadyForNextLocation()
{
	bool rVal = false;
	if(vertIsDoneMoving && horIsDoneMoving)
	{
		rVal = true;
	}
	return rVal;
}

bool MCPM::moveVerticalDegrees(int degreesToMove)
{
	bool rVal = false;
	float pitch = sensors->GetPitch();
	float finalPosition = pitch + degreesToMove;
	cout << "Pitch: " << pitch << " Final Position: " << finalPosition << endl;
	if (finalPosition <= 90 && finalPosition >= -90)
	{
		rVal = vertical->MoveDegrees(degreesToMove);
	}
	else
	{
		//cout << "Final position is going to be: " << finalPosition << endl;
	}
	return rVal;
}

bool MCPM::moveHorizontalDegrees(int degreesToMove)
{
	bool rVal = false;
	//cout << "Moving " << degreesToMove << " degrees." << endl;
	int endingDegrees = degreesMovedFromLimitSwitch + degreesToMove;
	//cout << "Ending degrees " << endingDegrees << " degrees." << endl;

	if (endingDegrees < 0)
	{
		degreesToMove = degreesToMove + 360;
	}
	else if (endingDegrees > 360)
	{
		degreesToMove = abs(degreesToMove) - 360;
	}
	//cout << "Actually going to move " << degreesToMove << endl;
	rVal = horizontal->MoveDegrees(degreesToMove);
	if (rVal)
	{
		degreesMovedFromLimitSwitch += degreesToMove;
		_degreesFromNorth += degreesToMove;
		//cout << "Degrees moved from Limit Switch: " << degreesMovedFromLimitSwitch << endl;
	}
	return rVal;
}

void MCPM::findLimitSwitch()
{
	if (!horIsDoneMoving)
		horizontal->Stop();
	cout << "Attempting to move -360 degrees" << endl;
	horizontal->MoveDegrees(-360);
	while(!horIsDoneMoving)
	{
		usleep(50000);
	}
}

void MCPM::findSouth()
{
	int bounceCount = 0;
	int magx = 0;
	int magy = 0;
	int prevx = 0;
	int prevy = 0;
	bool movedLeft = true;
	bool initialize = true;
	while(bounceCount < 10)
	{
		prevx = magx;
		prevy = magy;
		magx = sensors->GetMagXRaw();
		magy = sensors->GetMagYRaw();

		int difference = abs(abs(prevy)-abs(magy));
		if (initialize)
		{
			initialize = false;
			relativeMotion(LEFT, 20);
		}
		else
		{
			if (abs(prevy) > abs(magy)) // keep moving the same direction
			{

				cout << "Prev Y greater than current y" << endl;
				if (movedLeft)
				{
					cout << "Moving left..." << endl;
					relativeMotion(LEFT, difference);
					movedLeft = true;
				}
				else
				{
					cout << "Moving Right..." << endl;
					relativeMotion(RIGHT, difference);
					movedLeft = false;
				}
			}
			else //move opposide direction
			{
				cout << "Prev Y less than current y" << endl;
				if (movedLeft)
				{
					cout << "Moving right..." << endl;
					relativeMotion(RIGHT, difference);
					movedLeft = false;
				}
				else
				{
					cout << "Moving left..." << endl;
					relativeMotion(LEFT, difference);
					movedLeft = true;
				}
				bounceCount ++;
			}
		}
		while(!isReadyForNextLocation())
		{
			usleep(50000);
		}
	}
}