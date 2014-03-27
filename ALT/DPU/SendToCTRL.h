/*
 * SendToCTRL.h
 *
 *  Created on: Mar 11, 2014
 *      Author: dt02
 */

#ifndef SENDTOCTRL_H_
#define SENDTOCTRL_H_

#define HTTP_BEGIN "http://"

#define SENDTO_PORT ":80"

#define DEBUG_ROUTE "/capsloc/alt/debug"
#define PIC_ROUTE "/capsloc/image/save"

#include <curl/curl.h>
#include <string>
#include <boost/lexical_cast.hpp>


//#define CURL

class SendToCTRL {
public:
	SendToCTRL(std::string strIP);
	virtual ~SendToCTRL();

	void sendPicToCTRL(std::string filename, std::string altName, double longitude, double latitude, double altitude, std::string locname, std::string capTime);
	void sendCommandDebug(std::string cmdinfo);

private:
	std::string m_strIPAddr;
};

#endif /* SENDTOCTRL_H_ */
