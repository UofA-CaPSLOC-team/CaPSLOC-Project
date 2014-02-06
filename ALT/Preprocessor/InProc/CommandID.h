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
	SCFILE,

	CONFIG,
	GOTO, //Call to MCPM
	HALT,
	PAUSE,
	RESUME,
	CAPTURE,//Call to MCPM
	RMOTION,//Call to MCPM
	WAIT,
	EXEC,

	LATOFFSET,
	LONGOFFSET,
	ALTOFFSET,
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
	WAITTIME,
	NOTYPE,
	ERROR

}CommandType;

typedef enum Priority{
	SCRIPTFILE,
	STD_MANUAL,
	ADMIN_MANUAL,
	EMERGENCY
}Priority;

typedef enum Source{
	SCRIPT_FILE,
	USER_STD,
	USER_ADMIN
}Source;

typedef enum CaptureMode{
	PIC,
	VID
}CaptureMode;

typedef enum RelativeDirection{
	RIGHT,
	LEFT,
	UP,
	DOWN
}RelativeDirection;

class CommandID {
public:
	CommandID(std::string type, std::string attribute, Source src, std::string value, CommandType parentType);
	virtual ~CommandID();

	/**
	 * addData allows sub-elements of scripts to be added to this CommandID's vector.
	 *
	 * @param newData: pointer to new CommandID element.
	 */
	void addData(CommandID * newData);

	//GETTER METHODS
	CommandType getType(){return m_tType;}
	Priority getPriority(){return m_tPriority;}
	std::string getAttributes(){return m_strAttribute;}
	std::vector<CommandID> * getSubElements(){return m_vCmdData;}
	std::string getValue(){return m_strValue;}
	bool hasSubElements(){return hasSubElems;}

private:
	CommandType m_tType;
	CommandType m_tParentType;
	Source m_tSource;
	Priority m_tPriority;
	std::string m_strAttribute;
	std::string m_strValue;
	std::vector<CommandID> * m_vCmdData;
	bool hasSubElems;

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
