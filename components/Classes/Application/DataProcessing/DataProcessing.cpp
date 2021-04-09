/*
 * CommunicationsProcessing.cpp
 *
 *  Created on: 7/04/2021
 *      Author: lance
 */


#include "../DataProcessing/DataProcessing.h"

DataProcessing::DataProcessing() {}

/*
 * config - in the future this will also handle exceptions and stuff
 */
void DataProcessing::init() {

		this->WifiHandler.connectNetwork();
		this->messagingService.initMessagingServices();
		this->messagingService.addMessageSubscription("device/add-device", DEFAULT_QOS);
		this->messagingService.addMessageSubscription("device/add-employee", DEFAULT_QOS);
		this->messagingService.addMessageSubscription("device/command", DEFAULT_QOS);

		this->globalTime = SntpTime();

		globalTime.config();
		globalTime.SyncTime();
		globalTime.printTime();
		globalTime.updateToCurrentTime(); // sets the time to the current time
		globalTime.printTime();

		this->devices = DeviceModel();
		this->employees = EmployeeModel();
}

/**
 *
 * @return void
 */
void DataProcessing::doMessageProcessing() {

	// Recieves the next message from the queue - returns empty message if none in queue
	message mes = this->messagingService.popMessage();
	this->recvdMessage = mes;
	//check if we recieved a message

	if (this->recvdMessage.recvd) {
		this->messagingService.printMessage(recvdMessage);

		if (!(this->recvdMessage.topic.compare("device/add-employee"))) { // recieve employee data and add it to list of employees
			printf("Data Message Received\n");
			this->addEmployee();
		} else if (!(this->recvdMessage.topic.compare("device/add-device"))) { // receive a list of devices to add
			this->addDevice();
		} else if (!(this->recvdMessage.topic.compare("device/command"))) { // recieve command to do something
			this->commandReceived();
		}
	}
}

void DataProcessing::sendDevices() {

	cJSON* finalJSON = this->devices.jsonDeviceList();
	char *deviceDataBuffer = cJSON_Print(finalJSON);

	printf("SENDING DATA %s", deviceDataBuffer);
	this->messagingService.sendMessage("app/app-data", deviceDataBuffer, DEFAULT_QOS);

	//Free Memory (Created with Malloc so must use free)
	free(deviceDataBuffer);
	deviceDataBuffer = NULL;

	free(finalJSON);
	finalJSON = NULL;
}

void DataProcessing::sendEmployees() {

	cJSON* finalJSON = this->employees.jsonEmployeeList();
	char *employeeDataBuffer = cJSON_Print(finalJSON);
	printf("SENDING DATA\n%s", employeeDataBuffer);
	this->messagingService.sendMessage("app/app-data", employeeDataBuffer, DEFAULT_QOS);

	// Free Memory (Created with Malloc so must use free)
	free(employeeDataBuffer);
	employeeDataBuffer = NULL;

	free(finalJSON);
	finalJSON = NULL;

}

//Should only do this if the device isnt in the list
void DataProcessing::addDevice() {
	cJSON *recvdJSON = cJSON_Parse(this->recvdMessage.data.c_str());
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

// It should only do this if the employee isnt in the list
void DataProcessing::addEmployee() {
	cJSON *recvdJSON = cJSON_Parse(this->recvdMessage.data.c_str());
	try {
		Employee emp = this->employees.employeeFromJson(recvdJSON);
		this->employees.addEmployee(emp);

	} catch(const char* msg) {
		printf(msg);
		printf("Employee Not added");
	}

	free(recvdJSON);
	recvdJSON = NULL;
}


/**
 * @Return Nothing
 */
void DataProcessing::commandReceived() {
	printf("command Message Received\n");

	// Now check all of the commands and
	if (!(recvdMessage.data.compare("send-notification"))) {
		// Send a notification to desktop-application wiith data one
		this->messagingService.sendMessage("app/app-status", "1", DEFAULT_QOS);

	} else if (!(recvdMessage.data.compare("send-employee-list"))) {
		// publish the list of empoyees to a topi
		this->sendEmployees();

	} else if (!(recvdMessage.data.compare("send-device-list"))) {
		this->sendDevices();
	}
}

MessagingService DataProcessing::getMessagingService() {
	return this->messagingService;
}

/*
 * PUBLIC FUNCTIONS
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

EmployeeModel& DataProcessing::getEmployeeModel() {
	return this->employees;
}

DeviceModel& DataProcessing::getDeviceModel() {
	return this->devices;
}

