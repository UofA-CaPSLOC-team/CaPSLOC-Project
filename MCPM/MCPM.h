/*
 * MCPM.h
 *
 *  Created on: Feb 6, 2014
 *      Author: dt02
 */

#ifndef __MCPM_H_
#define __MCPM_H_


//#include "../../Preprocessor/InProc/CommandNode.h"
//#include "../../Preprocessor/InProc/CommandList.h"
#include "../Sensors/SensorManager.h"
#include "../PICManager/HorizontalAxis.h"
#include "../PICManager/VerticalAxis.h"
#include <iostream>
#include <unistd.h>
#include <string>

typedef struct Coordinates{
	double longitude;
	double latitude;
	Coordinates(double lon, double lat): longitude(lon), latitude(lat){
	}
}GPSCoordinates;

typedef enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
} RelativeDirection;

class MCPM {
public:
	MCPM();
	~MCPM();

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
	bool gotoLocation(double dLongitude, double dLatitude, double dAltitude);

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
	//bool capturePicture(CaptureMode tCapMode, long lTimeOnTarget, int nResolution, short sFrameRate);

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

	/**
	 * getGPSCoordinate polls the sensors to determine the GPS coordinates of the ALT unit.
	 *
	 * @return GPS coordinates in a GPSCoordinates struct.
	 */
	GPSCoordinates getGPSCoordinate();

	/**
	* isReadyForNextLocation indicates when the MCPM has reached the assigned location and is ready for the next.
	*
	* @return true: ready for the next location
	* @return false: not ready for the next location
	*/
	bool isReadyForNextLocation();

	void SetGPSCoordinatesFromGoogle(float latititude, float longitude);

	void SetAltitudeFromGoogle(float altitudeInMeters);

	bool GPSHasLock()
	{
		return sensors->GPSHasLock();
	}

private:
	SensorManager *sensors;
	VerticalAxis *vertical;
	HorizontalAxis *horizontal;

	int _degreesFromSouth;
	void findLimitSwitch();
	void findSouth();
	bool moveVerticalDegrees(int degreesToMove);
	bool moveHorizontalDegrees(int degreesToMove);
};

#endif /* MCPM_H_ */
