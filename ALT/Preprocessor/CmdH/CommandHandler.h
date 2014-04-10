/*
 * CommandHandler.h
 *
 *  Created on: Feb 4, 2014
 *      Author: dt02
 */

#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_

#define DEBUG

#include <iostream>
#include <string>
#include <vector>
#include "../InProc/CommandNode.h"
#include "../InProc/CommandList.h"
#include "../InProc/BoostParse.h"
#include "../../../MCPM/MCPM.h"
#include <stdlib.h>
#include <unistd.h>
#include "../InProc/Config.h"
#include "../../DPU/SendToCTRL.h"
#include <boost/lexical_cast.hpp>
#include "../../ControlLogic/HI/CameraDriver.h";

#define CNFGFILELOC "/opt/CaPSLOC/config.txt"

class BoostParse; //Forward Declaration to appease his majesty, G++.
class CommandHandler {
public:

	CommandHandler();
	CommandHandler(BoostParse * bp, SendToCTRL * stc);
	virtual ~CommandHandler();

	/**
	 * execNext executes manual and scripted command.
	 *
	 * When a manual command exists in the queue it is popped from the front
	 * of the queue before being executed.
	 *
	 * The scripted queue is run in a continuous loop unless a halt command is received.
	 */
	void execNext();

	void smoothHalt(){m_bHaltExec = true;}
	void startFromHalt(){m_bHaltExec = false;}

private:
	void capturePicture(CaptureMode mode, long tot, int qual, short framerate);

	bool m_bExecScript;
	BoostParse * m_bp;
	CommandList * m_cmdManual;
	CommandList * m_cmdScript;
	MCPM * m_ptrMCPM;
	SendToCTRL * m_stc;


	//TODO These have been defined in Config class, call them from there...
	double m_dLatOffset, m_dLongOffset, m_dAltOffset, m_dRMotionAngle;
	double m_dcurrLat, m_dcurrLong, m_dcurrAlt;
	long m_lVidtime, m_lWaitTime;
	int m_nQuality;
	CaptureMode m_tCapMode;
	short m_sFrameRate;
	bool m_bPaused, m_bHaltExec;
	std::string m_strLocName;
	CameraDriver * m_ptrCD;
	std::string m_strAltName;
};

#endif /* COMMANDHANDLER_H_ */
