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
#include "../MCPM/MCPM.h"
#include "DPU/SendToCTRL.h"
//#include <boost/thread.hpp>
#include <curl/curl.h>
#include "Preprocessor/SocketComm/BoostSocketServer.h"

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

void runCmdH(CommandHandler * ch){
	ch->execNext();
}

int main(int argv, char ** argc){
//	std::string fn = "/opt/CaPSLOC/scripts/test_script.xml";
	std::cout << "*****BEGIN*****" << std::endl;
	std::cout << "\t --> Creating CommandParse object." << std::endl;

	BoostParse * bp = new BoostParse();
//	std::cout << "\t --> Parsing " << fn << "\n\n";
//	bp->scriptFileParse(fn);
	std::cout << "\n\t --> Creating CommandHandler object." << std::endl;

	SendToCTRL * stc = new SendToCTRL();
//	stc->sendPicToCTRL("/opt/CaPSLOC/pics/Accepted.jpg", "ALT 1", 40.296, -80.125, 360, "Somewhere Over the Rainbow", "13:55");
//	stc->sendPicToCTRL("/home/dt02/curl.html", "ALT 2", -40.296, 80.125, 360, "Somewhere Under the Rainbow", "14:89");
//	stc->sendPicToCTRL("/home/pi/ALT/ALT/main.cpp", "ALT 2", -26.28, 41.903, 2, "On the Pi!", "15:17"); //TESTING on PI!
//	stc->sendCommandDebug("DEBUG is working!");

	CommandHandler * ch = new CommandHandler(bp, stc);
	std::cout << "\t --> Executing scripts..." << std::endl;

	bp->setCmdH(new CommandHandler(bp, stc));

	 try
	  {
	    boost::asio::io_service io_service;
	    std::remove(UDSLOCATION);
	    server s(io_service, UDSLOCATION, bp, stc);

	    boost::thread bt(boost::bind(static_cast<std::size_t (boost::asio::io_service::*) ()>(&boost::asio::io_service::run), &io_service));


	    std::string ipaddr;//don't set this in real code.
	    while(ipaddr.empty()){
		    //Get IP address from config file in /opt/CaPSLOC/config.txt, look for "CTRL="
	    	//Test to make sure this works... (JMP - It didn't but I fixed it)
	    	ipaddr = getCTRLIp();
	    	usleep(50);
	    }

		stc->setIPAddr(ipaddr);
	    boost::thread bt2(runCmdH, ch);


	    // NO CODE BELOW THIS LINE
	    bt.join();
	  }
	  catch (std::exception& e)
	  {
	    std::cerr << "Exception: " << e.what() << "\n";
	  }

//	ch->execNext();
//	stc->~SendToCTRL();


	return 0;
}



