/*
 * MainApplication.h
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include <sys/param.h>
#include "esp_system.h"
#include "esp_event.h"

#include <vector>
#include "../Employee/Employee.h"
#include "../Networking/Messaging/MessagingService.h"
#include "../RfidScanner/RfidScanner.h"

#ifndef COMPONENTS_CLASSES_MAINAPPLICATION_MAINAPPLICATION_H_
#define COMPONENTS_CLASSES_MAINAPPLICATION_MAINAPPLICATION_H_

class MainApplication {
private:
	std::vector<Employee> employeeList;
public:
	MainApplication();
	void runConfig(); // Startup Config
	void runLoop(); // Main Loop
};

#endif /* COMPONENTS_CLASSES_MAINAPPLICATION_MAINAPPLICATION_H_ */
