/*
 * MainApplication.cpp
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */

#include "MainApplication.h"
#include <string>

// Creaete Pages
//httpResponse emps = httpResponse(std::string("<h1> Default Data <h1>"), HTTPD_RESP_USE_STRLEN, std::string("/emps"), std::string("application/json"), HTTP_GET);
//httpResponse devs = httpResponse(std::string("<h1> Default Data <h1>"), HTTPD_RESP_USE_STRLEN, std::string("/devs"), std::string("application/json"), HTTP_GET);

esp_event_loop_handle_t loop_without_task;
ESP_EVENT_DECLARE_BASE(APPLICATION_CODE_EVENTS);

#define MAX_SECONDS 30

MainApplication::MainApplication() : eventSkeleton()  {
	// TODO Auto-generated constructor stub
	this->tag = RfidTag();
}

#define HELLO_HANDLER 56

void MainApplication::runConfig() {
	this->addHandler(5,(esp_event_handler_t) &MainApplication::instanceHandler, NULL, NULL);
}

static int * dataPtr = NULL;

// Basic State Machine
void MainApplication::runLoop() {

	int number = 5;
	dataPtr = &number;
	int *& dataPtrRef = &dataPtr;

	this->tag.setUUID(0);

	while(1) {

		vTaskDelay(550/portTICK_RATE_MS);
		this->eventPostTo(5, dataPtrRef, 0);
        this->runEventLoop(500);
	}
}

void MainApplication::printHelloHandler() {
	ESP_LOGI(TAG_PROCESSING, "Hello From handler");
}


void MainApplication::instanceHandler(int *num) {
	ESP_LOGI(TAG_PROCESSING, "Hello From handler");
	*dataPtr = 44;
	ESP_LOGI("","handler pointer value %p", dataPtr);
	ESP_LOGI("","handler value %d", *dataPtr);
}



/*
WebServer myServer = WebServer();
vTaskDelay(10/portTICK_RATE_MS);

myServer.addResponse(&emps);
myServer.addResponse(&devs);

ESP_LOGI(TAG_PROCESSING, "Entering loop");
//pageOne.data.assign(std::string("<h1> New Data <h1>"));
char *empJson = "[{\"Letter\": 0 ,\"Freq\": 2858},{\"Letter\" : 31,\"Freq\" : 0}]";
char *devJson = "[{\"Letter\": 1 ,\"Freq\": 45345},{\"Letter\" : 222,\"Freq\" : 1}]";
*/



