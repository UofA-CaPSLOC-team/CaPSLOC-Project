/*
 * CommandHandler.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: dt02
 */

#include "CommandHandler.h"

CommandHandler::CommandHandler(CommandList * ManualCmd, CommandList * ScriptCmd) {
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
}

CommandHandler::~CommandHandler() {
	delete m_ptrMCPM;
	delete m_cmdManual;
	delete m_cmdScript;
}

void CommandHandler::execNext(){
	CommandNode * currCmd;
	//TODO this is dangerous if it isn't a separate thread.
	while(1){
		m_cmdScript->printList();
		if(m_cmdManual->hasCommands()){
			currCmd = (m_cmdManual->pop_front());
		}else if(m_cmdScript->hasCommands()){
			currCmd = (m_cmdScript->pop_front());
		}else {
			continue;
		}
		switch(currCmd->getType()){
		case HALT:
			//TODO Stop things!
			break;
			//END HALT
		case RMOTION:
			m_ptrMCPM->relativeMotion(currCmd->getRelDirection(),
					currCmd->getAngle());
			break;
			//END RMOTION
		case CAPTURE:
			m_ptrMCPM->capturePicture(currCmd->getCapMode(),
					currCmd->getTimeOnTarget(),
					currCmd->getQuality(),
					currCmd->getFrameRate());
			break;
			//END CAPTURE
		case GOTO:
			m_ptrMCPM->gotoLocation(currCmd->getLongitude(),
					currCmd->getLatitude(),
					currCmd->getAltitude(),
					currCmd->getName());
			break;
			//END GOTO
		case WAIT:
			//time in script is in ms, usleep reads in us: convert!
			std::cout << "Waiting for " << currCmd->getWaitTime() << " ms..." << std::endl;
			usleep((currCmd->getWaitTime()) * 1000);
			break;
			//END WAIT
		case PAUSE:
			//TODO Pause stuff!
			break;
			//END PAUSE
		case RESUME:
			//TODO Resume from pause!
			break;
			//END RESUME
		case EXEC:
			//TODO bring in another script file.
			break;
			//END EXEC
		default:
			break;
		}

	}
}
