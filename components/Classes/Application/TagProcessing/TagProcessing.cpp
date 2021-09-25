/*
 * tagProcessing.cpp
 *
 *  Created on: 7/04/2021
 *      Author: lance
 */

#include "/home/lance/eclipse-workspace/rfidProject_V2_S2/components/Classes/Application/TagProcessing/TagProcessing.h"

TagProcessing::TagProcessing() {}

httpResponse TagProcessing::addDevToSys = httpResponse("Default", HTTPD_RESP_USE_STRLEN, std::string("/addDev"), std::string("application/json"), HTTP_POST);
httpResponse TagProcessing::addEmpToSys = httpResponse("Default", HTTPD_RESP_USE_STRLEN, std::string("/addEmp"), std::string("application/json"), HTTP_POST);
httpResponse TagProcessing::unregisteredTag = httpResponse("", HTTPD_RESP_USE_STRLEN, std::string("/unregistered-tag"), std::string("application/json"), HTTP_GET);


/*
 * Pass dataProcessor by address and lists by address
 */
TagProcessing::TagProcessing(DataProcessing *dataProcessor, DisplayDriver *myDriver) {
	this->dataProcessor = dataProcessor;
	this->previousTag = RfidTag();
	this->rfidScanner = RfidScanner();
	this->display = myDriver;
	this->previousDevice = Device();

	this->dataProcessor->myServer.addResponse(&TagProcessing::addDevToSys);
	this->dataProcessor->myServer.addResponse(&TagProcessing::addEmpToSys);

	TagProcessing::unregisteredTag.deleteDataOnResponse = true; // Once the data is read from the server it will be deleted automatically
	this->dataProcessor->myServer.addResponse(&TagProcessing::unregisteredTag);

}

void TagProcessing::init() {
	this->rfidScanner.startRfidScanner();
}

/** \brief Tag Processing Logic
 *
 */
void TagProcessing::doProcessing() {
	RfidTag recvdTag = this->rfidScanner.popRfidList();

	if (!(recvdTag.isEmpty())) { // If we havent recieved an empty tag

			printf("1 : ");
			previousTag.printTag();

			//If has not been scanned within a time frame of the last tag
			if (!(this->rfidScanner.isDoubleTagged(recvdTag, this->previousTag, MICROSECOND_TIMEOUT_PERIOD))) {
				//Dummy objects used for searching by UUID
				Employee searchEmployee = Employee("", recvdTag);
				Device searchDevice = Device(recvdTag);

				// Finds and updates Device if tag is a device tag
				if (this->dataProcessor->getDeviceModel().findDevice(searchDevice)) {
					ESP_LOGV(TAG_PROCESSING, "---found device----");
					this->deviceIsFound(searchDevice,recvdTag);
					previousTag.printTag();
					this->previousDevice = searchDevice;
					printf("2");
				}
				else if (this->dataProcessor->getEmployeeModel().findEmployee(searchEmployee)) // finds the employee if the tag is an employee tag
				{
					ESP_LOGV(TAG_PROCESSING, "---found employee----");
					this->foundEmployee(searchEmployee, recvdTag); // Finds and updates employee if found
					previousTag.printTag();
					printf("3");
				}
				else
				{
					this->sendConfigRequest(searchEmployee); // Request dataProcessor configuration
					previousTag.printTag();
					printf("4");
				}
				this->dataProcessor->printLists();
				this->dataProcessor->getDeviceModel().updateDeviceResponse();
				this->dataProcessor->getEmployeeModel().updateEmployeeResponse();
			}
		}

		// Check if we have recieved data from a post request!
		if (!(this->addDevToSys.dataRead)) {
			ESP_LOGW("","GET FUCKEd");
			//We have read data so we do stuff with it <3
			std::string recvdDevices = this->addDevToSys.getData();
			this->dataProcessor->addDevice(recvdDevices);
			this->addDevToSys.dataRead = true;
		} else if (!(this->addEmpToSys.dataRead)) {
			//we have read data so we do stuff with it <3
			ESP_LOGW("","GET FUCKEd");
			std::string recvdEmployees = this->addEmpToSys.getData();
			this->dataProcessor->addEmployee(recvdEmployees);
			this->addEmpToSys.dataRead  = true;
		}
		previousTag = recvdTag; // Updated the previos tag only if its legit

}

