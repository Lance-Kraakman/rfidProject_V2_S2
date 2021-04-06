/*
 * MainApplication.cpp
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */

#include "MainApplication.h"

MainApplication::MainApplication() {
	// TODO Auto-generated constructor stub

}

void MainApplication::runConfig() {
	this->WifiHandler.connectNetwork();
	this->messagingService.initMessagingServices();
	this->messagingService.addMessageSubscription("rfid-scanner-system/data", DEFAULT_QOS);
	this->messagingService.addMessageSubscription("rfid-scann-system/commands", DEFAULT_QOS);
	this->rfidScanner.startRfidScanner();
}

void MainApplication::tagProcessing(RfidTag& recvdTag, RfidTag& previousTag) {

	recvdTag = this->rfidScanner.popRfidList(); // Pop the next tag from the list


	if (!(recvdTag.isEmpty())) { // If we havent recieved an empty tag

		//If has not been scanned within a time frame of the last tag
		if (!(this->rfidScanner.isDoubleTagged(recvdTag, previousTag, MICROSECOND_TIMEOUT_PERIOD))) {
			// do Tag Processing
			ESP_LOGI(MAIN_APP, "DO TAG PROCESSING");

			//Dummy objects used for searching by UUID
			Employee searchEmployee = Employee("", recvdTag);
			Device searchDevice = Device(recvdTag);


			if (this->findDevice(searchDevice)) {

			} else if (this->findEmployee(searchEmployee)) {

			} else {
				// Request listning apps to create an employee with the UI.
				char buffer[50];
				sprintf(buffer, "Buffer: %lld \n", searchEmployee.getTag().getUUID());
				this->messagingService.sendMessage("config-employee", buffer, DEFAULT_QOS);
			}



			/*
			 * See if we have a employee with this tag or a device. If we dont we send a message to the computer with the uuid of the device
			 * and a request for the rfid tag or device to be added to the system
			 */


			recvdTag.printTag();

		}

	}
	previousTag = recvdTag; // Updated the previos tag
}

void MainApplication::messageProcessing(message& recvdMessage) {

	// Recieves the next message from the queue - returns empty message if none in queue
	recvdMessage = this->messagingService.popMessage();

	//check if we recieved a message
	if (recvdMessage.recvd) {
		this->messagingService.printMessage(recvdMessage);

		if (recvdMessage.topic.compare("add-employees")) { // recieve employee data and add it to list of employees
			printf("Data Message Received\n");

		} else if (recvdMessage.topic.compare("add-devices")) { // receive a list of devices to add

		} else if (recvdMessage.topic.compare("command")) { // recieve command to do something

			printf("command Message Received\n");

			// Now check all of the commands and
			if (recvdMessage.data.compare("send-notification")) {
				// Send a notification to desktop-application wiith data one
			} else if (recvdMessage.data.compare("send-employee-list")) {
				// publish the list of empoyees to a topi
			}

		}

	}
}

// Basic State Machine
void MainApplication::runLoop() {

//		RfidTag empTag = RfidTag(123456789);
//
//		Employee lanceKraakman = Employee();
//		lanceKraakman.setName("Lance Kraakman");
//		lanceKraakman.setTag(empTag);
//
//		Device device1 = Device(RfidTag(1234));
//		Device device2 = Device(RfidTag(223425));
//		Device device3 = Device(RfidTag(3234));
//		Device device4 = Device(RfidTag(42341));
//
//		Device device5 = Device(RfidTag(23425));
//		Device device6 = Device(RfidTag(646576));
//		Device device7 = Device(RfidTag(234657));
//		Device device8 = Device(RfidTag(7438));
//
//		Device device9 = Device(RfidTag(900));
//		device9.setName("Akinifenwa");
//
//
//		ESP_LOGI("---- ", "3");
//
//	//
//		lanceKraakman.addDevice(device1);
//		lanceKraakman.addDevice(device2);
//		lanceKraakman.addDevice(device3);
//		lanceKraakman.addDevice(device4);
//		lanceKraakman.addDevice(device5);
//		lanceKraakman.addDevice(device6);
//		lanceKraakman.addDevice(device7);
//		lanceKraakman.addDevice(device8);
//		lanceKraakman.addDevice(device9);
//
//		this->employeeList.push_back(lanceKraakman);

	// Configure proccesing function variables so they don't have to be initialized inside the functions every loop

	// Tag Processing Variables
	RfidTag recvdTag = RfidTag();
	RfidTag previousTag = RfidTag();

	// Message Processing Variables
	message recvdMessage = {"","",0};

	while(1) {
		// Process Tags
		this->tagProcessing(recvdTag, previousTag);

		// Process Messages and if large data is being transfered displays please wait screen.
		this->messageProcessing(recvdMessage);



		vTaskDelay(500/portTICK_RATE_MS);
	}
}


bool MainApplication::findDevice(Device& deviceToCheck) {

	std::vector<Device> DeviceList = this->deviceList;

	auto it = std::find(DeviceList.begin(), DeviceList.end(), deviceToCheck);

	if (it != DeviceList.end()) {
		printf("Find Complete -- Found\n");
		//Device myDev = *it;
		deviceToCheck = *it;
		return true;
	}
	printf("Find Complete -- Not Found\n");
	return false;

}

bool MainApplication::findEmployee(Employee& EmployeeToCheck) {

	std::vector<Employee> employeeList = this->employeeList;

	auto it = std::find(employeeList.begin(), employeeList.end(), EmployeeToCheck);

		if (it != employeeList.end()) {
			printf("Find Complete -- Found\n");
			//Employee emp = *it;
			EmployeeToCheck = *it;
			return true;
		}
		printf("Find Complete -- Not Found\n");
		return false;
	return false;
}



