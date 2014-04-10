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

#include <raspicam/raspicam_still.h>

class CameraDriver {
public:
	CameraDriver();
	virtual ~CameraDriver();

	void takePicture(std::string name);

private:
	std::string m_strName;
};

#endif /* CAMERADRIVER_H_ */
