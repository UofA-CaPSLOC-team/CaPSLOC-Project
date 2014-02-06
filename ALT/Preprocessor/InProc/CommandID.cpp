/*
 * CommandID.cpp
 *
 *  Created on: Nov 14, 2013
 *      Author: dt02
 */

#include "CommandID.h"


CommandID::CommandID(std::string type, std::string attribute = "", Source src = SCRIPT_FILE, std::string value = "", CommandType parentType = NOTYPE) {
	m_tParentType = parentType;
	defineType(type);
	m_strAttribute = attribute;
	m_strValue = value;
	m_tParentType = parentType;
	m_tSource = src;
	hasSubElems = false;
}

CommandID::~CommandID() {
	// TODO Auto-generated destructor stub
}

void CommandID::addData(CommandID * newData){
	const CommandID * nd = newData;
	m_vCmdData->push_back(*nd);
	hasSubElems = true;
}

inline void CommandID::defineType(std::string strType){
	if(!strType.compare("script")){
		m_tType = SCFILE;
	}else if(!strType.compare("command")){
		m_tType = COMMAND;
	}else if((m_tParentType == SCFILE) || (m_tParentType == COMMAND)){
		if(!strType.compare("config")){
			m_tType = CONFIG;
		}else if(!strType.compare("goto")){
			m_tType = GOTO;
		}else if(!strType.compare("halt")){
			m_tType = HALT;
			//TODO IMMEDIATELY HALT EVERYTHING!
			//TODO Alert CTRL unit of imminent shutdown.
		}else if(!strType.compare("pause")){
			m_tType = PAUSE;
		}else if(!strType.compare("resume")){
			m_tType = RESUME;
		}else if(!strType.compare("capture")){
			m_tType = CAPTURE;
		}else if(!strType.compare("rmotion")){
			m_tType = RMOTION;
		}else if(!strType.compare("wait")){
			m_tType = WAIT;
		}else if(!strType.compare("exec")){
			m_tType = EXEC;
		}else {
			m_tType = ERROR;
		}
	}else if(m_tParentType == CONFIG){
		if(!strType.compare("rmotionangle")){
			m_tType = RMOTIONANGLE;
		}else if(!strType.compare("vidtime")){
			m_tType = VIDTIME;
		}else if(!strType.compare("framerate")){
			m_tType = FRAMERATE;
		}else if(!strType.compare("imagemode")){
			m_tType = IMAGEMODE;
		}else if(!strType.compare("quality")){
			m_tType = QUALITY;
		}else if(!strType.compare("waitime")){
			m_tType = WAITTIME;
		}else if(!strType.compare("longoffset")){
			m_tType = LONGOFFSET;
		}else if(!strType.compare("latoffset")){
			m_tType = LATOFFSET;
		}else if(!strType.compare("altoffset")){
			m_tType = ALTOFFSET;
		}else {
			m_tType = ERROR;
		}
	}else if(m_tParentType == GOTO){
		if(!strType.compare("longitude")){
			m_tType = LONGITUDE;
		}else if(!strType.compare("latitude")){
			m_tType = LATITUDE;
		}else if(!strType.compare("altitude")){
			m_tType = ALTITUDE;
		}else {
			m_tType = ERROR;
		}
	}else if(!strType.compare("name")){
		if(m_tParentType == EXEC || m_tParentType == GOTO){
			m_tType = NAME;
		}else {
			m_tType = ERROR;
		}
	}else if(!strType.compare("time")){
		if(m_tParentType == WAIT){
			m_tType = TIME;
		}else {
			m_tType = ERROR;
		}
	}else {
		m_tType = ERROR;
	}
}
