/*
 * CommunicationsProcessing.cpp
 *
 *  Created on: 7/04/2021
 *      Author: lance
 */


#include "../DataProcessing/DataProcessing.h"

DataProcessing::DataProcessing() {}

DataProcessing::DataProcessing(std::vector<Employee> *employeeListPtr, std::vector<Device> *deviceListPtr) {
	this->deviceListPtr = deviceListPtr;
	this->employeeListPtr = employeeListPtr;
	ESP_LOGI(DATA_PROCESSING, "Employee List Size: %d, Device List size %d", (*this->employeeListPtr).size(), (*this->deviceListPtr).size());
}

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
	cJSON *finalJSON = cJSON_CreateObject();
	cJSON *jsonArray = cJSON_AddArrayToObject(finalJSON, "DEVICE LIST");
	for (Device dev : *(this->deviceListPtr)) {
		cJSON *deviceJsonObject = cJSON_CreateObject();

		cJSON_AddStringToObject(deviceJsonObject, "name", dev.getName().c_str());
		cJSON_AddNumberToObject(deviceJsonObject, "uuid", dev.getTag().getUUID());
		cJSON_AddStringToObject(deviceJsonObject, "time", dev.getTag().getScannedTime().timeString().c_str());
		cJSON_AddNumberToObject(deviceJsonObject, "has-employee", dev.hasEmployee());
		cJSON_AddNumberToObject(deviceJsonObject, "employee-uuid", dev.getEmployee().getTag().getUUID() );

		cJSON_AddItemToArray(jsonArray, deviceJsonObject);
	}
	char *deviceDataBuffer = cJSON_Print(finalJSON);
	printf("SENDING DATA %s", deviceDataBuffer);
	this->messagingService.sendMessage("app/app-data", deviceDataBuffer, DEFAULT_QOS);
	free(deviceDataBuffer);
	deviceDataBuffer = NULL;
}

void DataProcessing::sendEmployees() {

	cJSON *finalJSON = cJSON_CreateObject();
	cJSON *jsonArray = cJSON_AddArrayToObject(finalJSON, "EMPLOYEE LIST");
	for (Employee emp : *(this->employeeListPtr)) {
		cJSON *employeeJsonObject = cJSON_CreateObject();

		cJSON_AddStringToObject(employeeJsonObject, "name", emp.getName().c_str());
		cJSON_AddNumberToObject(employeeJsonObject, "uuid", emp.getTag().getUUID());
		cJSON_AddStringToObject(employeeJsonObject, "time", emp.getTag().getScannedTime().timeString().c_str());
		cJSON_AddNumberToObject(employeeJsonObject, "active", emp.isActive());
		cJSON_AddItemToArray(jsonArray, employeeJsonObject);
	}
	char *employeeDataBuffer = cJSON_Print(finalJSON);
	printf("SENDING DATA\n%s", employeeDataBuffer);
	this->messagingService.sendMessage("app/app-data", employeeDataBuffer, DEFAULT_QOS);
	free(employeeDataBuffer);
	employeeDataBuffer = NULL;


}

//Should only do this if the device isnt in the list
void DataProcessing::addDevice() {
	cJSON *recvdJSON = cJSON_Parse(this->recvdMessage.data.c_str());
	char *string = cJSON_Print(recvdJSON);
	printf("RECEIVED JSON\n%s", string);

	try {
		cJSON* name  = cJSON_GetObjectItem(recvdJSON, "name");
		cJSON* uuid = cJSON_GetObjectItem(recvdJSON, "uuid");
		cJSON* cJSON_ARR[] = {name, uuid};

		for (int i=0; i<2; i++) {
			if (cJSON_ARR[i] == NULL) {
				throw "ADD EMPLOYEE OBJECT ERROR";
			}
		}

		Device dev = Device();
		dev.setName(name->valuestring);
		if (uuid->type == cJSON_String) {
			dev.setTag(RfidTag(std::stoi(uuid->string)));
		} else if (uuid->type == cJSON_Number) {
			dev.setTag(RfidTag(uuid->valueint));
		} else {
			throw "Couldn't resolve cJSON_type";
		}
		this->deviceListPtr->insert(this->deviceListPtr->begin(), dev);


	} catch(const char* msg) {
		printf(msg);
		printf("Employee Not added");
	}

	free(recvdJSON);
	recvdJSON = NULL;}

// It should only do this if the employee isnt in the list
void DataProcessing::addEmployee() {
	cJSON *recvdJSON = cJSON_Parse(this->recvdMessage.data.c_str());
	char *string = cJSON_Print(recvdJSON);
	printf("RECEIVED JSON\n%s", string);

	try {
		cJSON* name  = cJSON_GetObjectItem(recvdJSON, "name");
		cJSON* uuid = cJSON_GetObjectItem(recvdJSON, "uuid");

		cJSON* cJSON_ARR[] = {name, uuid};

		for (int i=0; i<2; i++) {
			if (cJSON_ARR[i] == NULL) {
				throw "ADD EMPLOYEE OBJECT ERROR";
			}
		}

		Employee emp = Employee();
		emp.setName(name->valuestring);

		if (uuid->type == cJSON_String) {
			emp.setTag(RfidTag(std::stoi(uuid->string)));
		} else if (uuid->type == cJSON_Number) {
			emp.setTag(RfidTag(uuid->valueint));
		} else {
			throw "Couldn't resolve cJSON_type";
		}

		// if employee is not in list

		this->employeeListPtr->insert(this->employeeListPtr->begin(), emp);


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




/*
 * Searches for Device, if an Device is found it is updated with the values found in the list
 */
bool DataProcessing::findDevice(Device& deviceToCheck) {
	std::vector<Device> DeviceList = *(this->deviceListPtr);

	auto it = std::find(DeviceList.begin(), DeviceList.end(), deviceToCheck);
	if (it != DeviceList.end()) {
		printf("Find Complete -- Found\n");
		deviceToCheck = *it; // Update the devices other params
		return true;
	}
	printf("Find Complete -- Not Found\n");
	return false;
}

/*
 * Searches for employee, if an employee is found it is updated with the values found in the list
 */
bool DataProcessing::findEmployee(Employee& employeeToCheck) {
	std::vector<Employee> employeeList = *(this->employeeListPtr);

	auto it = std::find(employeeList.begin(), employeeList.end(), employeeToCheck);
		if (it != employeeList.end()) {
			employeeToCheck = *it;
			return true;
		}
		return false;
	return false;
}

bool DataProcessing::updateDeviceInList(Device deviceToUpdate) {
	std::replace(this->deviceListPtr->begin(), this->deviceListPtr->end(), deviceToUpdate, deviceToUpdate);
	return true;
}

bool DataProcessing::updateEmployeeInList(Employee EmployeeToUpdate) {
	std::replace(this->employeeListPtr->begin(), this->employeeListPtr->end(), EmployeeToUpdate, EmployeeToUpdate);
	return true;
}

void DataProcessing::printLists() {

	ESP_LOGI(DATA_PROCESSING, "PRINTING EMPLOYEES");
	for (Employee emp: *this->employeeListPtr) {
		emp.printEmployee();
	}
	ESP_LOGI(DATA_PROCESSING, "PRINTING DEVICES");
	for (Device dev: *this->deviceListPtr) {
		dev.printDevice();
	}
}

