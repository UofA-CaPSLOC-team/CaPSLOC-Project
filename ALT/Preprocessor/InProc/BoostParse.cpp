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
	m_cmdH = new CommandHandler();
}

BoostParse::~BoostParse() {
}

inline bool exists_test(const std::string& name) {
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
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
	std::istringstream ss(strManualCmd);
	CommandNode newNode = CommandNode();
	try{
		read_xml(ss, m_xmlScriptFile);
	}catch(boost::property_tree::xml_parser_error& e){
		return false;
	}

	BOOST_FOREACH(boost::property_tree::ptree::value_type const &v, m_xmlScriptFile.get_child("command")){
		if(v.first == "config"){
			//TODO May need to check that these exist.
			try{
				m_cnfg->setRMotionAngle(v.second.get<double>("rmotionangle"));
			} catch(std::exception * e){
				m_cnfg->setRMotionAngle(5);
			}
			try{
				m_cnfg->setVidTime(v.second.get<long>("vidtime"));
			} catch(std::exception * e){
				m_cnfg->setVidTime(5);
			}
			try{
				m_cnfg->setFrameRate(v.second.get<short>("framerate"));
			} catch(std::exception * e){
				m_cnfg->setFrameRate(30);
			}
			try{
				m_cnfg->setCaptureMode(((v.second.get<std::string>("imagemode").compare("pic")) ? VID : PIC));
			} catch(std::exception * e){
				m_cnfg->setCaptureMode(PIC);
			}
			try{
				m_cnfg->setQuality(v.second.get<int>("quality"));
			} catch(std::exception * e){
				m_cnfg->setQuality(600);
			}
			try{
				m_cnfg->setWaitTime(timeParse(v.second.get<std::string>("waittime")));
			} catch(std::exception * e){
				m_cnfg->setWaitTime(5000);
			}
			try{
				m_cnfg->setLatOffset(v.second.get<double>("latoffset"));
			} catch(std::exception * e){
				m_cnfg->setLatOffset(0);
			}
			try{
				m_cnfg->setLongOffset(v.second.get<double>("longoffset"));
			} catch(std::exception * e){
				m_cnfg->setLongOffset(0);
			}
			try{
				m_cnfg->setAltOffset(v.second.get<double>("altoffset"));
			} catch(std::exception * e){
				m_cnfg->setAltOffset(0);
			}
			//END CONFIG
		}else if(v.first == "goto"){
			float latitude, longitude, altitude;
			std::string name;
			try{
				latitude = v.second.get<float>("latitude");
			} catch(std::exception * e){
				latitude = 0;
			}
			try{
				longitude = v.second.get<float>("longitude");
			} catch(std::exception * e){
				longitude = 0;
			}
			try{
				altitude = v.second.get<float>("altitude");
			} catch(std::exception * e){
				altitude = 0;
			}
			try{
				name = v.second.get<std::string>("name");
			} catch(std::exception * e){
				name = "Unknown Name";
			}

			std::cout << "\t --> Parsing GOTO: \n";
			std::cout << "\t\t --> Lat: " << latitude << "\n";
			std::cout << "\t\t --> Long: " << longitude << "\n";
			std::cout << "\t\t --> Alt: " << altitude << "\n";
			std::cout << "\t\t --> Name: " << name << std::endl;

			newNode.setGoto(latitude,
					longitude,
					altitude,
					name);
			m_cmdManual->push_back(newNode);

			//END GOTO
		}else if(v.first == "halt"){
			std::cout << "\t --> Parsing HALT: " << std::endl;

			newNode.setHalt();
			m_cmdManual->push_back(newNode);
			//END HALT
		}else if(v.first == "pause"){
			std::cout << "\t --> Parsing PAUSE: " << std::endl;
			newNode.setPause();
			m_cmdManual->push_back(newNode);
			//END PAUSE
		}else if(v.first == "resume"){
			std::cout << "\t --> Parsing RESUME: " << std::endl;
			newNode.setResume();
			m_cmdManual->push_back(newNode);
			//END RESUME
		}else if(v.first == "capture"){
			CaptureMode cm;
			long tot;
			short fr;
			int qual;
			try{
				cm = v.second.get<std::string>("mode").compare("pic") ? VID : PIC;
			} catch(std::exception * e){
				cm = m_cnfg->getCaptureMode();
			}
			try{
				tot = timeParse(v.second.get<std::string>("tot"));
			} catch(std::exception * e){
				tot = m_cnfg->getVidTime();
			}
			try{
				fr = v.second.get<short>("framerate");
			} catch(std::exception * e){
				fr = m_cnfg->getFrameRate();
			}
			try{
				qual = v.second.get<int>("quality");
			} catch(std::exception * e){
				qual = m_cnfg->getQuality();
			}

			std::cout << "\t --> Parsing CAPTURE: \n";
			std::cout << "\t\t --> Mode: " << ((cm == PIC) ? "PIC" : "VID") << "\n";
			std::cout << "\t\t --> TOT: " << tot << "\n";
			std::cout << "\t\t --> FrameRate: " << fr << "\n";
			std::cout << "\t\t --> Quality: " << qual << std::endl;

			newNode.setCapture(cm,
					tot,
					fr,
					qual);
			m_cmdManual->push_back(newNode);
			//END CAPTURE
		}else if(v.first == "rmotion"){
			RelativeDirection rd;
			double ang;
			try{
				rd = directionParse(v.second.get<std::string>("direction"));
			} catch(std::exception * e){
				rd = RIGHT;
			}
			try{
				ang = v.second.get<double>("angle");
			} catch(std::exception * e){
				ang = m_cnfg->getRMotionAngle();
			}

			std::cout << "\t --> Parsing RMOTION: \n";
			std::cout << "\t\t --> Direction: " << rd << "\n";
			std::cout << "\t\t --> Angle: " << ang << std::endl;

			newNode.setRMotion(rd, ang);
			m_cmdManual->push_back(newNode);
			//END RMOTION
		}else if(v.first == "wait"){
			long time;
			try{
				time = timeParse(v.second.get<std::string>("time"));
			} catch(std::exception * e){
				time = m_cnfg->getWaitTime();
			}

			std::cout << "\t --> Parsing WAIT: \n";
			std::cout << "\t\t --> Time: " << time << std::endl;

			newNode.setWait(time);
			m_cmdScript->push_back(newNode);
			//END WAIT
		}else if(v.first == "exec"){
			std::string name = SCRIPTLOC;
			std::string fName;
			try{
				fName = v.second.get<std::string>("name");
			} catch(std::exception * e){
				fName = "NoScriptNameGiven";
			}
			name.append(fName);
			name.append(".xml");
			//			std::string name = v.second.get<std::string>("name");

			std::cout << "\t --> Parsing EXEC: \n";
			std::cout << "\t\t --> Name: " << name << std::endl;

			newNode.setExec(name);
			m_cmdManual->push_back(newNode);
			//END EXEC
		}else {
			//Skip erroneous element.
			//END DEFAULT
		}
	}
	return true;
}