// It wont double up because when you add a person it gets the UUID from the system anyway -> which will be gotten from a https request.
// Which will be set when a unregistered tag is sent

/*
 * PRIVATE METHODS
 */
void TagProcessing::deviceIsFound(Device& searchDevice, RfidTag& recvdTag) {
	searchDevice.setTag(recvdTag); // Keep tag of scanned tag to update the time

	// check if device has an employee, remove employee if so.
	if (searchDevice.hasEmployee() && !(searchDevice.getTag().checkTagTimeoutSeconds(this->previousDevice.getTag(), 3))) {
		this->display->updateHomeText((char *) "Device Returned\nThank You!", STATE_NOT_HOME, 3);
		searchDevice.setHasEmployee(false);
		searchDevice.setEmployee(Employee());
		this->dataProcessor->getDeviceModel().updateDeviceInList(searchDevice);
	} else if (!(searchDevice.hasEmployee()) && (!(searchDevice.getTag().checkTagTimeoutSeconds(this->previousDevice.getTag(), 3)))) {
		this->display->updateHomeText((char *) "No Tagged\nEmplpoyee", STATE_DEVICE_TAGGED, 3);
	}

	// add employee to device if there is an active employee.
	if (searchDevice.getTag().checkTagTimeoutSeconds(this->previousEmployee.getTag(), MAX_SECONDS)) {
		ESP_LOGE(TAG_PROCESSING, "SCAN IN TIME");
		if (this->previousEmployee.isActive()) {
			ESP_LOGE(TAG_PROCESSING, "OK");

			this->empIsActive(searchDevice);
			this->display->updateHomeText((char *) "Device Tagged\nTo Person", STATE_DEVICE_TAGGED, 3); // This should have a timer function to go back to original display
		}
	}
	else // Scan timeout so reset the active status
	{
		this->scanTimeout();
		this->previousEmployee.setActive(false);
		//this->display->updateHomeText("No Tagged\nEmplpoyee", STATE_DEVICE_TAGGED, 3);
	}




}

void TagProcessing::empIsActive(Device& searchDevice) {
	// set the employee as active
	this->previousEmployee.setActive(false);
	this->dataProcessor->getEmployeeModel().updateEmployeeInList(this->previousEmployee);

	// Add the employee to the device
	searchDevice.setHasEmployee(true);
	searchDevice.setEmployee(this->previousEmployee);
	this->dataProcessor->getDeviceModel().updateDeviceInList(searchDevice);
}

void TagProcessing::foundEmployee(Employee& searchEmployee, RfidTag& recvdTag) {
	this->previousEmployee = searchEmployee;
	this->previousEmployee.setActive(true); // activate the employee
	this->previousEmployee.setTag(recvdTag); //update the tag
	this->previousEmployee.printEmployee();
	this->dataProcessor->getEmployeeModel().updateEmployeeInList(this->previousEmployee);
	this->display->updateHomeText("Employee\nTagged", STATE_EMPLOYEE_TAGGED, 5);
}

void TagProcessing::scanTimeout() {
	ESP_LOGI(TAG_PROCESSING, "TOOK TO LONG TO SCAN");
	this->previousEmployee.setActive(false); // if taken to long to scan, reset the employee
	this->dataProcessor->getEmployeeModel().updateEmployeeInList(this->previousEmployee);
}

void TagProcessing::sendConfigRequest(Employee& emp) {
	/*
	 * See if we have a employee with this tag or a device. If we dont we send a message to the computer with the uuid of the device
	 * and a request for the rfid tag or device to be added to the system.
	 */
	char *uuidString = (char *) malloc(20 * sizeof(char)); //int_64_t is 10 chars. plus -, plus \0. then for assurance couple of extra tings for safety
	sprintf(uuidString, "%lld\n", emp.getTag().getUUID());

	// Update the data!
	this->unregisteredTag.updateData(uuidString);

	// Set the display
	this->display->updateHomeText((char *) "Please Register\nTag", STATE_EMPLOYEE_TAGGED, 5);

	// De allocate memory
	free(uuidString);
	uuidString = NULL;

}





