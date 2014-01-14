/*
 * CommandParse.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: dt02
 */

#include "CommandParse.h"

CommandParse::CommandParse(std::string fileCommand, bool bScript) {
	m_bScript = bScript;

	if(m_bScript){
		m_strFilename = fileCommand;
		m_strCommand = "NULL";
		parseFile();
	}else {
		m_strCommand = fileCommand;
		m_strFilename = "NULL";
		parseCommand();
	}
}

CommandParse::~CommandParse() {
	// TODO Auto-generated destructor stub
}

bool CommandParse::parseFile(){
	m_xmlScriptFile = TiXmlDocument(m_strFilename.c_str());
	bool loadOk = m_xmlScriptFile.LoadFile();

	TiXmlNode

	return loadOk;
}

bool CommandParse::parseCommand(){
	m_xmlScriptFile.Parse(m_strCommand.c_str(), 0, TIXML_ENCODING_UTF8);
	return true;
}

