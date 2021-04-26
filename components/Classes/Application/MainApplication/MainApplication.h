/*
 * MainApplication.h
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "esp_system.h"

#include <vector>
#include <algorithm>

#include "../../Employee/Employee.h"
#include "../../Device/Device.h"
#include "../../Networking/Messaging/MessagingService.h"
#include "../../RfidScanner/RfidScanner.h"
#include "../../Networking/Wifi/Wifi.h"
#include "../TagProcessing/TagProcessing.h"
#include "../../Display/DisplayDriver.h"
#include "../../Networking/WebServer/WebServer.h"

#include "../../eventLoopSkeleton/eventSkeleton.h"

#define MAIN_APP "MAIN_APP"

#ifndef COMPONENTS_CLASSES_MAINAPPLICATION_MAINAPPLICATION_H_
#define COMPONENTS_CLASSES_MAINAPPLICATION_MAINAPPLICATION_H_


/** \brief Application Code - Initialization, startup code and loop.
 *
 */
class MainApplication {
private:

	std::vector<Employee> employeeList;
	std::vector<Device> deviceList;

	// Private state machine functions
	DataProcessing DataProcessor;
	TagProcessing TagProcessor;

	//Display
	DisplayDriver DisplayProcessor;

public:
	MainApplication();
	void runConfig(); // Startup Config
	void runLoop(); // Main Loop
};

#endif /* COMPONENTS_CLASSES_MAINAPPLICATION_MAINAPPLICATION_H_ */
