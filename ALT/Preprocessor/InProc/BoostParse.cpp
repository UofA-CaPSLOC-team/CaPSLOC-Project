/*
 * BoostParse.cpp
 *
 *  Created on: Feb 24, 2014
 *      Author: edwin
 */

#include "BoostParse.h"

BoostParse::BoostParse() {
	m_cmdScript = new CommandList();
	m_cmdManual = new CommandList();
	m_cnfg = new Config();
}

BoostParse::~BoostParse() {
}

inline long timeParse(std::string time){
	long rv, hours = 0, mins = 0, secs = 0, msecs = 0;
	std::string delimiter = ":";
	size_t pos = 0;
	std::string token;
	short count = 0;
	while ((pos = time.find(delimiter)) != std::string::npos) {
		token = time.substr(0, pos);
		if(count == 0){
			hours = atol(token.c_str());
		}else if(count == 1){
			mins = atol(token.c_str());
		}else if(count == 2){
			secs = atol(token.c_str());
		}else if(count == 3){
			msecs = atol(token.c_str());
		}
		time.erase(0, pos + delimiter.length());
		count++;
	}

	rv = (((((hours * 60) + mins) * 60) + secs) * 1000) + msecs;

	return rv;
}

inline RelativeDirection directionParse(std::string direction){
	if(!direction.compare("left")){
		return LEFT;
	}else if(!direction.compare("right")){
		return RIGHT;
	}else if(!direction.compare("up")){
		return UP;
	}else if(!direction.compare("down")){
		return DOWN;
	}else {
		return RIGHT;
	}
}

bool BoostParse::parseFile(){
	//TODO Implement using Boost Library
	return true;
}

bool BoostParse::addManualCommand(std::string strManualCmd){
	//TODO Implement using Boost Library
	return true;
}

bool BoostParse::scriptFileParse(std::string strFileName){
	//TODO Implement using Boost Library
	std::ifstream xmlFile(strFileName.c_str(), std::ifstream::in);
	CommandNode newNode = CommandNode();
	read_xml(xmlFile, m_xmlScriptFile);

	BOOST_FOREACH(boost::property_tree::ptree::value_type const &v, m_xmlScriptFile.get_child("script")){
		if(v.first == "config"){
			//TODO May need to check that these exist.
			m_cnfg->setRMotionAngle(v.second.get<double>("rmotionangle"));
			m_cnfg->setVidTime(v.second.get<long>("vidtime"));
			m_cnfg->setFrameRate(v.second.get<short>("framerate"));
			m_cnfg->setCaptureMode(((v.second.get<std::string>("imagemode").compare("pic")) ? VID : PIC));
			m_cnfg->setQuality(v.second.get<int>("quality"));
			m_cnfg->setWaitTime(timeParse(v.second.get<std::string>("waittime")));
			m_cnfg->setLatOffset(v.second.get<double>("latoffset"));
			m_cnfg->setLongOffset(v.second.get<double>("longoffset"));
			m_cnfg->setAltOffset(v.second.get<double>("altoffset"));
			//END CONFIG
		}else if(v.first == "goto"){
			float latitude = v.second.get<float>("latitude");
			float longitude = v.second.get<float>("longitude");
			float altitude = v.second.get<float>("altitude");
			std::string name = v.second.get<std::string>("name");

			std::cout << "\t --> Parsing GOTO: \n";
			std::cout << "\t\t --> Lat: " << latitude << "\n";
			std::cout << "\t\t --> Long: " << longitude << "\n";
			std::cout << "\t\t --> Alt: " << altitude << "\n";
			std::cout << "\t\t --> Name: " << name << std::endl;

			newNode.setGoto(latitude,
					longitude,
					altitude,
					name);
			m_cmdScript->push_back(newNode);

			//END GOTO
		}else if(v.first == "halt"){
			std::cout << "\t --> Parsing HALT: " << std::endl;

			newNode.setHalt();
			m_cmdScript->push_back(newNode);
			//END HALT
		}else if(v.first == "pause"){
			std::cout << "\t --> Parsing PAUSE: " << std::endl;
			newNode.setPause();
			m_cmdScript->push_back(newNode);
			//END PAUSE
		}else if(v.first == "resume"){
			std::cout << "\t --> Parsing RESUME: " << std::endl;
			newNode.setResume();
			m_cmdScript->push_back(newNode);
			//END RESUME
		}else if(v.first == "capture"){
			CaptureMode cm = v.second.get<std::string>("mode").compare("pic") ? VID : PIC;
			long tot = timeParse(v.second.get<std::string>("tot"));
			short fr = v.second.get<short>("framerate");
			int qual = v.second.get<int>("quality");

			std::cout << "\t --> Parsing CAPTURE: \n";
			std::cout << "\t\t --> Mode: " << ((cm == PIC) ? "PIC" : "VID") << "\n";
			std::cout << "\t\t --> TOT: " << tot << "\n";
			std::cout << "\t\t --> FrameRate: " << fr << "\n";
			std::cout << "\t\t --> Quality: " << qual << std::endl;

			newNode.setCapture(cm,
					tot,
					fr,
					qual);
			m_cmdScript->push_back(newNode);
			//END CAPTURE
		}else if(v.first == "rmotion"){
			RelativeDirection rd = directionParse(v.second.get<std::string>("direction"));
			double ang = v.second.get<double>("angle");

			std::cout << "\t --> Parsing RMOTION: \n";
			std::cout << "\t\t --> Direction: " << rd << "\n";
			std::cout << "\t\t --> Angle: " << ang << std::endl;

			newNode.setRMotion(rd, ang);
			m_cmdScript->push_back(newNode);
			//END RMOTION
		}else if(v.first == "wait"){
			long time = timeParse(v.second.get<std::string>("time"));

			std::cout << "\t --> Parsing WAIT: \n";
			std::cout << "\t\t --> Time: " << time << std::endl;

			newNode.setWait(time);
			m_cmdScript->push_back(newNode);
			//END WAIT
		}else if(v.first == "exec"){
			std::string name = v.second.get<std::string>("name");

			std::cout << "\t --> Parsing EXEC: \n";
			std::cout << "\t\t --> Name: " << name << std::endl;

			newNode.setExec(name);
			m_cmdScript->push_back(newNode);
			//END EXEC
		}else {
			//Skip erroneous element.
			//END DEFAULT
		}
	}
	return true;
}

bool BoostParse::parseCommand(std::string strManualCmd){
	//TODO Implement using Boost Library
	return true;
}
