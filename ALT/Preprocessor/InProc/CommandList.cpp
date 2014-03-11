/*
 * CommandList.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: edwin
 */

#include "CommandList.h"

CommandList::CommandList() {
	m_lNumCmds = 0;
#ifndef VECTOR
	m_ptrHead = NULL;
	m_ptrEnd = NULL;
#else

#endif
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
#ifndef VECTOR
	if(m_lNumCmds < 1){
		m_ptrHead = m_ptrEnd = &newNode;
	}else {
		newNode.setPrev(m_ptrEnd);
		m_ptrEnd->setNext(&newNode);
		m_ptrEnd = m_ptrEnd->getNext();
	}
#else
	m_dqCmds.push_back(newNode);
#endif
	m_lNumCmds++;

}


CommandNode * CommandList::pop_front(){
#ifndef VECTOR
	CommandNode * rv = m_ptrHead;
	if(m_ptrHead->hasNext()){
		m_ptrHead = m_ptrHead->getNext();
	}
	//	delete m_ptrHead->getPrev();
	m_ldNumCmds--;

	//Value can return as NULL if nothing is in the list..
	//Be sure to use hasCommands() before popping.
	return rv;
#else
	CommandNode * rv = &m_dqCmds.front();
	m_dqCmds.pop_front();
	return rv;
#endif
}

void CommandList::printList(){
	std::cout << "\n\nContents of CommandList:\n";

#ifndef VECTOR
	CommandNode * currNode = m_ptrHead;
	while(1){//currNode != m_ptrEnd){
#else
for(std::deque<CommandNode>::iterator currNode = m_dqCmds.begin(); currNode != m_dqCmds.end(); ++currNode){
#endif
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
#ifndef VECTOR
		if(currNode->hasNext()){
			currNode = (currNode->getNext());
		}else{
			break;
		}
#endif
	}
	if(m_dqCmds.empty()){
		std::cout << "\t --> ALL GONE!!!!\n";
	}
	std::cout << std::endl;
}
