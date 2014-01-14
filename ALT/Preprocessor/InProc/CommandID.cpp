/*
 * CommandID.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: dt02
 */

#include "CommandID.h"


CommandID::CommandID(std::string type, std::string attribute = "", Source src, std::string value = "", CommandType parentType = CommandType::NOTYPE) {
	m_tParentType = parentType;
	defineType(type);
	m_strAttribute = attribute;
	m_strValue = value;
	m_tParentType = parentType;
	m_tSource = src;
}

CommandID::~CommandID() {
	// TODO Auto-generated destructor stub
}

inline void CommandID::defineType(std::string strType){
	if(!strType.compare("script")){
		m_tType = CommandType::SCRIPT;
	}else if(!strType.compare("command")){
		m_tType = CommandType::COMMAND;
	}else if(m_tParentType == CommandType::SCRIPT || m_tParentType == CommandType::COMMAND){
		if(!strType.compare("config")){
			m_tType = CommandType::CONFIG;
		}else if(!strType.compare("goto")){
			m_tType = CommandType = GOTO;
		}else if(!strType.compare("halt")){
			m_tType = CommandType::HALT;
			//TODO IMMEDIATELY HALT EVERYTHING!
			//TODO Alert CTRL unit of imminent shutdown.
			system("shutdown -h 5"); //Shutdown system in 5 seconds.
		}else if(!strType.compare("pause")){
			m_tType = CommandType::PAUSE;
		}else if(!strType.compare("resume")){
			m_tType = CommandType = RESUME;
		}else if(!strType.compare("capture")){
			m_tType = CommandType = CAPTURE;
		}else if(!strType.compare("rmotion")){
			m_tType = CommandType = RMOTION;
		}else if(!strType.compare("wait")){
			m_tType = CommandType::WAIT;
		}else if(!strType.compare("exec")){
			m_tType = CommandType::EXEC;
		}else {
			m_tType = CommandType::ERROR;
		}
	}else if(m_tParentType == CommandType::CONFIG){
		if(!strType.compare("rmotionangle")){
			m_tType = CommandType::RMOTIONANGLE;
		}else if(!strType.compare("vidtime")){
			m_tType = CommandType::VIDTIME;
		}else if(!strType.compare("framerate")){
			m_tType = CommandType::FRAMERATE;
		}else if(!strType.compare("imagemode")){
			m_tType = CommandType::IMAGEMODE;
		}else if(!strType.compare("quality")){
			m_tType = CommandType::QUALITY;
		}else if(!strType.compare("waitime")){
			m_tType = CommandType::WAITTIME;
		}else if(!strType.compare("locoffset")){
			m_tType = CommandType::LOCOFFSET;
		}else {
			m_tType = CommandType::ERROR;
		}
	}else if(m_tParentType == CommandType::LOCOFFSET || m_tParentType == CommandType::GOTO){
		if(!strType.compare("longitude")){
			m_tType = CommandType::LONGITUDE;
		}else if(!strType.compare("latitude")){
			m_tType = CommandType::LATITUDE;
		}else if(!strType.compare("altitude")){
			m_tType = CommandType::ALTITUDE;
		}else {
			m_tType = CommandType::ERROR;
		}
	}else if(!strType.compare("name")){
		if(m_tParentType == CommandType::EXEC || m_tParentType == CommandType::GOTO){
			m_tType = CommandType::NAME;
		}else {
			m_tType = CommandType::ERROR;
		}
	}else if(!strType.compare("time")){
		if(m_tParentType == CommandType::WAIT){
			m_tType = CommandType::TIME;
		}else {
			m_tType = CommandType::ERROR;
		}
	}else {
		m_tType = CommandType::ERROR;
	}
}
