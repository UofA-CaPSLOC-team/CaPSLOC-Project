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
	CommandNode * peek_front();

	bool hasCommands(){return (m_lNumCmds >= 1);}

	void printList();
	void setUpIterator();

	void clear();

	CommandNode * getNextCmd();


private:

	std::deque<CommandNode> m_dqCmds;
	std::deque<CommandNode>::iterator m_itCurrNode;

	long m_lNumCmds;
};

#endif /* COMMANDLIST_H_ */
