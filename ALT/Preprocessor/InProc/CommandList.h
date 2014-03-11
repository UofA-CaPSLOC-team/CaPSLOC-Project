/*
 * CommandList.h
 *
 *  Created on: Feb 26, 2014
 *      Author: edwin
 */

#ifndef COMMANDLIST_H_
#define COMMANDLIST_H_

#define VECTOR

#include "CommandNode.h"
#include <iostream>
#include <deque>

class CommandList {
public:
	CommandList();
	virtual ~CommandList();

	void push_back(CommandNode newNode);
	CommandNode * pop_front();

	bool hasCommands(){return (m_lNumCmds >= 1);}

	void printList();

private:
#ifndef VECTOR
	CommandNode * m_ptrHead, * m_ptrEnd;
#else
	std::deque<CommandNode> m_dqCmds;
#endif

	long m_lNumCmds;
};

#endif /* COMMANDLIST_H_ */
