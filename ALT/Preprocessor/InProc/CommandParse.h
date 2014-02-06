/*
 * CommandParse.h
 *
 *  Created on: Nov 14, 2013
 *      Author: dt02
 */

#ifndef COMMANDPARSE_H_
#define COMMANDPARSE_H_

#include <iostream>
#include <fstream>
#include "../tinyxml/tinyxml.h"
#include <string>
#include <vector>
#include <deque>
#include "CommandID.h"


class CommandParse {
public:
	/**
	 * CommandParse:
	 *
	 * Class that contains necessary data structures and algorithms to
	 * properly parse a script file to make it ready to use by the
	 * Command Handler.
	 */
	CommandParse();

	/**
	 * Unimplemented as of yet.
	 */
	virtual ~CommandParse();

	/**
	 * parseFile parses a script file from the file indicated by m_strFilename.
	 * Adds script elements to the m_vScriptCmd vector to be used by the command handler.
	 *
	 * @return true: parse was successful
	 * @return false: parse was unsuccessful
	 */
	bool parseFile();

	/**
	 * addManualCommand allows a manual command to be added to the manual command queue
	 *
	 * @param strManualCmd: manual command string.
	 */
	bool addManualCommand(std::string strManualCmd);

	/**
	 * scriptFileParse allows a script file to be switched into the script
	 *
	 * @param strFileName: file name of the script file
	 */
	//TODO add ability to append to the current script or replace old script
	bool scriptFileParse(std::string strFileName);

	/**
	 * parseCommand parses the command string contained in m_strCommand and adds the commands to
	 * the m_vManualCmd vector to be used by the command handler.
	 *
	 * @param strManualCmd: manual command string.
	 */
	bool parseCommand(std::string strManualCmd);

	/**
	 * getScriptCommands does exactly what the name suggests.
	 *
	 * @return pointer to a Vector of CommandID elements parsed from a script.
	 */
	std::vector<CommandID> * getScriptCommands(){return m_vScriptCmd;}

	/**
	 * getManualCommands does exactly what the name suggests.
	 *
	 * @return pointer to the whole Vector of CommandID elements parsed from manual input.
	 */
	std::deque<CommandID> * getManualCommands(){return m_dqManualCmd;}

	/**
	 * hasManualCommands shows whether the manual command queue contains any elements.
	 *
	 * @return true if commands are present in the queue.
	 * @return false if no commands are present in the queue.
	 */
	bool hasManualCommands(){return !m_dqManualCmd->empty();}

	/**
	 * hasScriptCommands shows whether the script command queue contains any elements.
	 *
	 * @return true if commands are present in the queue.
	 * @return false if no commands are present in the queue.
	 */
	bool hasScriptCommands(){return !m_vScriptCmd->empty();}

private:

	TiXmlDocument m_xmlScriptFile;
	std::string m_strFilename;
	std::vector<CommandID> * m_vScriptCmd;
	std::deque<CommandID> * m_dqManualCmd;

};

#endif /* COMMANDPARSE_H_ */
