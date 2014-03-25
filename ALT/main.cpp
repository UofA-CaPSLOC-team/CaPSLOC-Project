/*
 * main.cpp
 *
 *  Created on: Feb 20, 2014
 *      Author: dt02
 */

//System includes
#include <iostream>
#include <string>

//Local includes
#include "Preprocessor/InProc/BoostParse.h"
#include "Preprocessor/CmdH/CommandHandler.h"
#include "ControlLogic/MCPM/MCPM.h"
#include "DPU/SendToCTRL.h"
//#include <boost/thread.hpp>
#include <curl/curl.h>

//void func(void){
//	int x = 1 + 1;
//	std::cout << x<<std::endl;
//}

int main(int argv, char ** argc){
	std::string fn = "/opt/CaPSLOC/scripts/test_script.xml";
	std::cout << "*****BEGIN*****" << std::endl;
	std::cout << "\t --> Creating CommandParse object." << std::endl;

	BoostParse * bp = new BoostParse();
	std::cout << "\t --> Parsing " << fn << "\n\n";
	bp->scriptFileParse(fn);
	std::cout << "\n\t --> Creating CommandHandler object." << std::endl;

	SendToCTRL * stc = new SendToCTRL("130.101.12.131");
	stc->sendPicToCTRL("/opt/CaPSLOC/pics/Accepted.jpg", "ALT 1", 40.296, -80.125, 360, "Somewhere Over the Rainbow", "13:55");
//	stc->sendPicToCTRL("/home/dt02/curl.html", "ALT 2", -40.296, 80.125, 360, "Somewhere Under the Rainbow", "14:89");
//	stc->sendPicToCTRL("/home/pi/ALT/ALT/main.cpp", "ALT 2", -26.28, 41.903, 2, "On the Pi!", "15:17"); //TESTING on PI!
	stc->sendCommandDebug("DEBUG is working!");

	CommandHandler * ch = new CommandHandler(bp->getManualCommands(), bp->getScriptCommands(), stc);
	std::cout << "\t --> Executing scripts..." << std::endl;
//	ch->execNext();
	stc->~SendToCTRL();


	return 0;
}



