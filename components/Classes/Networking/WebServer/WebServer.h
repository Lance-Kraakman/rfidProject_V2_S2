/*
 * WebServer.h
 *
 *  Created on: 18/04/2021
 *      Author: lance
 */

#include "../WebServerSkeleton/WebServerSkeleton.h"
#include <string>

#define CONFIG_MAX_NUM_RESPONSES 5

#ifndef COMPONENTS_CLASSES_NETWORKING_MYWEBSERVER_MYWEBSERVER_H_
#define COMPONENTS_CLASSES_NETWORKING_MYWEBSERVER_MYWEBSERVER_H_

class httpResponse {
public:
	httpResponse() {};
	httpResponse(std::string data, int dataSize, std::string uriString, std::string responseType, httpd_method_t method);
	SemaphoreHandle_t dataSemaphore = NULL;
	bool deleteDataOnResponse = false;
	std::string uriString;
	std::string responseType;
	std::string data;
	int dataSize;
	void respond(httpd_req_t *req);
	void respond_get(httpd_req_t *req);
	void respond_post(httpd_req_t *req);
	bool dataRead = true;


	httpd_method_t method;

	void updateData(std::string data);
	void updateData(const char *data);
	void deleteData();
	std::string getData();
	void addToData(std::string data);
	void addToData(char *data);

private:
};

/** Web Server Class. Add pages and shit leshgooo
 *
 */
class WebServer : public WebServerSkeleton {
public:
	WebServer();
	void addResponse(httpResponse *httpResponse);
	static void response(const char *responseType, const char *data, ssize_t bufferLen, httpd_req_t *req);
	static void find_uri_and_respond(httpd_req_t *req);
private:
	static esp_err_t handler(httpd_req_t *req);
	static httpd_uri_t uri_list[CONFIG_MAX_NUM_RESPONSES];
	static httpResponse *responseList[CONFIG_MAX_NUM_RESPONSES];
	static int pageCount;


};

#endif /* COMPONENTS_CLASSES_NETWORKING_WEBSERVER_WEBSERVER_H_ */
