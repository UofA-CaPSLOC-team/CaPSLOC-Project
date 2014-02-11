/*
 * MCPM.h
 *
 *  Created on: Feb 6, 2014
 *      Author: dt02
 */

#ifndef MCPM_H_
#define MCPM_H_


#include "../../Preprocessor/InProc/CommandID.h"
#include <iostream>
#include <unistd.h>
#include <string>
class MCPM {
public:
	MCPM();
	virtual ~MCPM();

	/**
	 * gotoLocation performs the math and system calls necessary to
	 * facilitate motion of the camera mount system to point to the
	 * desired target specified by the longitude, latitude,
	 * and altitude.
	 *
	 * @param dLongitude: The longitude of the desired location
	 * @param dLatitude: The latitude of the desired location
	 * @param dAltitude: The altitude of the desired location
	 * @param strLocName: name of the location as defined by user
	 *
	 * @return true: operation completed successfully
	 * @return false: operation failed miserably
	 */
	bool gotoLocation(double dLongitude, double dLatitude, double dAltitude, std::string strLocName);

	/**
	 * capturePicture performs the necessary system calls to capture a picture or
	 * video with the given parameters.
	 *
	 * @param tCapMode: The capture mode for this capture (pic or vid)
	 * @param lTimeOnTarget: The number of milliseconds the system is to remain transfixed on the target.
	 * @param nResolution: The desired vertical resolution parameter (eg. 1080 = 1920x1080, 720 = 1280x720, etc.)
	 * @param sFrameRate: The framerate of the capture, if the capture is to be video. defaulted to 0;
	 *
	 * @return true: operation completed successfully
	 * @return false: operation blew up in your face
	 */
	bool capturePicture(CaptureMode tCapMode, long lTimeOnTarget, int nResolution, short sFrameRate = 0);

	/**
	 * relativeMotion performs the math and system calls necessary to
	 * facilitate motion of the camera mount system to point the desired
	 * number of degrees in desired direction (up, down, left, right).
	 *
	 * @param tRelDir: The desired direction of the relative motion. (Right, Left, Up, Down)
	 * @param nDegrees: The desired magnitude of the motion desired in degrees.
	 *
	 * @return true: operation completed successfully
	 * @return false: operation tried it's darndest, but in the end was insufficient for the task at hand.
	 */
	bool relativeMotion(RelativeDirection tRelDir, double nDegrees);

private:
	//TODO Fill in as needed for private members and methods
};

#endif /* MCPM_H_ */
