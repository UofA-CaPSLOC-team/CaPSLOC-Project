/*
 * CommandHandler.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: dt02
 */

#include "CommandHandler.h"

CommandHandler::CommandHandler(std::deque<CommandID> * vManualCmd, std::vector<CommandID> * vScriptCmd) {
	// TODO Auto-generated constructor stub
	m_dqManualCmd = vManualCmd;
	m_vScriptCmd = vScriptCmd;
	m_bExecScript = false;
	ptrMCPM = new MCPM();
	m_dLatOffset = m_dLongOffset = m_dAltOffset = 0;
	m_dRMotionAngle = 5;
	m_lVidtime = 5000000; //5 seconds
	m_lWaitTime = 500000; // half a second
	m_nQuality = 1080;
	m_tCapMode = PIC;
	m_sFrameRate = 30;
}

CommandHandler::~CommandHandler() {
	// TODO Auto-generated destructor stub
}

inline long timeParse(std::string time){
	long rv, hours, mins, secs, msecs;
	std::string delimiter = ":";
	size_t pos = 0;
	std::string token;
	short count = 0;
	while ((pos = time.find(delimiter)) != std::string::npos) {
	    token = time.substr(0, pos);
	    if(count == 0){
	    	hours = atol(token.c_str());
	    }else if(count == 1){
	    	mins = atol(token.c_str());
	    }else if(count == 2){
	    	secs = atol(token.c_str());
	    }else if(count == 3){
	    	msecs = atol(token.c_str());
	    }
	    time.erase(0, pos + delimiter.length());
	    count++;
	}

	rv = (((((hours * 60) + mins) * 60) + secs) * 1000) + msecs;

	return rv;
}

void CommandHandler::execNext(){
	CommandID * currManCmd = NULL;
	std::vector<CommandID>::iterator it = m_vScriptCmd->begin();
	std::vector<CommandID> * subelem;
	while(true){
		//If a manual command exists, execute it first.
		if(!m_dqManualCmd->empty()){
			currManCmd = &(m_dqManualCmd->front());
			m_dqManualCmd->pop_front();
			switch(currManCmd->getType()){
			//TODO Long, exhaustive case blocks that call MCPM methods.
			//TODO parse out numbers to pass them to MCPM headers.
			case CONFIG:
				subelem = currManCmd->getSubElements();
				for(std::vector<CommandID>::iterator se = subelem->begin(); se != subelem->end(); ++se){
					//TODO need defaults for these somewhere and variables to keep them in...
					switch(se->getType()){
					case LATOFFSET:
						m_dLatOffset = atof(se->getValue().c_str());
						break;
					case LONGOFFSET:
						m_dLongOffset = atof(se->getValue().c_str());
						break;
					case ALTOFFSET:
						m_dAltOffset = atof(se->getValue().c_str());
						break;
					case RMOTIONANGLE:
						m_dRMotionAngle = atof(se->getValue().c_str());
						break;
					case VIDTIME:
						m_lVidtime = timeParse(se->getValue());
						break;
					case FRAMERATE:
						m_sFrameRate = (short)atoi(se->getValue().c_str());
						break;
					case IMAGEMODE:
						if(se->getValue().compare("vid")){
							m_tCapMode = VID;
						}else if(se->getValue().compare("pic")){
							m_tCapMode = PIC;
						}
						break;
					case QUALITY:
						m_nQuality = atoi(se->getValue().c_str());
						break;
					case WAITTIME:
						m_lWaitTime = timeParse(se->getValue());
						break;
					default:
						break;
					}
				}
				break;

			case GOTO:
				double longitude, latitude, altitude;
				std::string name;
				for(std::vector<CommandID>::iterator se = subelem->begin(); se != subelem->end(); ++se){
					switch(se->getType()){
					case LONGITUDE:
						longitude = atof(se->getValue().c_str());
						//TODO figure out how offsets will work.
						longitude += m_dLongOffset;//guess
						break;
					case LATITUDE:
						latitude = atof(se->getValue().c_str());
						//TODO figure out how offsets will work.
						latitude += m_dLatOffset;//guess
						break;
					case ALTITUDE:
						altitude = atof(se->getValue().c_str());
						//TODO figure out how offsets will work.
						altitude += m_dAltOffset;//guess
						break;
					case NAME:
						name = se->getValue();
						break;
					default:
						break;
					}
				}
				ptrMCPM->gotoLocation(longitude, latitude, altitude, name);
				break;

			case CAPTURE:
				int yRes, frameRate;
				CaptureMode mode;
				long timeOnTarget;
				for(std::vector<CommandID>::iterator se = subelem->begin(); se != subelem->end(); ++se){
					switch(se->getType()){
					case MODE:
						if(se->getValue().compare("vid")){
							mode = VID;
						}else{
							mode = PIC; //when in doubt, take a picture!
						}
						break;
					case TOT:
						timeOnTarget = timeParse(se->getValue());
						break;
					case FRAMERATE:
						frameRate = atoi(se->getValue().c_str());
						break;
					case QUALITY:
						yRes = atoi(se->getValue().c_str());
						break;
					default:
						break;
					}
				}
				ptrMCPM->capturePicture(mode, timeOnTarget, yRes, frameRate);
				break;

			case RMOTION:
				RelativeDirection dir;
				//TODO figure out way to use default angle if not set.
				double degrees;
				for(std::vector<CommandID>::iterator se = subelem->begin(); se != subelem->end(); ++se){
					switch(se->getType()){
					case DIRECTION:
						if(se->getValue().compare("right")){
							dir = RIGHT;
						}else if(se->getValue().compare("left")){
							dir = LEFT;
						}else if(se->getValue().compare("up")){
							dir = UP;
						}else if(se->getValue().compare("down")){
							dir = DOWN;
						}else{
							continue;//I don't know what direction this is, skip it!
						}
						break;
					case ANGLE:
						degrees = atof(se->getValue().c_str());
						break;
					default:
						break;
					}
				}
				ptrMCPM->relativeMotion(dir, degrees);
				break;

			case WAIT:
				//TODO Figure out way to use default.
				long milliseconds;
				for(std::vector<CommandID>::iterator se = subelem->begin(); se != subelem->end(); ++se){
					switch(se->getType()){
					case TIME:
						milliseconds = timeParse(se->getValue());
						break;
					default:
						break;
					}
				}
				usleep(milliseconds * 1000);
				break;

			case EXEC:
				for(std::vector<CommandID>::iterator se = subelem->begin(); se != subelem->end(); ++se){
					switch(se->getType()){
					case NAME:
						break;
					default:
						break;
					}
				}
				break;
				//END TYPES WITH SUB-ELEMENTS

			case HALT:
				//TODO halt actions
				break;

			case PAUSE:
				//TODO pause current actions, save spot
				break;

			case RESUME:
				//TODO resume from a pause.
				break;

			default:
				break;
			}

		}else { //if a manual command doesn't exist, keep going with the script where it left off.
			if(m_bExecScript && !m_vScriptCmd->empty()){
				switch(it->getType()){
				 //TODO copy switch statement from above, changing "currManCmd" to "it"
				}
				//If this is the last element in the script, loop back to the beginning.
				if(it != m_vScriptCmd->end()){
					++it;
				} else{
					it = m_vScriptCmd->begin();
				}
			}
		}
	}
}

