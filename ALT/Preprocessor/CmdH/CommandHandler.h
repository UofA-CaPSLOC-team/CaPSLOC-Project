/*
 * CommandHandler.h
 *
 *  Created on: Feb 4, 2014
 *      Author: dt02
 */

#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_

#include <iostream>
#include <string>
#include <vector>
#include "../InProc/CommandNode.h"
#include "../InProc/CommandList.h"
#include "../InProc/BoostParse.h"
#include "../../ControlLogic/MCPM/MCPM.h"
#include <stdlib.h>
#include <unistd.h>
#include "../InProc/Config.h"
#include "../../DPU/SendToCTRL.h"



class CommandHandler {
public:

	CommandHandler(CommandList * ManualCmd, CommandList * ScriptCmd, SendToCTRL * stc);
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

private:
	bool m_bExecScript;
	CommandList * m_cmdManual;
	CommandList * m_cmdScript;
	MCPM * m_ptrMCPM;
	SendToCTRL * m_stc;


	//TODO These have been defined in Config class, call them from there...
	double m_dLatOffset, m_dLongOffset, m_dAltOffset, m_dRMotionAngle;
	long m_lVidtime, m_lWaitTime;
	int m_nQuality;
	CaptureMode m_tCapMode;
	short m_sFrameRate;
	bool m_bPaused;
};

#endif /* COMMANDHANDLER_H_ */
