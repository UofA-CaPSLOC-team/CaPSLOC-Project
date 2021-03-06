/*
 * SendToCTRL.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: dt02
 */

#include "SendToCTRL.h"
//From http://curl.haxx.se/libcurl/c/postit2.html

SendToCTRL::SendToCTRL() {

}

SendToCTRL::~SendToCTRL() {

}



void SendToCTRL::sendPicToCTRL(std::string filename, std::string altName, double longitude, double latitude, double altitude, std::string locname, std::string capTime){
	CURL *curl;
	CURLcode res;

	std::string ipAddr = m_strIPAddr;
	ipAddr.append(PIC_ROUTE);
	std::cout << "Sending Picture to " << ipAddr << "\n";
	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect:";
	curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "file",
			CURLFORM_FILE, filename.c_str(),
			CURLFORM_END);

	/* Fill in the filename field */
	curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "filename",
			CURLFORM_COPYCONTENTS, filename.c_str(),
			CURLFORM_END);

	curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "altName",
			CURLFORM_CONTENTTYPE, "text/plain",
			CURLFORM_COPYCONTENTS, altName.c_str(),
			CURLFORM_END);

	curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "latitude",
			CURLFORM_CONTENTTYPE, "text/plain",
			CURLFORM_COPYCONTENTS, boost::lexical_cast<std::string>(latitude).c_str(),
			CURLFORM_END);

	curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "longitude",
			CURLFORM_CONTENTTYPE, "text/plain",
			CURLFORM_COPYCONTENTS, boost::lexical_cast<std::string>(longitude).c_str(),
			CURLFORM_END);

	curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "altitude",
			CURLFORM_CONTENTTYPE, "text/plain",
			CURLFORM_COPYCONTENTS, boost::lexical_cast<std::string>(altitude).c_str(),
			CURLFORM_END);

	curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "locName",
			CURLFORM_CONTENTTYPE, "text/plain",
			CURLFORM_COPYCONTENTS, locname.c_str(),
			CURLFORM_END);

	curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "captureTime",
			CURLFORM_CONTENTTYPE, "text/plain",
			CURLFORM_COPYCONTENTS, capTime.c_str(),
			CURLFORM_END);

	/* Fill in the submit field too, even if this is rarely needed */
	curl_formadd(&formpost,
			&lastptr,
			CURLFORM_COPYNAME, "submit",
			CURLFORM_COPYCONTENTS, "send",
			CURLFORM_END);

	curl = curl_easy_init();
	/* initalize custom header list (stating that Expect: 100-continue is not
		     wanted */
	headerlist = curl_slist_append(headerlist, buf);
	if(curl) {
		/* what URL that receives this POST */
		curl_easy_setopt(curl, CURLOPT_URL, ipAddr.c_str());

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
		/* free slist */
		curl_slist_free_all (headerlist);
	}
}

void SendToCTRL::sendCommandDebug(std::string cmdinfo){
	CURL *curl;
	CURLcode res;

	std::string ipAddr = m_strIPAddr;
	ipAddr.append(DEBUG_ROUTE);
	std::cout << "Sending Debug to " << ipAddr << "\n";
	struct curl_slist *headerlist=NULL;
	std::string jsonObj = "AltName=ALT1&Message=";
	//TODO: Add variable for AltName.
	jsonObj.append(curl_easy_escape(curl, cmdinfo.c_str(), 0));

	curl = curl_easy_init();

	struct curl_slist *headers = NULL;
	curl_slist_append(headers, "Accept: */*");

	curl_easy_setopt(curl, CURLOPT_URL, ipAddr.c_str());

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj.c_str());

	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

//	/* always cleanup */
	curl_easy_cleanup(curl);
//
//	/* free slist */
	curl_slist_free_all (headerlist);
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

double SendToCTRL::sendGPS(GPSCoordinates coordinates)
{
	CURL *curl;
	CURLcode res;
	std::string readBuffer;
	double altitude = std::numeric_limits<double>::min();

	std::string ipAddr = m_strIPAddr;

	ipAddr.append(ALTITUDE_ROUTE);
	// Append longitude and latitude
	ipAddr.append("?longitude=");
	ipAddr.append(boost::lexical_cast<std::string>(coordinates.longitude));
	ipAddr.append("&latitude=");
	ipAddr.append(boost::lexical_cast<std::string>(coordinates.latitude));

	struct curl_slist *headerlist=NULL;

	curl = curl_easy_init();

	struct curl_slist *headers = NULL;
	curl_slist_append(headers, "Accept: */*");

	curl_easy_setopt(curl, CURLOPT_URL, ipAddr.c_str());

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

	std::cout << "Sending to IP address: " << ipAddr;
	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK){
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
	}
	else{  // We should have data
		std::cout << "Data Received: \n";
		std::cout << readBuffer << "\n";

		int offset = readBuffer.find("\"data\":") + 7;
		altitude = strtod(readBuffer.substr(offset, std::string::npos).c_str(), NULL);
	}

	//	/* always cleanup */
		curl_easy_cleanup(curl);
	//
	//	/* free slist */
		curl_slist_free_all (headerlist);

		return altitude;
}



void SendToCTRL::setIPAddr(std::string ip){
	curl_global_init(CURL_GLOBAL_ALL);
	m_strIPAddr = HTTP_BEGIN;
	m_strIPAddr.append(ip);
	m_strIPAddr.append(SENDTO_PORT);
}
