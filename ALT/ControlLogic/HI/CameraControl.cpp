/*
 * CameraControl.cpp
 *
 *  Created on: Apr 8, 2014
 *      Author: dt02
 */

#include "CameraControl.h"

CameraControl::CameraControl() {
	if (!Camera.open()) {
		std::cerr << "Error opening camera" << std::endl;
	}
	sleep(3);
}

CameraControl::~CameraControl() {
	// TODO Auto-generated destructor stub
}

void CameraControl::takeAPicture(std::string filename){
//	raspicam::RaspiCam Camera; //Camera object

	//Open camera
//	std::cout <<"Opening Camera..."<< std::endl;
//	if (!Camera.open()) {std::cerr << "Error opening camera" << std::endl;
//	return;
//	}

	//wait a while until camera stabilizes
//	std::cout << "Sleeping for 3 secs" << std::endl;
//	sleep(3);

	//capture
	Camera.grab();

	//allocate memory
	unsigned char *data = new unsigned char[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];

	//extract the image in rgb format
	Camera.retrieve(data, raspicam::RASPICAM_FORMAT_RGB );//get camera image

	//save
	std::ofstream outFile (filename, std::ios::binary );
	outFile<<"P6\n" << Camera.getWidth() << " " << Camera.getHeight() <<" 255\n";
	outFile.write ((char*)data, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
	std::cout << "Image saved at raspicam_image.ppm" << std::endl;

	//free resrources
	delete data;
}