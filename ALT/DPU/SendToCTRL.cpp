/*
 * SendToCTRL.cpp
 *
 *  Created on: Mar 11, 2014
 *      Author: dt02
 */

#include "SendToCTRL.h"
//From http://curl.haxx.se/libcurl/c/postit2.html

//TODO modify to fit needs.
SendToCTRL::SendToCTRL() {
	curl_global_init(CURL_GLOBAL_ALL);
}

SendToCTRL::~SendToCTRL() {

}

void SendToCTRL::sendPicToCTRL(std::string filename, std::string altName, double longitude, double latitude, double altitude, std::string locname, std::string capTime){
	CURL *curl;
	CURLcode res;

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
		curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:3000/pics");

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

	struct curl_httppost *formpost=NULL;
	struct curl_httppost *lastptr=NULL;
	struct curl_slist *headerlist=NULL;
	std::string jsonObj = "{ \"cmdInfo\" : \"";
	jsonObj.append(cmdinfo);
	jsonObj.append("\" }");

	curl = curl_easy_init();

	struct curl_slist *headers = NULL;
	curl_slist_append(headers, "Accept: application/json");
	curl_slist_append(headers, "Content-Type: application/json");
	curl_slist_append(headers, "charsets: utf-8");

	curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:3000/debug");

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj.c_str());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcrp/0.1");

	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

//	/* always cleanup */
	curl_easy_cleanup(curl);
//
//	/* then cleanup the formpost chain */
	curl_formfree(formpost);
//	/* free slist */
	curl_slist_free_all (headerlist);
}