bool BoostParse::scriptFileParse(std::string strFileName){
	//TODO Implement using Boost Library
	CommandNode newNode;
	try{
		std::ifstream xmlFile(strFileName.c_str(), std::ifstream::in);
		newNode = CommandNode();
		read_xml(xmlFile, m_xmlScriptFile);
	} catch(std::exception * e){
		std::cerr << "Cannot read file.\n";
	}
	//TODO Halt script execution NOW!!!!
	m_cmdH->smoothHalt();
	m_cmdScript->clear(); //clear script before putting new one into memory

	BOOST_FOREACH(boost::property_tree::ptree::value_type const &v, m_xmlScriptFile.get_child("script")){
		if(v.first == "config"){
			//TODO May need to check that these exist.
			try{
				m_cnfg->setRMotionAngle(v.second.get<double>("rmotionangle"));
			} catch(std::exception * e){
				m_cnfg->setRMotionAngle(5);
			}
			try{
				m_cnfg->setVidTime(v.second.get<long>("vidtime"));
			} catch(std::exception * e){
				m_cnfg->setVidTime(5);
			}
			try{
				m_cnfg->setFrameRate(v.second.get<short>("framerate"));
			} catch(std::exception * e){
				m_cnfg->setFrameRate(30);
			}
			try{
				m_cnfg->setCaptureMode(((v.second.get<std::string>("imagemode").compare("pic")) ? VID : PIC));
			} catch(std::exception * e){
				m_cnfg->setCaptureMode(PIC);
			}
			try{
				m_cnfg->setQuality(v.second.get<int>("quality"));
			} catch(std::exception * e){
				m_cnfg->setQuality(600);
			}
			try{
				m_cnfg->setWaitTime(timeParse(v.second.get<std::string>("waittime")));
			} catch(std::exception * e){
				m_cnfg->setWaitTime(5000);
			}
			try{
				m_cnfg->setLatOffset(v.second.get<double>("latoffset"));
			} catch(std::exception * e){
				m_cnfg->setLatOffset(0);
			}
			try{
				m_cnfg->setLongOffset(v.second.get<double>("longoffset"));
			} catch(std::exception * e){
				m_cnfg->setLongOffset(0);
			}
			try{
				m_cnfg->setAltOffset(v.second.get<double>("altoffset"));
			} catch(std::exception * e){
				m_cnfg->setAltOffset(0);
			}
			//END CONFIG
		}else if(v.first == "goto"){
			float latitude, longitude, altitude;
			std::string name;
			try{
				latitude = v.second.get<float>("latitude");
			} catch(std::exception * e){
				latitude = 0;
			}
			try{
				longitude = v.second.get<float>("longitude");
			} catch(std::exception * e){
				longitude = 0;
			}
			try{
				altitude = v.second.get<float>("altitude");
			} catch(std::exception * e){
				altitude = 0;
			}
			try{
				name = v.second.get<std::string>("name");
			} catch(std::exception * e){
				name = "Unknown Name";
			}
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
			CaptureMode cm;
			long tot;
			short fr;
			int qual;
			try{
				cm = v.second.get<std::string>("mode").compare("pic") ? VID : PIC;
			} catch(std::exception * e){
				cm = m_cnfg->getCaptureMode();
			}
			try{
				tot = timeParse(v.second.get<std::string>("tot"));
			} catch(std::exception * e){
				tot = m_cnfg->getVidTime();
			}
			try{
				fr = v.second.get<short>("framerate");
			} catch(std::exception * e){
				fr = m_cnfg->getFrameRate();
			}
			try{
				qual = v.second.get<int>("quality");
			} catch(std::exception * e){
				qual = m_cnfg->getQuality();
			}
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
			RelativeDirection rd;
			double ang;
			try{
				rd = directionParse(v.second.get<std::string>("direction"));
			} catch(std::exception * e){
				rd = RIGHT;
			}
			try{
				ang = v.second.get<double>("angle");
			} catch(std::exception * e){
				ang = m_cnfg->getRMotionAngle();
			}
			std::cout << "\t --> Parsing RMOTION: \n";
			std::cout << "\t\t --> Direction: " << rd << "\n";
			std::cout << "\t\t --> Angle: " << ang << std::endl;

			newNode.setRMotion(rd, ang);
			m_cmdScript->push_back(newNode);
			//END RMOTION
		}else if(v.first == "wait"){
			long time;
			try{
				time = timeParse(v.second.get<std::string>("time"));
			} catch(std::exception * e){
				time = m_cnfg->getWaitTime();
			}

			std::cout << "\t --> Parsing WAIT: \n";
			std::cout << "\t\t --> Time: " << time << std::endl;

			newNode.setWait(time);
			m_cmdScript->push_back(newNode);
			//END WAIT
		}else if(v.first == "exec"){
			//This could break. Test that script actually exists,
			// If it doesn't, exclude this element from the script.
			std::string name = SCRIPTLOC;
			std::string fName;
			try{
				fName = v.second.get<std::string>("name");
			} catch(std::exception * e){
				fName = "NoScriptNameGiven";
			}
			name.append(fName);
			name.append(".xml");
			std::cout << "\t --> Parsing EXEC: \n";
			std::cout << "\t\t --> Name: " << name << std::endl;

			newNode.setExec(name);

			if(exists_test(name)){
				m_cmdScript->push_back(newNode);
			}
			//END EXEC
		}else {
			//Skip erroneous element.
			//END DEFAULT
		}
	}
	m_cmdH->startFromHalt();
	return true;
}

bool BoostParse::parseCommand(std::string strManualCmd){
	//TODO Implement using Boost Library
	return true;
}
