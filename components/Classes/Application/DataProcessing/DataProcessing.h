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
#include "../../Display/DisplayDriver.h"
#include "../../Networking/WebServer/WebServer.h"

extern "C" {
	#include "cJSON.h"
}

#define DATA_PROCESSING "DATA PROCESSING"

#ifndef COMPONENTS_CLASSES_APPLICATION_DATAPROCESSING_DATAPROCESSING_H_
#define COMPONENTS_CLASSES_APPLICATION_DATAPROCESSING_DATAPROCESSING_H_

/** \brief Application code, Provides interaction between communications, employee models and device models
 *		Provides a state machine which decides what to do with the messages
 *
 */
class DataProcessing {
private:


	EmployeeModel employees;
	DeviceModel devices;

	DisplayDriver display;
	SntpTime globalTime;

	message recvdMessage;
	void commandReceived();
public:
	WebServer myServer;
	DataProcessing();
	DataProcessing(DisplayDriver& myDriver);
	void init();
	void doMessageProcessing();
	MessagingService getMessagingService();

	void sendDevices();
	void sendEmployees();
	void addDevice(std::string data);
	void addEmployee(std::string data);

	EmployeeModel& getEmployeeModel();
	DeviceModel& getDeviceModel();

	void printLists();
};

#endif /* COMPONENTS_CLASSES_APPLICATION_CLIENTPROCESSING_CLIENTPROCESSING_H_ */
