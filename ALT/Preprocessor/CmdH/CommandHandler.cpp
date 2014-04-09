/*
 * CommandHandler.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: dt02
 */

#include "CommandHandler.h"

CommandHandler::CommandHandler(){
	m_cmdManual = new CommandList();
	m_cmdScript = new CommandList();
	m_bExecScript = true;//change for official release.
	m_ptrMCPM = new MCPM();
	m_dLatOffset = m_dLongOffset = m_dAltOffset = 0;
	m_dRMotionAngle = 5;
	m_lVidtime = 5000000; //5 seconds
	m_lWaitTime = 500000; // half a second
	m_nQuality = 1080;
	m_tCapMode = PIC;
	m_sFrameRate = 30;
	m_stc = NULL;
	m_bPaused = false;
	m_bHaltExec = false;
	m_bp = NULL;
}

CommandHandler::CommandHandler(BoostParse * bp, SendToCTRL * stc) {
	m_cmdManual = bp->getManualCommands();
	m_cmdScript = bp->getScriptCommands();
	m_bExecScript = true;//change for official release.
	m_ptrMCPM = new MCPM();
	m_dLatOffset = bp->getConfig()->getLatOffset();
	m_dLongOffset = bp->getConfig()->getLongOffset();
	m_dAltOffset = bp->getConfig()->getAltOffset();
	m_dRMotionAngle = bp->getConfig()->getRMotionAngle();
	m_lVidtime = bp->getConfig()->getVidTime(); //5 seconds
	m_lWaitTime = bp->getConfig()->getWaitTime(); // half a second
	m_nQuality = bp->getConfig()->getQuality();
	m_tCapMode = bp->getConfig()->getCaptureMode();
	m_sFrameRate = bp->getConfig()->getFrameRate();
	m_stc = stc;
	m_bPaused = false;
	m_bHaltExec = false;
	m_bp = bp;
}

CommandHandler::~CommandHandler() {
	delete m_ptrMCPM;
	delete m_cmdManual;
	delete m_cmdScript;
}

void CommandHandler::execNext(void){
	while(!m_ptrMCPM->GPSHasLock()){
		sleep(1);
	}
	m_ptrMCPM->SetAltitudeFromGoogle(m_stc->sendGPS(m_ptrMCPM->getGPSCoordinate()));
	m_ptrMCPM->SetGPSCoordinatesFromGoogle(m_dLatOffset, m_dLongOffset);
	CommandNode * currCmd;
	std::string stcString;
	//This must run constantly to ensure proper execution of scripts.
	m_cmdScript->setUpIterator();
	while(1){
		//Remove this printList() for production.
//		m_cmdScript->printList();
		if(!m_bPaused && !m_bHaltExec){
			if(m_cmdManual->hasCommands()){
				currCmd = (m_cmdManual->pop_front());
			}else if(m_cmdScript->hasCommands()){
				currCmd = (m_cmdScript->getNextCmd());
			}else {
				continue;
			}
		}else {
			//Peek at the front of both lists. If either one has a RESUME, reset the pause flag and continue.
			if(m_bPaused){
				if(m_cmdManual->hasCommands()){
					if(m_cmdManual->peek_front()->getType() == RESUME){
						m_bPaused = false;
						m_stc->sendCommandDebug("RESUMING");
						continue;
					}
				}
				if(m_cmdScript->hasCommands()){
					if(m_cmdScript->peek_front()->getType() == RESUME){
						m_bPaused = false;
						m_stc->sendCommandDebug("RESUMING");
						continue;
					}
				}
			}
		}
		switch(currCmd->getType()){
		case HALT:
			//TODO Stop things!
			//TODO Need code to stop motors NOW!
			m_stc->sendCommandDebug("HALTING");
			break;
			//END HALT
		case RMOTION:
			m_ptrMCPM->relativeMotion(currCmd->getRelDirection(),
					currCmd->getAngle());
#ifdef DEBUG
			stcString = "MOVING in DIRECTION ";
			stcString.append(boost::lexical_cast<std::string>(currCmd->getRelDirection()));
			stcString.append(" ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getAngle()));
			stcString.append(" degrees.");
			m_stc->sendCommandDebug(stcString);
#endif
			break;
			//END RMOTION
		case CAPTURE:
			//TODO CHANGE THIS!
			while(!m_ptrMCPM->isReadyForNextLocation()){
				usleep(50000);
			}

			/*capturePicture(currCmd->getCapMode(),
					currCmd->getTimeOnTarget(),
					currCmd->getQuality(),
					currCmd->getFrameRate());
					*/
#ifdef DEBUG
			stcString = "CAPTURING in MODE ";
			stcString.append(boost::lexical_cast<std::string>(currCmd->getCapMode()));
			stcString.append(" for ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getTimeOnTarget()));
			stcString.append(" ms at a FRAMERATE of ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getFrameRate()));
			stcString.append(" in a QUALITY of ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getQuality()));
			m_stc->sendCommandDebug(stcString);
#endif
			break;
			//END CAPTURE
		case GOTO:
			while(!m_ptrMCPM->isReadyForNextLocation()){
				usleep(50000);
			}
			m_strLocName = currCmd->getName();
			m_ptrMCPM->gotoLocation(currCmd->getLongitude(),
					currCmd->getLatitude(),
					currCmd->getAltitude());
#ifdef DEBUG
			stcString = "GOING to ";
			stcString.append(boost::lexical_cast<std::string>(currCmd->getName()));
			stcString.append(" at LONGITUDE ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getLongitude()));
			stcString.append(", LATITUDE ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getLatitude()));
			stcString.append(", and ALTITUDE ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getAltitude()));
			m_stc->sendCommandDebug(stcString);
#endif
			break;
			//END GOTO
		case WAIT:
			//time in script is in ms, usleep reads in us: convert!
			std::cout << "--> Waiting for " << currCmd->getWaitTime() << " ms..." << std::endl;
#ifdef DEBUG
			stcString = "WAITING for ";
			stcString.append(boost::lexical_cast<std::string>(currCmd->getWaitTime()));
			m_stc->sendCommandDebug(stcString);
#endif
			usleep((currCmd->getWaitTime()) * 1000);
#ifdef DEBUG
			stcString = "RESUMING from WAIT.";
			m_stc->sendCommandDebug(stcString);
#endif
			break;
			//END WAIT
		case PAUSE:
			//Pause stuff!
			m_bPaused = true;
			m_stc->sendCommandDebug("PAUSING");
			break;
			//END PAUSE
		case RESUME:
			//Handled above, will only be hit if no matching PAUSE.
			break;
			//END RESUME
		case EXEC:
			//bring in another script file.
			m_bp->scriptFileParse(currCmd->getName());
			m_stc->sendCommandDebug("EXECUTING");
			break;
			//END EXEC
		default:
			break;
		}

	}
}


/*
void CommandHandler::capturePicture(CaptureMode mode, long tot, int qual, short framerate){
	std::ofstream picFile(m_strLocName.c_str(), std::ofstream::binary));
	if(mode == PIC){
		CCamera * cam = StartCamera((int)((qual * 4)/3), qual, 30, 0, true);
		int size = (qual * 4)/3) * qual * 4;
		char buffer[size];
		cam->ReadFrame(0, buffer, sizeof(buffer));
		picFile.write(buffer, size);
	}else {
		m_stc->sendCommandDebug("VIDEO NOT YET SUPPORTED!!!");
	}
}
*/

