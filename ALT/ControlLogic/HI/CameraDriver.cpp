/*
 * CameraDriver.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: dt02
 */

#include "CameraDriver.h"

CameraDriver::CameraDriver() {

}

CameraDriver::~CameraDriver() {

}

void CameraDriver::takePicture(std::string name){
	m_strName = name;
	m_strName.append(".jpg");
	raspicam::RaspiCam_Still * rpc = new raspicam::RaspiCam_Still(); //Camera object
	rpc->setEncoding(raspicam::RASPICAM_ENCODING_JPEG);
	rpc->setCaptureSize(640,480);
    //Open camera
    std::cout << "Opening Camera..." << std::endl;
    if ( !rpc->open()) {
    	std::cerr << "Error opening camera" << std::endl;
    	return;
    }
    //wait a while until camera stabilizes
    std::cout << "Sleeping for 3 secs" << std::endl;
    //capture
    //allocate memory
    unsigned char * data = new unsigned char[rpc->getImageBufferSize()];
    //extract 
    rpc->grab_retrieve(data,rpc->getImageBufferSize());//get camera image
    //save
    std::ofstream outFile (m_strName.c_str(), std::ios::binary);
    outFile.write((char*)data, rpc->getImageBufferSize());
    std::cout << "Image saved at raspicam_image.ppm" << std::endl;
    outFile.close();
    //free resrources
    delete data;
    delete rpc;
}
