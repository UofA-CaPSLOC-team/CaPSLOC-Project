/*
 * MCPM.cpp
 *
 *  Created on: Feb 6, 2014
 *      Author: dt02
 */

#include "MCPM.h"

#define TESTING

MCPM::MCPM() {

}

MCPM::~MCPM() {

}

bool MCPM::gotoLocation(double dLongitude, double dLatitude, double dAltitude, std::string strLocName){


#ifdef TESTING
	std::cout << "---> Going to " << strLocName <<":\n\tLongitude: " << dLongitude << "\n\tLatitude: " << dLatitude << "\n\tAltitude: " << dAltitude << std::endl;
	usleep(500000); //sleep for half a second.
	return true;
#endif
}

bool MCPM::capturePicture(CaptureMode tCapMode, long lTimeOnTarget, int nResolution, short sFrameRate = 0){


#ifdef TESTING
	if(tCapMode == PIC){
		std::cout << "---> Capturing PICTURE for " << lTimeOnTarget << " ms at a resolution of "<< nResolution << "p."<< std::endl;
	} else if(tCapMode == VID){
		std::cout << "---> Capturing VIDEO for " << lTimeOnTarget << " ms at a resolution of " << nResolution << "p and a framerate of " << sFrameRate << " frames/second." << std::endl;
	}
	usleep(500000); //sleep for half a second.
	return true;
#endif
}

bool MCPM::relativeMotion(RelativeDirection tRelDir, double nDegrees){


#ifdef TESTING
	switch(tRelDir){
	case RIGHT:
		std::cout << "---> Moving RIGHT "<< nDegrees << " degrees."<< std::endl;
		break;
	case LEFT:
		std::cout << "---> Moving LEFT "<< nDegrees << " degrees."<< std::endl;
		break;
	case UP:
		std::cout << "---> Moving UP "<< nDegrees << " degrees."<< std::endl;
		break;
	case DOWN:
		std::cout << "---> Moving DOWN "<< nDegrees << " degrees."<< std::endl;
		break;
	default:
		break;
	}
	usleep(500000); //sleep for half a second
	return true;
#endif
}

GPSCoordinates MCPM::getGPSCoordinate(){
#ifdef TESTING
	return GPSCoordinates(43.298, 23.3429);
#endif
}
