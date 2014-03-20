/*
 * SendToCTRL.h
 *
 *  Created on: Mar 11, 2014
 *      Author: dt02
 */

#ifndef SENDTOCTRL_H_
#define SENDTOCTRL_H_

#include <curl/curl.h>
#include <string>
#include <boost/lexical_cast.hpp>


//#define CURL

class SendToCTRL {
public:
	SendToCTRL();
	virtual ~SendToCTRL();

	void sendPicToCTRL(std::string filename, std::string altName, double longitude, double latitude, double altitude, std::string locname, std::string capTime);
	void sendCommandDebug(std::string cmdinfo);

private:
//	CURL *curl;
//	CURLcode res;
//
//	struct curl_httppost *formpost=NULL;
//	struct curl_httppost *lastptr=NULL;
//	struct curl_slist *headerlist=NULL;

};

#endif /* SENDTOCTRL_H_ */
