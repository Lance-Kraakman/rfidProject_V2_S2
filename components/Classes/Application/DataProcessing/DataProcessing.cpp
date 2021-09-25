/*
 * CommunicationsProcessing.cpp
 *
 *  Created on: 7/04/2021
 *      Author: lance
 */


#include "../DataProcessing/DataProcessing.h"

DataProcessing::DataProcessing() {}

DataProcessing::DataProcessing(DisplayDriver& myDriver) {
	this->display = myDriver;
}

/*
 * config - in the future this will also handle exceptions and stuff
 */
void DataProcessing::init() {

		vTaskDelay(10/portTICK_RATE_MS);

		this->globalTime = SntpTime();
		globalTime.config();
		globalTime.SyncTime();
		globalTime.printTime();
		globalTime.updateToCurrentTime(); // sets the time to the current time

		this->myServer.addResponse(&this->devices.deviceResponse);
		this->myServer.addResponse(&this->employees.employeeResponse);

}

/** State machine which does sends, receives and manipulates data based on messages received
 *
 * @return void
 */
void DataProcessing::doMessageProcessing() {


}

/** Sends device list json to app/app-data MQTT topic
 *
 */
void DataProcessing::sendDevices() {

	cJSON* finalJSON = this->devices.jsonDeviceList();
	char *deviceDataBuffer = cJSON_Print(finalJSON);

	printf("SENDING DATA %s", deviceDataBuffer);

	//Free Memory (Created with Malloc so must use free)
	free(deviceDataBuffer);
	deviceDataBuffer = NULL;

	free(finalJSON);
	finalJSON = NULL;
}

/** Sends employee list json to app/app-data MQTT topic
 *
 */
void DataProcessing::sendEmployees() {

	cJSON* finalJSON = this->employees.jsonEmployeeList();
	char *employeeDataBuffer = cJSON_Print(finalJSON);
	printf("SENDING DATA\n%s", employeeDataBuffer);

	//this->messagingService.sendMessage("app/app-data", employeeDataBuffer, DEFAULT_QOS);

	// Free Memory (Created with Malloc so must use free)
	free(employeeDataBuffer);
	employeeDataBuffer = NULL;

	free(finalJSON);
	finalJSON = NULL;

}

/** Adds device to device model from the received message
 *
 */
void DataProcessing::addDevice(std::string data) {
	cJSON *recvdJSON = cJSON_Parse(data.c_str());
	try {
		Device dev = this->devices.deviceFromJson(recvdJSON);
		this->devices.addDevice(dev); // Adds the device if it is not already in the list
	} catch(const char* msg) {
		printf(msg);
		printf("Employee Not added");
	}

	free(recvdJSON);
	recvdJSON = NULL;
}

/** Adds Employee to device model from the received message
 *
 */
void DataProcessing::addEmployee(std::string data) {
	try {
		// Multiple faults post requests will corrupt memory
		cJSON *recvdJSON = cJSON_Parse(data.c_str());
		Employee emp = this->employees.employeeFromJson(recvdJSON);
		this->employees.addEmployee(emp);
		free(recvdJSON);
		recvdJSON = NULL;

	} catch(const char* msg) {
		printf(msg);
		printf("Employee Not added");
	}




}

/** State-Machine which decides what to do when a command is received.
 *
 */
void DataProcessing::commandReceived() {
	printf("command Message Received\n");

	// Now check all of the commands and
	if (!(recvdMessage.data.compare("send-notification"))) {
		// Send a notification to desktop-application wiith data one
	//	this->messagingService.sendMessage("app/app-status", "1", DEFAULT_QOS);

	} else if (!(recvdMessage.data.compare("send-employee-list"))) {
		// publish the list of empoyees to a topi
		this->sendEmployees();

	} else if (!(recvdMessage.data.compare("send-device-list"))) {
		this->sendDevices();
	}
}

/**
 *
 * @return copy of messaging service
 */
MessagingService DataProcessing::getMessagingService() {
	//return this->messagingService;
	return MessagingService();
}

/** Prints the employee/device lists/models which the dataprocessor has
 *
 */
void DataProcessing::printLists() {

	ESP_LOGW(DATA_PROCESSING, "PRINTING EMPLOYEES");
	for (Employee emp: this->employees.getEmployeeList()) {
		emp.printEmployee();
	}
	ESP_LOGW(DATA_PROCESSING, "PRINTING DEVICES");
	for (Device dev: this->devices.getDeviceList()) {
		dev.printDevice();
	}
}

/**
 *
 * @return reference to employee model
 */
EmployeeModel& DataProcessing::getEmployeeModel() {
	return this->employees;
}

/**
 *
 * @return reference to device model
 */
DeviceModel& DataProcessing::getDeviceModel() {
	return this->devices;
}

