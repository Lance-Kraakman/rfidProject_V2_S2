/*
 * MainApplication.h
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */

//#include "esp_wifi.h"
//#include "nvs_flash.h"
//#include <sys/param.h>
//#include "esp_netif.h"
//#include "esp_eth.h"
//#include "freertos/queue.h"
//#include "esp_wifi.h"
//#include "esp_event.h"
//#include "driver/gpio.h"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "esp_system.h"

#include <vector>
#include <algorithm>

#include "../Employee/Employee.h"
#include "../Device/Device.h"
#include "../Networking/Messaging/MessagingService.h"
#include "../RfidScanner/RfidScanner.h"
#include "../Networking/Wifi/Wifi.h"

#define MAIN_APP "MAIN_APP"

#ifndef COMPONENTS_CLASSES_MAINAPPLICATION_MAINAPPLICATION_H_
#define COMPONENTS_CLASSES_MAINAPPLICATION_MAINAPPLICATION_H_

class MainApplication {
private:
	Wifi WifiHandler;
	MessagingService messagingService;
	RfidScanner rfidScanner;
	std::vector<Employee> employeeList;
	std::vector<Device> deviceList;
	SntpTime globalTime;

	// Private state machine functions


public:
	MainApplication();
	void runConfig(); // Startup Config
	void runLoop(); // Main Loop
	void messageProcessing(message& recvdMessage); // Process messages and functions based on this
	void tagProcessing(RfidTag& recvdTag, RfidTag& previousTag, Employee& lastScannedEmployee); // Process tags and functions based on this
	bool findDevice(Device& deviceToCheck);
	bool findEmployee(Employee& EmployeeToCheck);
	bool updateDeviceInList(Device& deviceToUpdate);
	bool updateEmployeeInList(Employee& EmployeeToUpdate);
	bool checkTagTimeDuration(RfidTag tagFirst, RfidTag taggedSecond, int maxSeconds);
};

#endif /* COMPONENTS_CLASSES_MAINAPPLICATION_MAINAPPLICATION_H_ */
