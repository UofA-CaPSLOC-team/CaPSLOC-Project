/*
 * CommandNode.h
 *
 *  Created on: Feb 25, 2014
 *      Author: dt02
 */


#ifndef COMMANDNODE_H_
#define COMMANDNODE_H_

#define COMMANDNODE_ALT

#include <string>
#include "../../ControlLogic/MCPM/MCPM.h"

typedef enum CommandType{
	NOTYPE,
	CONFIG,
	GOTO, //Call to MCPM
	HALT,
	PAUSE,
	RESUME,
	CAPTURE,//Call to MCPM
	RMOTION,//Call to MCPM
	WAIT,
	EXEC,

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

/*typedef enum RelativeDirection{
	RIGHT,
	LEFT,
	UP,
	DOWN
}RelativeDirection;*/

class CommandNode{
public:
	CommandNode();

	virtual ~CommandNode();

	void setGoto(double longitude, double latitude, double altitude, std::string name);
	void setCapture(CaptureMode mode, long tot, short framerate, int quality);
	void setWait(long time);
	void setExec(std::string name);
	void setRMotion(RelativeDirection dir, double angle);
	void setHalt();
	void setPause();
	void setResume();

	CommandType getType(){return m_tType;}

	double getLongitude(){return m_dLong;}
	double getLatitude(){return m_dLat;}
	double getAltitude(){return m_dAlt;}
	std::string getName(){return m_strName;}
	CaptureMode getCapMode(){return m_capMode;}
	long getTimeOnTarget(){return m_lTOT;}
	short getFrameRate(){return m_sFrameRate;}
	int getQuality(){return m_nQuality;}
	long getWaitTime(){return m_lWaittime;}
	RelativeDirection getRelDirection(){return m_rDir;}
	double getAngle(){return m_dAngle;}

	//Massive amounts of getters/setters

private:

	CommandType m_tType;
	double m_dLong, m_dLat, m_dAlt;
	std::string m_strName;
	CaptureMode m_capMode;
	long m_lTOT;
	short m_sFrameRate;
	int m_nQuality;
	long m_lWaittime;
	RelativeDirection m_rDir;
	double m_dAngle;
};

#endif /* COMMANDNODE_H_ */

