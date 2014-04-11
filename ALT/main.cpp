/*
 * main.cpp
 *
 *  Created on: Feb 20, 2014
 *      Author: dt02
 */

//System includes
#include <iostream>
#include <string>

#include <unistd.h>
#include <sched.h>
#include <cstdio>

//Local includes
#include "Preprocessor/InProc/BoostParse.h"
#include "Preprocessor/CmdH/CommandHandler.h"
#include "../MCPM/MCPM.h"
#include "DPU/SendToCTRL.h"
//#include <boost/thread.hpp>
#include <curl/curl.h>
//#include "Preprocessor/SocketComm/BoostSocketServer.h"

#define CNFGFILELOC "/opt/CaPSLOC/config.txt"

std::string getCTRLIp(){
	char rawResult[256];
	std::string rv;
	std::ifstream is(CNFGFILELOC, std::ifstream::in);
	while(is.is_open() && rv.empty() && !is.eof()){
		is.getline(rawResult, 256);
		rv = rawResult;
		if(rv.find("CTRL=") != std::string::npos){
			rv = rv.substr(5, std::string::npos);
		} else {
			rv.clear();
		}
	}
	return rv;
}




int main(int argc, char ** argv){
	std::cout << "*****BEGIN*****" << std::endl;
	std::cout << "\t --> Creating CommandParse object." << std::endl;

	BoostParse * bp = new BoostParse();

	if(argc >= 2){
		std::cout << "Usage " << argv[0] << " <filename>\n";

		return 0;
	} else {
		bp->scriptFileParse(argv[1]);
	}

	std::cout << "\n\t --> Creating CommandHandler object." << std::endl;

	SendToCTRL * stc = new SendToCTRL();

	CommandHandler * ch = new CommandHandler(bp, stc);
	std::cout << "\t --> Executing scripts..." << std::endl;
	ch->execNext();

	return 0;
}



