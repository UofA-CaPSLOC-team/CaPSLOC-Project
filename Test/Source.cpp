
#include <iostream>
#include "L3GD20.h"
#include "LPS331AP.h"
#include "LSM303.h"
#include "GPSManager.h"
#include "SensorManager.h"
#include "MCPM.h"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>				//Needed for SPI port
#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include "wiringPi.h"
#include "VerticalAxis.h"
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>


using namespace std;

void testGPS()
{
	LPS331AP *pressure = new LPS331AP();
	FloatAverage *pressureAverage = new FloatAverage(50);
	cout.unsetf( ios::floatfield );
	cout.precision(15);
	GPSManager *gps = new GPSManager();
	for (int i=0; i<100; i++)
	{
		pressureAverage->Add(pressure->getAltitudeInMeters());
		gps->UpdateGPSData();
		cout << "Longitude: " << gps->GetLongitude() << endl;
		cout << "Lattitutde: " << gps->GetLattitude() << endl;
		cout << "Altitude (GPS): " << gps->GetAltitude() << endl;
		cout << "Altitude (pressure): " << pressureAverage->GetAverage() << endl;
		cout << endl;
	}
	cout << "All Done" << endl;
	delete pressure;
	delete pressureAverage;
	delete gps;
}

void TestGPS2()
{
	int handle = serialOpen("/dev/ttyAMA0", 9600);
	while(1)
	{
		std::string message;
		int i = 0;
		char currentChar = serialGetchar( handle );
		while (currentChar != '\n')
		{
			message.append(1, currentChar);
			currentChar = serialGetchar( handle );
			i++;
		}
		cout << message << endl;
		message.clear();
	}
}
void testMagnetometer2()
{
	MCPM *b = new MCPM();
	LSM303* mag = new LSM303();
	cout.precision(10);

	while(1)
	{
		int b = 25;
		int magx = 0;
		int magy= 0;
		int magz= 0;
		float accx = 0;
		float accy = 0;
		float accz = 0;
		for (int i=0; i<b; i++)
		{
			magx += mag->magGetXAxisRaw();
			magy += mag->magGetYAxisRaw();	
			magz += mag->magGetZAxisRaw();
			accx += mag->accGetXAxisG();
			accy += mag->accGetYAxisG();
			accz += mag->accGetZAxisG();
		}
		float pitchangle;
		if (accz>0 && accx <0)
			pitchangle = atan2f(accz, accx) * 180/3.141592654 - 270;
		else
			pitchangle = 90 + atan2f(accz, accx)*180/3.141592654;

		float rollAngle;
		rollAngle = 90 + atan2f(accz, accy) * 180/3.141592654;
		cout << magx/b << " " << magy/b << " " << magz/b << " " <<atan2f(abs(magy), abs(magx))*180/3.141592654 << "        " <<pitchangle << " " << rollAngle << endl;
	}
}

void testAccelerometer()
{
	LSM303 *acc = new LSM303();
	int b = 50;
	cout.precision(5);
	while (1)
	{
		float x = 0;
		float y = 0;
		float z = 0;
		for(int i=0; i<b; i++)
		{
			x += acc->accGetXAxisG();
			y += acc->accGetYAxisG();
			z += acc->accGetZAxisG();
		}
		
		float angle;
		if (z>0 && x <0)
			angle = atan2f(z, x) * 180/3.141592654 - 270;
		else
			angle = 90 + atan2f(z, x)*180/3.141592654;
		cout << angle << endl;
		//cout << x/b << " " << y/b << " " << z/b << endl;
	}
}

void testPressure()
{
	LPS331AP *ressure = new LPS331AP();
	while (1)
	{
		int b = 50;
		float pressure = 0;
		for (int i=0; i<b; i++)
		{
			pressure += ressure->getAltitudeInFeet();
		}
		cout << pressure / b << endl;
	}
}

