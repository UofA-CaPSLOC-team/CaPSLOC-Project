/*
 * BoostSocketServer.cpp
 *
 *  Created on: Mar 27, 2014
 *      Author: dt02
 */

#include "BoostSocketServer.h"

BoostSocketServer::BoostSocketServer(BoostParse * bp, SendToCTRL * stc) {
	 try
	  {
	    boost::asio::io_service io_service;
	    std::remove(UDSLOCATION);
	    server s(io_service, UDSLOCATION, bp, stc);

//	    boost::asio::io_service io_service;
//	    boost::asio::io_service::work work(io_service);

	    boost::thread bt(boost::bind(static_cast<std::size_t (boost::asio::io_service::*) ()>(&boost::asio::io_service::run), &io_service));
	    bt.join();
//	    bt.detach();
	  }
	  catch (std::exception& e)
	  {
	    std::cerr << "Exception: " << e.what() << "\n";
	  }

}

BoostSocketServer::~BoostSocketServer() {
}

