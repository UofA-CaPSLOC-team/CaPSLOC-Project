/*
 * CommandList.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: edwin
 */

#include "CommandList.h"

CommandList::CommandList() {
	m_lNumCmds = 0;
}

CommandList::~CommandList() {
	//TODO ensure no memory leaks!
	//	while(m_lNumCmds >= 1){
	//		pop_front();
	//	}
	//	delete m_ptrHead;
	//	delete m_ptrEnd;
}

void CommandList::push_back(CommandNode newNode){
	m_dqCmds.push_back(newNode);
	m_lNumCmds++;
}


CommandNode * CommandList::pop_front(){
	CommandNode * rv = &m_dqCmds.front();
	m_dqCmds.pop_front();
	return rv;

}

void CommandList::printList(){
	std::cout << "\n\nContents of CommandList:\n";


		for(std::deque<CommandNode>::iterator currNode = m_dqCmds.begin(); currNode != m_dqCmds.end(); ++currNode){
			std::cout << "\t -->";
			switch(currNode->getType()){
			case GOTO:
				std::cout << "GOTO\n";
				std::cout << "\t\t --> Lat: " << currNode->getLatitude() << "\n";//TODO for some reason calling a get method clears out everything in the following nodes. WHY!?!?!!?
				std::cout << "\t\t --> Long: " << currNode->getLongitude() << "\n";
				std::cout << "\t\t --> Alt: " << currNode->getAltitude() << "\n";
				std::cout << "\t\t --> Name: " << currNode->getName() << "\n";
				break;
			case CAPTURE:
				std::cout << "CAPTURE\n";
				std::cout << "\t\t --> Mode: " << ((currNode->getCapMode() == PIC)? "PIC" : "VID") << "\n";
				std::cout << "\t\t --> TOT: " << currNode->getTimeOnTarget() << "\n";
				std::cout << "\t\t --> FrameRate: " << currNode->getFrameRate() << "\n";
				std::cout << "\t\t --> Quality: " << currNode->getQuality() << "\n";
				break;
			case WAIT:
				std::cout << "WAIT\n";
				std::cout << "\t\t --> Time: " << currNode->getWaitTime() << "\n";
				break;
			case EXEC:
				std::cout << "EXEC\n";
				std::cout << "\t\t --> Name: " << currNode->getName() << "\n";
				break;
			case RMOTION:
				std::cout << "RMOTION\n";
				std::cout << "\t\t --> Direction: " << currNode->getRelDirection() << "\n";
				std::cout << "\t\t --> Angle: " << currNode->getAngle() << "\n";
				break;
			case HALT:
				std::cout << "HALT\n";
				break;
			case PAUSE:
				std::cout << "PAUSE\n";
				break;
			case RESUME:
				std::cout << "RESUME\n";
				break;
			case NOTYPE:
				std::cout << "NOTYPE\n";
				break;
			default:
				std::cout << "ERROR\n";
				break;
			}
		}
		if(m_dqCmds.empty()){
			std::cout << "\t --> ALL GONE!!!!\n";
		}
		std::cout << std::endl;
	}


	void CommandList::setUpIterator(){
		m_itCurrNode = m_dqCmds.begin();
	}

	CommandNode * CommandList::getNextCmd(){
		CommandNode * rv = &*m_itCurrNode;
		if(m_itCurrNode == m_dqCmds.end()){
			m_itCurrNode = m_dqCmds.begin();
		}else {
			++m_itCurrNode;
		}
		return rv;
	}
