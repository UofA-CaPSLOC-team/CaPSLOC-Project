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

typedef enum RelativeDirection{
	RIGHT,
	LEFT,
	UP,
	DOWN
}RelativeDirection;

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

	void setNext(CommandNode * newNode){m_ptrNext = newNode;}
	void setPrev(CommandNode * newNode){m_ptrPrev = newNode;}
	bool hasNext(){return (m_ptrNext != NULL);}
	CommandType getType(){return m_tType;}

	CommandNode * getPrev(){return m_ptrPrev;}
	CommandNode * getNext(){return m_ptrNext;}

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

	//TODO Massive amounts of getters/setters

private:
	CommandNode * m_ptrPrev, * m_ptrNext;
	CommandType m_tType;
	double m_dLong, m_dLat, m_dAlt;
	std::string m_strName;
	CaptureMode m_capMode;
	long m_lTOT;
	short m_sFrameRate;
	int m_nQuality;
	long m_lWaittime;
//	std::string m_strName;
	RelativeDirection m_rDir;
	double m_dAngle;
};

#ifndef COMMANDNODE_ALT
class CommandNode{
public:
	CommandNode(CommandType tType = NOTYPE){
		m_ptrPrev = 0;
		m_ptrNext = 0;
		m_tType = tType;
	}

	virtual ~CommandNode(){
		delete m_ptrPrev;
		delete m_ptrNext;
	}

	bool hasNext(){return (m_ptrNext != 0);}
    CommandType getType(){return m_tType;}

	void setPrev(CommandNode * prev){m_ptrPrev = prev;}
	void setNext(CommandNode * next){m_ptrNext = next;}
	CommandNode * getPrev(){return m_ptrPrev;}
	CommandNode * getNext(){return m_ptrNext;}

protected:
	CommandNode * m_ptrPrev, * m_ptrNext;
	CommandType m_tType;
};

class GotoNode: public CommandNode{
public:
	GotoNode(double longitude, double latitude, double altitude, std::string name): CommandNode(GOTO), m_dLong(longitude),
																	m_dLat(latitude),
																	m_dAlt(altitude),
																	m_strName(name){
	}
	~GotoNode(){}

	double getLongitude(){return m_dLong;}
	double getLatitude(){return m_dLat;}
	double getAltitude(){return m_dAlt;}
	std::string getName(){return m_strName;}

private:
	double m_dLong, m_dLat, m_dAlt;
	std::string m_strName;

};

class CaptureNode: public CommandNode{
public:
	CaptureNode(CaptureMode mode, long tot=-1,
				short framerate=-1, int quality=-1): CommandNode(CAPTURE), m_capMode(mode),
														m_lTOT(tot),
														m_sFrameRate(framerate),
														m_nQuality(quality){
	}
	~CaptureNode(){}

	CaptureMode getCaptureMode(){return m_capMode;}
	long getTimeOnTarget(){return m_lTOT;}
	short getFramerate(){return m_sFrameRate;}
	int getQuality(){return m_nQuality;}

private:
	CaptureMode m_capMode;
	long m_lTOT;
	short m_sFrameRate;
	int m_nQuality;
};

class WaitNode: public CommandNode{
public:
	WaitNode(long time): CommandNode(WAIT), m_lWaittime(time){
	}
	~WaitNode(){}

	long getWaitTime(){return m_lWaittime;}
private:
	long m_lWaittime;
};

class ExecNode: public CommandNode{
public:
	ExecNode(std::string name): CommandNode(EXEC), m_strName(name){
	}
	~ExecNode(){}

	std::string getName(){return m_strName;}
private:
	std::string m_strName;
};

class RmotionNode: public CommandNode{
public:
	RmotionNode(RelativeDirection dir, double angle): CommandNode(RMOTION), m_rDir(dir), m_dAngle(angle){
	}
	~RmotionNode(){}

	RelativeDirection getDirection(){return m_rDir;}
	double getAngle(){return m_dAngle;}
private:
		RelativeDirection m_rDir;
		double m_dAngle;
};

class HaltNode: public CommandNode{
public:
	HaltNode(): CommandNode(HALT){
	}
	~HaltNode(){}
};

class PauseNode: public CommandNode{
public:
	PauseNode(): CommandNode(PAUSE){
	}

	~PauseNode(){}
};

class ResumeNode: public CommandNode{
public:
	ResumeNode(): CommandNode(RESUME){
	}

	~ResumeNode(){}
};
#endif
#endif /* COMMANDNODE_H_ */

