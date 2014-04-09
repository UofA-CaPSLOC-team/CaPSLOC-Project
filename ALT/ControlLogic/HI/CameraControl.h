/*
 * CameraControl.h
 *
 *  Created on: Apr 8, 2014
 *      Author: dt02
 */

#ifndef CAMERACONTROL_H_
#define CAMERACONTROL_H_

#include <ctime>
#include <fstream>
#include <iostream>
//#include <raspicam/raspicam.h>

class CameraControl {
public:
	CameraControl();
	virtual ~CameraControl();

	void takeAPicture(std::string filename);

private:
	//raspicam::RaspiCam Camera;
};

#endif /* CAMERACONTROL_H_ */
