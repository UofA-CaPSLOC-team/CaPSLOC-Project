/*
 * Config.h
 *
 *  Created on: Feb 24, 2014
 *      Author: edwin
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "CommandNode.h"

class Config {
public:
	Config();
	virtual ~Config();

	void setLatOffset(double latOff){ m_dLatOffset = latOff;}
	void setLongOffset(double longOff){ m_dLongOffset = longOff;}
	void setAltOffset(double altOff){ m_dAltOffset = altOff;}
	void setRMotionAngle(double rAng){ m_dRMotionAngle = rAng;}

	void setVidTime(long vTime){m_lVidtime = vTime;}
	void setWaitTime(long wTime){m_lWaitTime = wTime;}

	void setQuality(int qual){m_nQuality = qual;}

	void setCaptureMode(CaptureMode cmode){m_tCapMode = cmode;}

	void setFrameRate(short fRate){m_sFrameRate = fRate;}

	double getLatOffset(){return m_dLatOffset;}
	double getLongOffset(){return m_dLongOffset;}
	double getAltOffset(){return m_dAltOffset;}
	double getRMotionAngle(){return m_dRMotionAngle;}

	long getVidTime(){return m_lVidtime;}
	long getWaitTime(){return m_lWaitTime;}

	int getQuality(){return m_nQuality;}

	CaptureMode getCaptureMode(){return m_tCapMode;}

	short getFrameRate(){return m_sFrameRate;}

private:
	double m_dLatOffset, m_dLongOffset, m_dAltOffset, m_dRMotionAngle;
	long m_lVidtime, m_lWaitTime;
	int m_nQuality;
	CaptureMode m_tCapMode;
	short m_sFrameRate;
};

#endif /* CONFIG_H_ */
