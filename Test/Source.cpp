
#include <iostream>
#include "L3GD20.h"
#include "LPS331AP.h"
#include "LSM303.h"
#include "GPSManager.h"
#include "PicManager.h"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>				//Needed for SPI port
#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "wiringPi.h"

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

void testMagnetometer()
{
	LSM303 *mag = new LSM303();
	cout.precision(10);

	while (1)
	{
		float accx = mag->accGetXAxisG();
		float accy = mag->accGetYAxisG();
		float accz = mag->accGetZAxisG();

		float magx = mag->magGetXAxisGaus();
		float magy = mag->magGetYAxisGaus();
		float magz = mag->magGetZAxisGaus();

		float roll = accx*accx + accz*accz;
		float rollRadians = atan2f(accy, sqrt(roll));

		float pitch = accy*accy + accz*accz;
		float pitchRadian = atan2f(accx, sqrt(pitch));

		float cosRoll = cosf(rollRadians);
		float sinRoll = sinf(rollRadians);
		float cosPitch = cosf(pitchRadian);
		float sinPitch = sinf(pitchRadian);

		float cmagx = magx * cosPitch + magz*sinPitch;
		float cmagy = magx * sinRoll * sinPitch + magy * cosRoll - magz * sinRoll * cosPitch;

		float angle = atan2f(cmagy, cmagx) * 180 /3.141592654;
		//cout << cmagx << " " << cmagy << '\r';
		//cout << magx << " " <<magy << " " << magz << '\r';
		cout << angle << '\r';
	}
}

void testMagnetometer2()
{
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

		string level;
		if (pitchangle < 1 && rollAngle < 1)
			level = "Level";
		else
			level = "Not Level";

		string south;
		if (magx < 0 && (abs(magy/b) <= 2))
			south = "South";
		else
			south = "not south";
		//cout << magx/b << " " << magy/b << " " << magz/b << " " <<atan2f(abs(magy), abs(magx))*180/3.141592654 << "        " <<pitchangle << " " << rollAngle << endl;
		cout << level << "   " << south << "    " << magx/b << " " << magy/b << "  " << atan2f(abs(magy), abs(magx))*180/3.141592654 << "     " << pitchangle << " " << rollAngle << endl;
		//cout << level << "    " << south << endl;
		//cout << magx << endl;
		//cout << atan2f(magx, magy) *180 /3.141592654 << '\r';
	}
}

template<typename Ta, typename Tb, typename To> void cross(Ta *a, Tb *b, To *out)
{
	out[0] = (a[1] * b[2]) - (a[2] * b[1]);
	out[1] = (a[2] * b[0]) - (a[0] * b[2]);
	out[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

template<typename Ta, typename Tb> float dot(Ta *a, Tb *b)
{
	float rVal = 0.0;
	rVal = float( (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]) );
	return rVal;
}

void normalize(int16_t *a)
{
	float mag = sqrt(dot(a, a));
	a[0]/=mag;
	a[1]/=mag;
	a[2]/=mag;
}

void normalize(float *a)
{
	float mag = sqrt(dot(a, a));
	a[0]/=mag;
	a[1]/=mag;
	a[2]/=mag;
}

void testMangnetometer3()
{
	LSM303* mag = new LSM303();
	cout.precision(10);

	while(1)
	{
		int from[] = {0, -1, 0};
		int16_t x = mag->magGetXAxisRaw();
		int16_t y = mag->magGetYAxisRaw();
		int16_t z = mag->magGetZAxisRaw();

		int16_t accx = -mag->accGetXAxisRaw();
		int16_t accy = -mag->accGetYAxisRaw();
		int16_t accz = -mag->accGetZAxisRaw();

		int16_t m[] = {x, y, z};
		int16_t a[] = {accx, accy, accz};

		float e[] = {0, 0, 0};
		float n[] = {0, 0, 0};
		cross(m, a, e);
		normalize(e);
		cross(a, e, n);
		normalize(n);

		float heading = atan2f(dot(e, from), dot(n, from)) * 180 / 3.141592654;
		cout << "Heading: " << heading << endl;
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

void testPicManager()
{
	PicManager * hor = new PicManager(0);
	PicManager * vert = new PicManager(1);
	while(1)
	{
		string input;
		int vertdegress;
		int hordegrees; 
		cout << "Enter horizontal Degrees to move: ";
		cin >> input;
		if (input == "exit")
			break;
		hordegrees = atoi(input.c_str());
		cout << "Enter vertical Degrees to move: ";
		cin >> input;
		if (input == "exit")
			break;
		vertdegress = atoi(input.c_str());
		cout << endl;
		if (abs(hordegrees) <= 360)
			hor->MoveDegrees(hordegrees);
		if (abs(vertdegress) <= 180)
			vert->MoveDegrees(vertdegress);
	}
	delete vert;
	delete hor;
}

int main(void)
{
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
	testPicManager();
	//testGPS();
	//testMagnetometer2();
	//testAccelerometer();
	//testPressure();
	//testGyro();
	//LSM303 *acc = new LSM303();
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
	cin.get();
	return 0;
}