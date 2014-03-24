/*
 * CommandHandler.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: dt02
 */

#include "CommandHandler.h"

CommandHandler::CommandHandler(CommandList * ManualCmd, CommandList * ScriptCmd, SendToCTRL * stc) {
	// TODO Auto-generated constructor stub
	m_cmdManual = ManualCmd;
	m_cmdScript = ScriptCmd;
	m_bExecScript = true;//TODO change for official release.
	m_ptrMCPM = new MCPM();
	m_dLatOffset = m_dLongOffset = m_dAltOffset = 0;
	m_dRMotionAngle = 5;
	m_lVidtime = 5000000; //5 seconds
	m_lWaitTime = 500000; // half a second
	m_nQuality = 1080;
	m_tCapMode = PIC;
	m_sFrameRate = 30;
	m_stc = stc;
}

CommandHandler::~CommandHandler() {
	delete m_ptrMCPM;
	delete m_cmdManual;
	delete m_cmdScript;
}

void CommandHandler::execNext(void){
	CommandNode * currCmd;
	std::string stcString;
	//TODO this is dangerous if it isn't a separate thread.
	m_cmdScript->setUpIterator();
	while(1){
		m_cmdScript->printList();
		if(!m_bPaused){
			if(m_cmdManual->hasCommands()){
				currCmd = (m_cmdManual->pop_front());
			}else if(m_cmdScript->hasCommands()){
				currCmd = (m_cmdScript->getNextCmd());
			}else {
				continue;
			}
		}else {
			//TODO Peek at the front of both lists. If either one has a RESUME, reset the pause flag and continue.
		}
		switch(currCmd->getType()){
		case HALT:
			//TODO Stop things!
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
			m_ptrMCPM->capturePicture(currCmd->getCapMode(),
					currCmd->getTimeOnTarget(),
					currCmd->getQuality(),
					currCmd->getFrameRate());
#ifdef DEBUG
			stcString = "CAPTURING in MODE ";
			stcString.append(boost::lexical_cast<std::string>(currCmd->getCapMode()));
			stcString.append(" for ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getTimeOnTarget()));
			stcString.append(" ms at a FRAMERATE of ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getFrameRate()));
			stcString.append(" in a QUALITY of ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getQuality()))
					m_stc->sendCommandDebug(stcString);
#endif
			break;
			//END CAPTURE
		case GOTO:
			m_ptrMCPM->gotoLocation(currCmd->getLongitude(),
					currCmd->getLatitude(),
					currCmd->getAltitude(),
					currCmd->getName());
#ifdef DEBUG
			stcString = "GOING to ";
			stcString.append(boost::lexical_cast<std::string>(currCmd->getName()));
			stcString.append(" at LONGITUDE ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getLongitude()));
			stcString.append(", LATITUDE ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getLatitude()));
			stcString.append(", and ALTITUDE ");
			stcString.append(boost::lexical_cast<std::string>(currCmd->getAltitude()))
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
			//TODO Pause stuff!
			m_bPaused = true;
			m_stc->sendCommandDebug("PAUSING");
			break;
			//END PAUSE
		case RESUME:
			//TODO Resume from pause!
			m_stc->sendCommandDebug("RESUMING");
			break;
			//END RESUME
		case EXEC:
			//TODO bring in another script file.
			m_stc->sendCommandDebug("EXECUTING");
			break;
			//END EXEC
		default:
			break;
		}

	}
}
