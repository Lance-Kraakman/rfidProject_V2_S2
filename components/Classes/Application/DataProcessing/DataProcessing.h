/*
 * clientProcessing.h
 *
 *  Created on: 7/04/2021
 *      Author: lance
 */
#include "../../Networking/Wifi/Wifi.h"
#include "../../Networking/Messaging/MessagingService.h"
#include "../../Employee/Employee.h"
#include "../../EmployeeModel/EmployeeModel.h"
#include "../../Device/Device.h"
#include "../../DeviceModel/DeviceModel.h"
#include "../../MyTime/SntpTime.h"
#include <vector>
#include <algorithm>

extern "C" {
	#include "cJSON.h"
}

#define DATA_PROCESSING "DATA PROCESSING"

#ifndef COMPONENTS_CLASSES_APPLICATION_DATAPROCESSING_DATAPROCESSING_H_
#define COMPONENTS_CLASSES_APPLICATION_DATAPROCESSING_DATAPROCESSING_H_

class DataProcessing {
private:
	Wifi WifiHandler;
	MessagingService messagingService;
	EmployeeModel employees;
	DeviceModel devices;
	SntpTime globalTime;

	message recvdMessage;
	void commandReceived();
public:
	DataProcessing();
	void init();
	void doMessageProcessing();
	MessagingService getMessagingService();

	void sendDevices();
	void sendEmployees();
	void addDevice();
	void addEmployee();

	EmployeeModel& getEmployeeModel();
	DeviceModel& getDeviceModel();

	void printLists();
};

#endif /* COMPONENTS_CLASSES_APPLICATION_CLIENTPROCESSING_CLIENTPROCESSING_H_ */
