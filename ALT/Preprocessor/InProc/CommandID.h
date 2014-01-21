/*
 * CommandID.h
 *
 *  Created on: Nov 14, 2013
 *      Author: dt02
 */

#ifndef COMMANDID_H_
#define COMMANDID_H_

#include <vector>
#include <string>
#include <iostream>

typedef enum CommandType{
	COMMAND,
	SCRIPT,

	CONFIG,
	GOTO,
	HALT,
	PAUSE,
	RESUME,
	CAPTURE,
	RMOTION,
	WAIT,
	EXEC,

	LOCOFFSET,
	LONGITUDE,
	LATITUDE,
	ALTITUDE,
	NAME,
	MODE,
	TOT,
	FRAMERATE,
	QUALITY,
	DIRECTION,
	RMOTIONANGLE,
	ANGLE,
	TIME,
	VIDTIME,
	IMAGEMODE,
	QUALITY,
	WAITTIME,
	NOTYPE,
	ERROR

}CommandType;

typedef enum Priority{
	SCRIPT,
	STD_MANUAL,
	ADMIN_MANUAL,
	EMERGENCY
}Priority;

typedef enum Source{
	SCRIPT_FILE,
	USER_STD,
	USER_ADMIN
}Source;

class CommandID {
public:
	CommandID(std::string type, std::string attribute, Source src, std::string value, CommandType parentType);
	virtual ~CommandID();
	void addData(CommandID *);

	//GETTER METHODS
	CommandType getType(){return m_tType;}
	Priority getPriority(){return m_tPriority;}
	std::string getAttributes(){return m_strAttribute;}
	std::vector<CommandID> getSubElements(){return m_vCmdData;}
	std::string getValue(){return m_strValue;}

private:
	CommandType m_tType;
	CommandType m_tParentType;
	Source m_tSource;
	Priority m_tPriority;
	std::string m_strAttribute;
	std::string m_strValue;
	std::vector<CommandID> m_vCmdData;

	/**
	 *
	 * Assigns an enumerated type to each CommandID to allow for easier processing and
	 * code readability. Immediately executes a "Halt" statement
	 *
	 * @param strType : String of XML Element name.
	 */
	inline void defineType(std::string strType);

};

#endif /* COMMANDID_H_ */