void testGyro()
{
	L3GD20 * gyro = new L3GD20();
	while (1)
	{
		int b = 10;
		float x = 0;
		float y = 0;
		float z = 0;

		for (int i=0; i<b; i++)
		{
			x += gyro->getXAxisRotationSpeed();
			y += gyro->getYAxisRotationSpeed();
			z += gyro->getZAxisRotationSpeed();
		}
		cout << x/10 << " " << y/10 << " " << z/10 << endl;
	}
}

void TestVerticalAxis()
{
	VerticalAxis *vert = new VerticalAxis(0);
	while(1)
	{
		string input;
		cout << "Enter Degrees To Move: ";
		cin >> input;
		if (input == "exit")
			break;
		int degrees = atoi(input.c_str());
		vert->MoveDegrees(degrees);
		while(!vertIsDoneMoving)
		{
			sleep(1);
		}
	}
	delete vert;
}

void TestHorizontalAxis()
{
	HorizontalAxis *hor = new HorizontalAxis(1);
	while(1)
	{
		string input;
		cout << "Enter Degrees To Move: ";
		cin >> input;
		if (input == "exit")
			break;
		int degrees = atoi(input.c_str());
		hor->MoveDegrees(degrees);
		while(!horIsDoneMoving)
		{
			sleep(1);
		}
	}
}

void TestMCPM2()
{
	bool moveRight = false;
	MCPM *b = new MCPM();
	while(1)
	{
		while(!b->isReadyForNextLocation())
		{
			usleep(50000);
		}
		int degrees = rand() % 200;
		moveRight = degrees % 2;
		if (moveRight)
		{
			b->relativeMotion(RIGHT, degrees);
		}
		else
		{
			b->relativeMotion(LEFT, degrees);
		}
		while(!horIsDoneMoving)
		{
			usleep(50000);
		}
	}
}

void TestMCPM()
{
	MCPM *b = new MCPM();
	while(1)
	{
		bool canMove = false;
		string entry;
		cout << "Enter vertical degrees to move: ";
		cin >> entry;
		int degrees = atoi(entry.c_str());
		if (degrees >= 0)
			canMove = b->relativeMotion(UP, abs(degrees));
		else
			canMove = b->relativeMotion(DOWN, abs(degrees));

		cout << "Is the bastard able to move? " << canMove << endl;
		while(!vertIsDoneMoving && !horIsDoneMoving)
		{
			usleep(50000);
		}
	}
}

void TestyFindySouthy()
{
	LSM303 *mag = new LSM303();
	MCPM *b = new MCPM();
	while(!b->isReadyForNextLocation())
	{
		usleep(50000);
	}
	bool foundSouth = false;
	int c = 50;
	int magx = 0;
	int magy= 0;
	int magz= 0;
	int prevx;
	int prevy;
	bool movedLeft = true;
	int bounceCount = 0;
	bool initialize = true;
	while(bounceCount < 10)
	{
		cout << "Das Bounce Count: " << bounceCount << endl;
		prevx = magz;
		prevy = magy;
		magx = 0;
		magy = 0;
		magz = 0;
		for (int i=0; i<c; i++)
		{
			magx += mag->magGetXAxisRaw();
			magy += mag->magGetYAxisRaw();	
			magz += mag->magGetZAxisRaw();
		}
		magx = magx/c;
		magy = magy/c;
		cout << magx << " " <<magy << " " << endl;
		cout << "Previous Y: " << abs(prevy) << " Current Y: " << abs(magy) << endl;
		int difference = abs(abs(prevy) - abs(magy));
		if (initialize)
		{
			initialize = false;
			b->relativeMotion(LEFT, 20);

		}
		else
		{
			if (abs(prevy) > abs(magy)) // keep moving the same direction
			{

				cout << "Prev Y greater than current y" << endl;
				if (movedLeft)
				{
					cout << "Moving left..." << endl;
					b->relativeMotion(LEFT, difference);
					movedLeft = true;
				}
				else
				{
					cout << "Moving Right..." << endl;
					b->relativeMotion(RIGHT, difference);
					movedLeft = false;
				}
			}
			else //move opposide direction
			{
				cout << "Prev Y less than current y" << endl;
				if (movedLeft)
				{
					cout << "Moving right..." << endl;
					b->relativeMotion(RIGHT, difference);
					movedLeft = false;
				}
				else
				{
					cout << "Moving left..." << endl;
					b->relativeMotion(LEFT, difference);
					movedLeft = true;
				}
				bounceCount ++;
			}
		}
		while(!b->isReadyForNextLocation())
		{
			usleep(50000);
		}
	}
	delete b;
	delete mag;
}

