/*
 * CameraDriver.h
 *
 *  Created on: Apr 10, 2014
 *      Author: dt02
 */

#ifndef CAMERADRIVER_H_
#define CAMERADRIVER_H_

 #include <ctime>
#include <fstream>
#include <iostream>

#include <raspicam/raspicam.h>

class CameraDriver {
public:
	CameraDriver(std::string name);
	virtual ~CameraDriver();

	void takePicture();

private:
	std::string m_strName;
};

#endif /* CAMERADRIVER_H_ */
