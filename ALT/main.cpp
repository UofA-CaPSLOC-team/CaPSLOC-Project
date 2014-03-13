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
#include <curl/curl.h>

int main(int argv, char ** argc){
	std::string fn = "/opt/CaPSLOC/scripts/test_script.xml";
	std::cout << "*****BEGIN*****" << std::endl;
	std::cout << "\t --> Creating CommandParse object." << std::endl;
	BoostParse * bp = new BoostParse();
	std::cout << "\t --> Parsing " << fn << "\n\n";
	bp->scriptFileParse(fn);
	std::cout << "\n\t --> Creating CommandHandler object." << std::endl;
	CommandHandler * ch = new CommandHandler(bp->getManualCommands(), bp->getScriptCommands());
	std::cout << "\t --> Executing scripts..." << std::endl;
	ch->execNext();


	return 0;
}



