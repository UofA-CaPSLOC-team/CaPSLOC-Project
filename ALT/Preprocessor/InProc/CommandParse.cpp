/*
 * CommandParse.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: dt02
 */

#include "CommandParse.h"

CommandParse::CommandParse() {
	//Initialize command queues
	m_dqManualCmd = new std::deque<CommandID>();
	m_vScriptCmd = new std::vector<CommandID>();
}

CommandParse::~CommandParse() {
	// TODO Auto-generated destructor stub
}

bool CommandParse::addManualCommand(std::string strManualCmd){

	//            m_strCommand = strManualCmd;
	m_strFilename = "NULL";
	parseCommand(strManualCmd);

	return true;
}


//TODO add ability to append to the current script or replace old script
bool CommandParse::scriptFileParse(std::string strFileName){

	m_strFilename = strFileName;
	parseFile();

	return true;
}

//Includes code examples from http://www.dinomage.com/2012/01/tutorial-using-tinyxml-part-1/
bool CommandParse::parseFile(){
	m_xmlScriptFile = TiXmlDocument(m_strFilename.c_str());
	//Load the script file. If it doesn't exist, throw and error.
	if(m_xmlScriptFile.LoadFile()){
		std::cerr << m_xmlScriptFile.ErrorDesc() << std::endl;
		return false;
	}
	//The root element is what contains all the other elements, so this is how we can access them.
	//This finds the <script></script> element
	TiXmlElement * root = m_xmlScriptFile.FirstChildElement();

	//If there is no root element, we are very angry. Give the user an error to show our angst!
	if(root == NULL){
		std::cerr << "Failed to load file: No root element. Why would you trick me like this?!?!" << std::endl;
		m_xmlScriptFile.Clear();
		return false;
	}
	CommandID * currID;
	CommandID * sub;

	//CommandID(std::string type, std::string attribute, Source src, std::string value, CommandType parentType);
	for(TiXmlElement * elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){

		m_vScriptCmd->push_back(CommandID(elem->Value(), "", SCRIPT_FILE, "", SCFILE));
		currID = &(m_vScriptCmd->back());
		switch(currID->getType()){
		//Look at the elements that have sub-elements
		case CONFIG:
		case GOTO:
		case CAPTURE:
		case RMOTION:
		case WAIT:
		case EXEC:
			for(TiXmlNode * subelem = elem->FirstChild(); subelem != NULL; subelem = subelem->NextSibling()){
				// all subelements except locoffset are text elements
				if(subelem->ValueTStr() == "locoffset"){
					//One more set of sub-elements
					sub = new CommandID(subelem->Value(), "", SCRIPT_FILE, "", currID->getType());
					for(TiXmlNode * subsubelem = subelem->FirstChild(); subsubelem != NULL; subsubelem = subsubelem->NextSibling()){
						TiXmlText * subsubtext = subsubelem->ToText();
						sub->addData(new CommandID(subsubelem->Value(), "", SCRIPT_FILE, subsubtext->Value(), sub->getType()));
					}
				}else{
					TiXmlText * subtext = subelem->ToText();
					sub = new CommandID(subelem->Value(), "", SCRIPT_FILE, subtext->Value(), currID->getType());
				}
				currID->addData(sub);
			}
			break;
		default:
			//No sub-elements? This element has already been added to the list, so just keep going!
			break;
		}
	}

	return true;
}

bool CommandParse::parseCommand(std::string strManualCmd){
	m_xmlScriptFile.Parse(strManualCmd.c_str(), 0, TIXML_ENCODING_UTF8);
	//The root element is what contains all the other elements, so this is how we can access them.
	//This finds the <command></command> element
	TiXmlElement * root = m_xmlScriptFile.FirstChildElement();

	//If there is no root element, we are very angry. Give the user an error to show our angst!
	if(root == NULL){
		std::cerr << "Failed to read command from stream: No root element." << std::endl;
		m_xmlScriptFile.Clear();
		return false;
	}
	//TODO Add way to determine whether the Source should be labeled "USER_STD" or "USER_ADMIN" using the "user" attribute of the command element.
	Source src = USER_STD;
	CommandID * currID;
	CommandID * sub;
	std::string text;
	//CommandID(std::string type, std::string attribute, Source src, std::string value, CommandType parentType);
	for(TiXmlElement * elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){
		m_dqManualCmd->push_back(CommandID(elem->Value(), "", USER_STD, "", SCFILE));
		currID = &(m_dqManualCmd->back());
		switch(currID->getType()){
		//Look at the elements that have sub-elements
		case CONFIG:
		case GOTO:
		case CAPTURE:
		case RMOTION:
		case WAIT:
		case EXEC:
			for(TiXmlNode * subelem = elem->FirstChild(); subelem != NULL; subelem = subelem->NextSibling()){
				// all subelements except locoffset are text elements
				text = subelem->Value();
				if(!text.compare("locoffset")){
					//One more set of sub-elements
					sub = new CommandID(subelem->Value(), "", src, "", currID->getType());
					for(TiXmlNode * subsubelem = subelem->FirstChild(); subsubelem != NULL; subsubelem = subsubelem->NextSibling()){
						TiXmlText * subsubtext = subsubelem->ToText();
						sub->addData(new CommandID(subsubelem->Value(), "", src, subsubtext->Value(), sub->getType()));
					}
				}else{
					TiXmlText * subtext = subelem->ToText();
					sub = new CommandID(subelem->Value(), "", src, subtext->Value(), currID->getType());
				}
				currID->addData(sub);
			}
			break;
		default:
			//No sub-elements? This element has already been added to the list, so just keep going!
			break;
		}
	}
	return true;
}

