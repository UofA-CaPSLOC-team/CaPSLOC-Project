/*
 * CommandHandler.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: dt02
 */

#include "CommandHandler.h"

std::string getALTName(){
	char rawResult[256];
	std::string rv;
	std::ifstream is(CNFGFILELOC, std::ifstream::in);
	while(is.is_open() && rv.empty() && !is.eof()){
		is.getline(rawResult, 256);
		rv = rawResult;
		if(rv.find("Name=") != std::string::npos){
			rv = rv.substr(5, std::string::npos);
		} else {
			rv.clear();
		}
	}
	return rv;
}

CommandHandler::CommandHandler(){
	m_strAltName = getALTName();
	m_cmdManual = new CommandList();
	m_cmdScript = new CommandList();
	m_bExecScript = true;//change for official release.
	m_ptrMCPM = NULL;
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
	m_strAltName = getALTName();
	m_cmdManual = bp->getManualCommands();
	m_cmdScript = bp->getScriptCommands();
	m_bExecScript = true;//change for official release.
	m_ptrMCPM = NULL;
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
	m_ptrMCPM = new MCPM();
	m_ptrCD = new CameraDriver();
	if (m_dLatOffset != 0.0)
	{
		std::cout << "Setting the gps from Google" << std::endl;
		m_ptrMCPM->SetGPSCoordinatesFromGoogle(m_dLatOffset, m_dLongOffset);
	}
	else
	{
		std::cout << "Waiting for GPS Lock.\n";
		int counter = 0;
		while(!m_ptrMCPM->GPSHasLock() && counter < 5){
				sleep(1);
				++counter;
			}
	}
	std::cout<< "Attempting to the get the altitude from Google..." << std::endl;
	m_ptrMCPM->SetAltitudeFromGoogle(m_stc->sendGPS(m_ptrMCPM->getGPSCoordinate()));
	std::cout << "GPS lock acquired.\n";
	m_stc->sendCommandDebug("INFO: GPS lock aqcuired.");
	CommandNode * currCmd;
	std::string stcString;
	//This must run constantly to ensure proper execution of scripts.
	m_cmdScript->setUpIterator();
	while(1){
		//Remove this printList() for production.
//		m_cmdScript->printList();
		std::cout << "Script Commands:\n";
		m_cmdScript->printList();

			if(m_cmdManual->hasCommands()){
				currCmd = (m_cmdManual->pop_front());
			}else if(m_cmdScript->hasCommands()){
				currCmd = (m_cmdScript->getNextCmd());
			}else {
				continue;
			}
		std::cout << "Command Type: " << currCmd->getType() << "\n";
		switch(currCmd->getType()){
		case HALT:
			//Stop things!
			//Need code to stop motors NOW!
			m_stc->sendCommandDebug("HALTING");
			m_ptrMCPM->stopMovement();
			m_cmdScript->clear();
			m_cmdManual->clear();
			m_bHaltExec = true;
			break;
			//END HALT
		case RMOTION:
			m_ptrMCPM->relativeMotion(currCmd->getRelDirection(),
					currCmd->getAngle());
			stcString = "MOVING in DIRECTION ";
			stcString.append(boost::lexical_cast<std::string>(currCmd->getRelDirection()));
			stcString.append(" ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getAngle()));
			stcString.append(" degrees.");
			m_stc->sendCommandDebug(stcString);
			break;
			//END RMOTION
		case CAPTURE:
			//TODO CHANGE THIS!
		{
			while(!m_ptrMCPM->isReadyForNextLocation()){
				usleep(50000);
			}
			std::string fullName = "/opt/CaPSLOC/pics/";
			fullName.append(m_strLocName);
//			system(fullName);
			m_ptrCD->takePicture(fullName);
			/*capturePicture(currCmd->getCapMode(),
					currCmd->getTimeOnTarget(),
					currCmd->getQuality(),
					currCmd->getFrameRate());
					*/
			fullName.append(".png");
			stcString = "CAPTURING in MODE ";
			stcString.append(boost::lexical_cast<std::string>(currCmd->getCapMode()));
			stcString.append(" for ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getTimeOnTarget()));
			stcString.append(" ms at a FRAMERATE of ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getFrameRate()));
			stcString.append(" in a QUALITY of ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getQuality()));
			m_stc->sendCommandDebug(stcString);
			m_stc->sendPicToCTRL(fullName, m_strAltName, m_dcurrLong, m_dcurrLat, m_dcurrAlt, m_strLocName, "null");
		}
			break;
			//END CAPTURE
		case GOTO:
			while(!m_ptrMCPM->isReadyForNextLocation()){
				std::cout << "Waiting for isReadyForNextLocation()\n";
				usleep(50000);
			}
			m_strLocName = currCmd->getName();
			m_ptrMCPM->gotoLocation(m_dcurrLong = currCmd->getLongitude(),
					m_dcurrLat = currCmd->getLatitude(),
					m_dcurrAlt = currCmd->getAltitude());
			stcString = "GOING to ";
			stcString.append(boost::lexical_cast<std::string>(currCmd->getName()));
			stcString.append(" at LONGITUDE ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getLongitude()));
			stcString.append(", LATITUDE ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getLatitude()));
			stcString.append(", and ALTITUDE ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getAltitude()));
			m_stc->sendCommandDebug(stcString);
			break;
			//END GOTO
		case WAIT:
			//time in script is in ms, usleep reads in us: convert!
			std::cout << "--> Waiting for " << currCmd->getWaitTime() << " ms..." << std::endl;
			stcString = "WAITING for ";
			stcString.append(boost::lexical_cast<std::string>(currCmd->getWaitTime()));
			m_stc->sendCommandDebug(stcString);
			usleep((currCmd->getWaitTime()) * 1000);
			stcString = "RESUMING from WAIT.";
			m_stc->sendCommandDebug(stcString);
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
		usleep(50000);
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