void TestAccAndControl()
{
	string answer = "";
	HorizontalAxis *hor = new HorizontalAxis(1);
	LSM303 *acc = new LSM303();
	while (true)
	{
		//cout << "Are you ready to control via accelerometer? ";
		//cin >> answer;
		//if (answer == "y")
		//	break;
		cout << "Enter degrees to move: ";
		cin >> answer;
		int degreesToMove = atoi(answer.c_str());
		hor->MoveDegrees(degreesToMove);
		while(!horIsDoneMoving)
		{
		}
	}
	float last = 0;
	while(1)
	{
		float x, z;
		for (int i = 0; i<10; i++)
		{
			x += acc->accGetXAxisG();
			z += acc->accGetZAxisG();
		}
		x = x/10;
		z = z/10;

		float angle;
		if (z>0 && x <0)
			angle = atan2f(z, x) * 180/3.141592654 - 270;
		else
			angle = 90 + atan2f(z, x)*180/3.141592654;
		
		hor->MoveDegrees(angle - last);
		last = angle;
		usleep(50000);
	}
}

void TestSensorManager()
{
	SensorManager *sen = new SensorManager();
	while(1)
	{
		cout << sen->GetMagXRaw() << " " <<sen->GetMagYRaw() << endl;
	}
}

void TestI2C()
{
	L3GD20 *b = new L3GD20();
	while(1)
	{
		cout << "Press Enter when ready" << endl;
		cin.get();
		b->getXAxisRaw();
	}
}

void TestMCPM3()
{
	MCPM *m = new MCPM();
	while(!m->isReadyForNextLocation())
	{
		usleep(10000);
	}
	while(1)
	{
		string input;
		cout << "Enter horizontal degrees: ";
		cin >> input;
		int hor = atoi(input.c_str());
		cout << "Enter vertical degrees: ";
		cin >> input;
		int vert = atoi(input.c_str());
		if (hor < 0)
			m->relativeMotion(RIGHT, abs(hor));
		else
			m->relativeMotion(LEFT, abs(hor));
		if (vert < 0)
			m->relativeMotion(DOWN, abs(vert));
		else
			m->relativeMotion(UP, abs(vert));
		while(!m->isReadyForNextLocation())
		{
			usleep(10000);
		}
	}
	delete m;
}

void TestMCPM4()
{
	MCPM *m = new MCPM();
	while(!m->isReadyForNextLocation())
	{
		usleep(50000);
	}
	while(1)
	{
		cout << "STARTING NEW GIGGITY!!!" << endl;
		cout << " -----------------------------" << endl;
		//41.082432, -81.518510
		cout << "East Parking deck" << endl;
		m->gotoLocation(41.075983, -81.505137, 300); // east parking deck.
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
		sleep(5);

		cout << "Polsky Building" << endl;
		m->gotoLocation(41.078940, -81.519820, 300); // polsky building
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
		sleep(5);

		cout <<"Home plate of the Akron Aeors" << endl;
		m->gotoLocation(41.077763, -81.522180 , 300); //home plate of Akron Aeors
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
		sleep(5);

		cout <<"Exchange Parking Deck" << endl;
		m->gotoLocation(41.075133, -81.514702 , 300); //Exchange Parking Deck.
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
		sleep(5);
		cout <<"Route 8 Bridge" << endl;
		m->gotoLocation(41.087007, -81.501889 , 300); //Route 8 Bridge.
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
		sleep(5);
		cout <<"Somewhat north" << endl;
		m->gotoLocation(41.087770, -81.513103, 300); 
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
	
		sleep(5);
		cout <<"Home plate of the Akron Aeors" << endl;
		m->gotoLocation(41.077763, -81.522180 , 300); //home plate of Akron Aeors
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
		sleep(5);
		cout << "East Parking deck" << endl;
		m->gotoLocation(41.075983, -81.505137, 300); // east parking deck.
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
			sleep(5);

		cout << "Polsky Building" << endl;
		m->gotoLocation(41.078940, -81.519820, 300); // polsky building
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
		sleep(5);

		cout <<"Home plate of the Akron Aeors" << endl;
		m->gotoLocation(41.077763, -81.522180 , 300); //home plate of Akron Aeors
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
		sleep(5);

		cout <<"Exchange Parking Deck" << endl;
		m->gotoLocation(41.075133, -81.514702 , 300); //Exchange Parking Deck.
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
		sleep(5);
		cout <<"Route 8 Bridge" << endl;
		m->gotoLocation(41.087007, -81.501889 , 300); //Route 8 Bridge.
		while(!m->isReadyForNextLocation())
		{
			usleep(50000);
		}
	}
}


