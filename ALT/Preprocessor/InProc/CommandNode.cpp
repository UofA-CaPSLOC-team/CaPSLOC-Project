/*
 * CommandNode.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: edwin
 */

#include "CommandNode.h"

CommandNode::CommandNode(){
	m_dLong = 0;
	m_dLat = 0;
	m_dAlt = 0;
	m_capMode = PIC;
	m_lTOT = 0;
	m_sFrameRate = -1;
	m_nQuality = -1;
	m_lWaittime = -1;
	m_rDir = RIGHT;
	m_dAngle = 0;
	m_tType = NOTYPE;
	m_strName = "";
}

CommandNode::~CommandNode(){

}

void CommandNode::setGoto(double longitude, double latitude, double altitude, std::string name){
	m_dLong = longitude;
	m_dLat = latitude;
	m_dAlt = altitude;
	m_strName =  name;
	m_capMode = PIC;
	m_lTOT = 0;
	m_sFrameRate = -1;
	m_nQuality = -1;
	m_lWaittime = -1;
	m_rDir = RIGHT;
	m_dAngle = 0;
	m_tType = GOTO;
}


void CommandNode::setCapture(CaptureMode mode, long tot = -1, short framerate = -1, int quality = -1){
	m_dLong = 0;
	m_dLat = 0;
	m_dAlt = 0;
	m_strName = "";
	m_capMode = mode;
	m_lTOT = tot;
	m_sFrameRate = framerate;
	m_nQuality = quality;
	m_lWaittime = -1;
	m_rDir = RIGHT;
	m_dAngle = 0;
	m_tType = CAPTURE;
}


void CommandNode::setWait(long time){
	m_dLong = 0;
	m_dLat = 0;
	m_dAlt = 0;
	m_strName = "";
	m_capMode = PIC;
	m_lTOT = 0;
	m_sFrameRate = -1;
	m_nQuality = -1;
	m_lWaittime = time;
	m_rDir = RIGHT;
	m_dAngle = 0;
	m_tType = WAIT;
}


void CommandNode::setExec(std::string name){
	m_dLong = 0;
	m_dLat = 0;
	m_dAlt = 0;
	m_strName = name;
	m_capMode = PIC;
	m_lTOT = 0;
	m_sFrameRate = -1;
	m_nQuality = -1;
	m_lWaittime = -1;
	m_rDir = RIGHT;
	m_dAngle = 0;
	m_tType = EXEC;
}


void CommandNode::setRMotion(RelativeDirection dir, double angle){
	m_dLong = 0;
	m_dLat = 0;
	m_dAlt = 0;
	m_strName = "";
	m_capMode = PIC;
	m_lTOT = 0;
	m_sFrameRate = -1;
	m_nQuality = -1;
	m_lWaittime = -1;
	m_rDir = dir;
	m_dAngle = angle;
	m_tType = RMOTION;
}


void CommandNode::setHalt(){
	m_dLong = 0;
	m_dLat = 0;
	m_dAlt = 0;
	m_strName = "";
	m_capMode = PIC;
	m_lTOT = 0;
	m_sFrameRate = -1;
	m_nQuality = -1;
	m_lWaittime = -1;
	m_rDir = RIGHT;
	m_dAngle = 0;
	m_tType = HALT;
}


void CommandNode::setPause(){
	m_dLong = 0;
	m_dLat = 0;
	m_dAlt = 0;
	m_strName = "";
	m_capMode = PIC;
	m_lTOT = 0;
	m_sFrameRate = -1;
	m_nQuality = -1;
	m_lWaittime = -1;
	m_rDir = RIGHT;
	m_dAngle = 0;
	m_tType = PAUSE;
}


void CommandNode::setResume(){
	m_dLong = 0;
	m_dLat = 0;
	m_dAlt = 0;
	m_strName = "";
	m_capMode = PIC;
	m_lTOT = 0;
	m_sFrameRate = -1;
	m_nQuality = -1;
	m_lWaittime = -1;
	m_rDir = RIGHT;
	m_dAngle = 0;
	m_tType = RESUME;
}
