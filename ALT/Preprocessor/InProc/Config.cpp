/*
 * Config.cpp
 *
 *  Created on: Feb 24, 2014
 *      Author: edwin
 */

#include "Config.h"


Config::Config() {
	m_dLatOffset = 0;
	m_dLongOffset = 0;
	m_dAltOffset = 0;
	m_dRMotionAngle = 0;
	m_lVidtime = 0;
	m_lWaitTime = 0;
	m_nQuality = 600;
	m_tCapMode = PIC;
	m_sFrameRate = 15;
}

Config::~Config() {
}