int main(void)
{
	TestMCPM4();
	//TestGPS2();
	//TestMCPM3();
	//TestVerticalAxis();
	//TestI2C();
	//MCPM *bob = new MCPM();
	////41.076035, -81.509466

	//cout << endl << "North North East" << endl;
	//bob->gotoLocation(41.083822, -81.513145, 100);
	//cout << endl<< "East East North" << endl;
	//bob->gotoLocation(41.077494, -81.500365, 80);
	//cout << endl<< "East East South" << endl;
	//bob->gotoLocation(41.074965, -81.498636, 50000);
	//cout << endl<< "South South East" << endl;
	//bob->gotoLocation(41.070043, -81.512756, 40);
	//cout << endl<< "South South West" << endl;
	//bob->gotoLocation(41.069576, -81.515742, 20);
	//cout << endl<< "West West South" << endl;
	//bob->gotoLocation(41.075497, -81.526247, 10);
	//cout << endl<< "West West North" << endl;
	//bob->gotoLocation(41.077847, -81.523128, 200);
	//cout << endl<< "North North West" << endl;
	//bob->gotoLocation(41.084941, -81.517941, 300);
	//bob->gotoLocation(1, 1, 0);
	/*bob->gotoLocation(1, -1, 0);
	bob->gotoLocation(-1, -1, 0);
	bob->gotoLocation(-1, 1, 0);
	*///bob->gotoLocation(1, 1, 0);
	//bob->gotoLocation(3, 2, 0);
	//bob->gotoLocation(3, 0, 0);
	//bob->gotoLocation(0, 3, 0);
	//bob->gotoLocation(0, 0, 0);
	//delete bob;
	//TestSensorManager();
	//TestyFindySouthy();
	//testMagnetometer2();
	//TestMCPM2();
	//TestMCPM();
	//TestAccAndControl();
	//wiringPiSetupGpio();
	//pinMode(29, INPUT);
	//pinMode(31, INPUT);
	//pullUpDnControl(29, PUD_OFF);
	//pullUpDnControl(31, PUD_OFF);
	//while(1)
	//{
	//	int twoNine = digitalRead(29);
	//	int threeOne = digitalRead(31);
	//	cout << twoNine << "  " << threeOne << "\r";
	//}
	//testPicManager();
	//TestVerticalAxis();
	//TestHorizontalAxis();
	//testMagnetometer2();

	//testGPS();
	//testMagnetometer2();
	//testAccelerometer();
	//testPressure();
	//testGyro();
	////testMagnetometer();
	//FloatAverage *xAxis = new FloatAverage(5000);
	//FloatAverage *yAxis = new FloatAverage(5000);
	//FloatAverage *zAxis = new FloatAverage(5000);
	//
	//for (int i=0; i<5000; i++)
	//{
	//	xAxis->Add(acc->accGetXAxisG());
	//	yAxis->Add(acc->accGetYAxisG());
	//	zAxis->Add(acc->accGetZAxisG());
	//}
	//cout << "x: " <<xAxis->GetAverage() << " y: " << yAxis->GetAverage() << " z: " << zAxis->GetAverage() << endl;
	return 0;
}