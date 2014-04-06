/*
 * BoostParse.h
 *
 *  Created on: Feb 24, 2014
 *      Author: edwin
 */

#ifndef BOOSTPARSE_H_
#define BOOSTPARSE_H_

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <fstream>
#include <sstream>
#include "CommandNode.h"
#include "CommandList.h"
#include "../CmdH/CommandHandler.h"
#include "Config.h"

#define SCRIPTLOC "/opt/CaPSLOC/scripts/"

class CommandHandler;


class BoostParse {
public:
	/**
	 * CommandParse:
	 *
	 * Class that contains necessary data structures and algorithms to
	 * properly parse a script file to make it ready to use by the
	 * Command Handler.
	 */
	BoostParse();

	/**
	 * Unimplemented as of yet.
	 */
	virtual ~BoostParse();

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
	CommandList * getScriptCommands(){return m_cmdScript;}

	/**
	 * getManualCommands does exactly what the name suggests.
	 *
	 * @return pointer to the whole Vector of CommandID elements parsed from manual input.
	 */
	CommandList * getManualCommands(){return m_cmdManual;}

	Config * getConfig(){return m_cnfg;}

	/**
	 * hasManualCommands shows whether the manual command queue contains any elements.
	 *
	 * @return true if commands are present in the queue.
	 * @return false if no commands are present in the queue.
	 */
	bool hasManualCommands(){return m_cmdManual->hasCommands();}

	/**
	 * hasScriptCommands shows whether the script command queue contains any elements.
	 *
	 * @return true if commands are present in the queue.
	 * @return false if no commands are present in the queue.
	 */
	bool hasScriptCommands(){return m_cmdScript->hasCommands();}

	void setCmdH(CommandHandler * cmdH){
		m_cmdH = cmdH;
	}

private:

	boost::property_tree::ptree m_xmlScriptFile;
	std::string m_strFilename;
	CommandList * m_cmdScript, * m_cmdManual;
	CommandHandler * m_cmdH;
//	std::vector<CommandID> * m_vScriptCmd;
//	std::deque<CommandID> * m_dqManualCmd;
	Config * m_cnfg;

};

#endif /* BOOSTPARSE_H_ */
