/*
 * BoostSocketServer.h
 *
 *  Created on: Mar 27, 2014
 *      Author: dt02
 */

#ifndef BOOSTSOCKETSERVER_H_
#define BOOSTSOCKETSERVER_H_

#include <cstdio>
#include <iostream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "../InProc/BoostParse.h"
#include "../../DPU/SendToCTRL.h"

#define UDSLOCATION "/opt/CaPSLOC/socket/socket"

using boost::asio::local::stream_protocol;

class session
		: public boost::enable_shared_from_this<session>
{
public:
	session(boost::asio::io_service& io_service, BoostParse * bp, SendToCTRL * stc)
: socket_(io_service), m_bp(bp), m_stc(stc)
{
}

	stream_protocol::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		std::cout << "Session Started!\n";
		socket_.async_read_some(boost::asio::buffer(data_),
				boost::bind(&session::handle_read,
						shared_from_this(),
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
	}

	void handle_read(const boost::system::error_code& error,
			size_t bytes_transferred)
	{
		if (!error)
		{
			std::string str = this->getBufferData();
			std::string rstr;
			std::cout << str;
			if(str[0] == '/'){
				m_bp->scriptFileParse(str);
				rstr = "Received Script from Server at File Location ";
				rstr.append(str);
				m_stc->sendCommandDebug(rstr);
			}else {
				m_bp->addManualCommand(str);
				rstr = "Received Command String from Server: ";
				rstr.append(str);
				m_stc->sendCommandDebug(rstr);
			}
		}
	}

	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			socket_.async_read_some(boost::asio::buffer(data_),
					boost::bind(&session::handle_read,
							shared_from_this(),
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));

		}
	}

	std::string getBufferData(){
		std::string rv(data_.begin(), data_.end());
		return rv;
	}
private:
	// The socket used to communicate with the client.
	stream_protocol::socket socket_;

	// Buffer used to store data received from the client.
	boost::array<char, 1024> data_;

	BoostParse * m_bp;
	SendToCTRL * m_stc;
};

typedef boost::shared_ptr<session> session_ptr;

class server
{
public:
	server(boost::asio::io_service& io_service, const std::string& file, BoostParse * bp, SendToCTRL * stc)
: io_service_(io_service),
  acceptor_(io_service, stream_protocol::endpoint(file)),
  m_bp(bp),
  m_stc(stc)
{
		std::cout << "Creating Server\n";
		session_ptr new_session(new session(io_service_, m_bp, m_stc));
		acceptor_.async_accept(new_session->socket(),
				boost::bind(&server::handle_accept, this, new_session,
						boost::asio::placeholders::error));
}

	void handle_accept(session_ptr new_session,
			const boost::system::error_code& error)
	{
		if (!error)
		{

			new_session->start();
		}
		std::cout << "Starting Session\n";
		new_session.reset(new session(io_service_, m_bp, m_stc));
		acceptor_.async_accept(new_session->socket(),
				boost::bind(&server::handle_accept, this, new_session,
						boost::asio::placeholders::error));
	}

	void setData(boost::array<char, 1024> data){
		data_ = data;
	}

private:
	boost::asio::io_service& io_service_;
	stream_protocol::acceptor acceptor_;
	BoostParse * m_bp;
	SendToCTRL * m_stc;
	boost::array<char, 1024> data_;

};

class BoostSocketServer {
public:
	BoostSocketServer(BoostParse * bp, SendToCTRL * stc);
	virtual ~BoostSocketServer();

private:

};

#endif /* BOOSTSOCKETSERVER_H_ */
