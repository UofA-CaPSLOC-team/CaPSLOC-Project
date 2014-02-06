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
#include "../InProc/CommandID.h"
#include "../InProc/CommandParse.h"
#include "../../ControlLogic/MCPM/MCPM.h"
#include <stdlib.h>
#include <unistd.h>


class CommandHandler {
public:

	CommandHandler(std::deque<CommandID> * vManualCmd, std::vector<CommandID> * vScriptCmd);
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
	std::deque<CommandID> * m_dqManualCmd;
	std::vector<CommandID> * m_vScriptCmd;
	MCPM * ptrMCPM;
	double m_dLatOffset, m_dLongOffset, m_dAltOffset, m_dRMotionAngle;
	long m_lVidtime, m_lWaitTime;
	int m_nQuality;
	CaptureMode m_tCapMode;
	short m_sFrameRate;
};

#endif /* COMMANDHANDLER_H_ */
