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
#include "CommandID.h"


class CommandParse {
public:
	/**
	 * Coming soon: Documentation
	 */
	CommandParse(std::string fileCommand, bool bScript);

	virtual ~CommandParse();

	bool parseFile();

	bool parseCommand();

private:

	bool m_bScript;
	TiXmlDocument m_xmlScriptFile;
	std::string m_strCommand;
	std::string m_strFilename;
	std::vector<CommandID> m_vScriptCmd, m_vManualCmd;

};

#endif /* COMMANDPARSE_H_ */
