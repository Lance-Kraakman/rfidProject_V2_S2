/*
 * MainApplication.cpp
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */

#include "MainApplication.h"

#define MAX_SECONDS 30

MainApplication::MainApplication() {
	// TODO Auto-generated constructor stub

}

void MainApplication::runConfig() {
	this->WifiHandler.connectNetwork();
	this->messagingService.initMessagingServices();
	this->messagingService.addMessageSubscription("rfid-scanner-system/data", DEFAULT_QOS);
	this->messagingService.addMessageSubscription("rfid-scann-system/commands", DEFAULT_QOS);
	this->rfidScanner.startRfidScanner();

	this->globalTime = SntpTime();

	globalTime.config();
	globalTime.SyncTime();
	globalTime.printTime();
	globalTime.updateToCurrentTime(); // sets the time to the current time
	globalTime.printTime();

	// Create some tags and devices for testing
	Device dev1 = Device(RfidTag(196609223));
	Device dev2 = Device(RfidTag(194315418));
	Device dev3 = Device(RfidTag(196347067));

	Employee emp1 = Employee("Lance", RfidTag(195232940));
	Employee emp2 = Employee("Nicole", RfidTag(194970784));
	Employee emp3 = Employee("Charlie", RfidTag(194577574));

	this->employeeList.push_back(emp1);
	this->employeeList.push_back(emp2);
	this->employeeList.push_back(emp3);

	this->deviceList.push_back(dev1);
	this->deviceList.push_back(dev2);
	this->deviceList.push_back(dev3);


}

void MainApplication::tagProcessing(RfidTag& recvdTag, RfidTag& previousTag, Employee& lastScannedEmployee) {

	recvdTag = this->rfidScanner.popRfidList(); // Pop the next tag from the list

	if (!(recvdTag.isEmpty())) { // If we havent recieved an empty tag

		//If has not been scanned within a time frame of the last tag
		if (!(this->rfidScanner.isDoubleTagged(recvdTag, previousTag, MICROSECOND_TIMEOUT_PERIOD))) {
			ESP_LOGI(MAIN_APP, "DO TAG PROCESSING");

			//Dummy objects used for searching by UUID
			Employee searchEmployee = Employee("", recvdTag);
			Device searchDevice = Device(recvdTag);

			// Finds and updates Device if tag is a device tag
			if (this->findDevice(searchDevice)) {
				searchDevice.setTag(recvdTag); // Keep tag of scanned tag to update the time

				// check if device has an employee, remove employee if so.
				{
					searchDevice.setHasEmployee(false);
					searchDevice.setEmployee(Employee());
					this->updateDeviceInList(searchDevice);

				}

				// add employee to device if there is an active employee.
				if (this->checkTagTimeDuration(searchDevice.getTag(), lastScannedEmployee.getTag(), MAX_SECONDS)) {
					ESP_LOGI(MAIN_APP, "SCAN IN TIME");

					if (lastScannedEmployee.isActive()) {

						// set the employee as active
						lastScannedEmployee.setActive(false);
						this->updateEmployeeInList(lastScannedEmployee);

						// Add the employee to the device
						searchDevice.setHasEmployee(true);
						searchDevice.setEmployee(lastScannedEmployee);
						this->updateDeviceInList(searchDevice);

					}

				// Scan timeout so reset the active status
				} else {
					ESP_LOGI(MAIN_APP, "TOOK TO LONG TO SCAN");
					lastScannedEmployee.setActive(false); // if taken to long to scan, reset the employee
					this->updateEmployeeInList(lastScannedEmployee);

				}

			// finds the employee if the tag is an employee tag
			} else if (this->findEmployee(searchEmployee)) { // Finds and updates employee if found

				//keep the reference of the last scanned employee and update the global list
				lastScannedEmployee = searchEmployee;
				lastScannedEmployee.setActive(true); // activate the employee
				lastScannedEmployee.setTag(recvdTag); //update the tag
				ESP_LOGI(MAIN_APP, "1");
				lastScannedEmployee.printEmployee();
				this->updateEmployeeInList(lastScannedEmployee);


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

			ESP_LOGI(MAIN_APP, "PRINTING EMPLOYEES");
			for (Employee emp: this->employeeList) {
				emp.printEmployee();
			}
			ESP_LOGI(MAIN_APP, "PRINTING DEVICES");
			for (Device dev: this->deviceList) {
				dev.printDevice();
			}

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

		if (recvdMessage.topic.compare("device/add-employees")) { // recieve employee data and add it to list of employees
			printf("Data Message Received\n");

		} else if (recvdMessage.topic.compare("device/add-devices")) { // receive a list of devices to add

		} else if (recvdMessage.topic.compare("device/command")) { // recieve command to do something

			printf("command Message Received\n");

			// Now check all of the commands and
			if (recvdMessage.data.compare("send-notification")) {
				// Send a notification to desktop-application wiith data one
			} else if (recvdMessage.data.compare("send-employee-list")) {
				// publish the list of empoyees to a topi
			} else if (recvdMessage.data.compare("send-device-list")) {

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


	// Configure proccesing function variables so they don't have to be initialized inside the functions every loop

	// Tag Processing Variables
	RfidTag recvdTag = RfidTag();
	RfidTag previousTag = RfidTag();
	Employee scannedEmployee = Employee();

	// Message Processing Variables
	message recvdMessage = {"","",0};

	while(1) {
		// Process Tags
		this->tagProcessing(recvdTag, previousTag, scannedEmployee);

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
		deviceToCheck = *it; // Update the devices other params
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
			EmployeeToCheck = *it; // Update the devices other params
			return true;
		}
		printf("Find Complete -- Not Found\n");
		return false;
	return false;
}

/*
 * Function returns false if tag one scanned maxSeconds after tag two
 */
bool MainApplication::checkTagTimeDuration(RfidTag tagOne, RfidTag tagTwo, int maxSeconds) {
	SntpTime first = tagOne.getScannedTime();
	SntpTime second = tagTwo.getScannedTime();

	int firstTagSecondsToday = (first.hour * 60 * 60) + (first.minute * 60) + (first.second);
	int secondSecondsToday = (second.hour * 60 * 60) + (second.minute * 60) + (second.second);

	if ((abs(firstTagSecondsToday - secondSecondsToday) < maxSeconds)) {
		return true;
	}
	return false;
}

bool MainApplication::updateDeviceInList(Device& deviceToUpdate) {
	std::replace(this->deviceList.begin(), this->deviceList.end(), deviceToUpdate, deviceToUpdate);
	return true;
}

bool MainApplication::updateEmployeeInList(Employee& EmployeeToUpdate) {
	std::replace(this->employeeList.begin(), this->employeeList.end(), EmployeeToUpdate, EmployeeToUpdate);
	return true;
}


