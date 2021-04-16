/*
 * tagProcessing.cpp
 *
 *  Created on: 7/04/2021
 *      Author: lance
 */

#include "/home/lance/eclipse-workspace/rfidProject_V2_S2/components/Classes/Application/TagProcessing/TagProcessing.h"

TagProcessing::TagProcessing() {}
/*
 * Pass dataProcessor by reference and lists by address
 */
TagProcessing::TagProcessing(DataProcessing *dataProcessor, DisplayDriver *myDriver) {
	this->dataProcessor = dataProcessor;
	this->previousTag = RfidTag();
	this->rfidScanner = RfidScanner();
	this->display = myDriver;
	this->previousDevice = Device();
}

void TagProcessing::init() {
	this->rfidScanner.startRfidScanner();
}

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
			}
		}
		previousTag = recvdTag; // Updated the previos tag only if its legit


}

/*
 * PRIVATE METHODS
 */

void TagProcessing::deviceIsFound(Device& searchDevice, RfidTag& recvdTag) {
	searchDevice.setTag(recvdTag); // Keep tag of scanned tag to update the time

	// check if device has an employee, remove employee if so.
	if (searchDevice.hasEmployee() && !(searchDevice.getTag().checkTagTimeoutSeconds(this->previousDevice.getTag(), 3))) {
		this->display->updateHomeText("Device Returned\nThank You!", STATE_NOT_HOME, 3);
		searchDevice.setHasEmployee(false);
		searchDevice.setEmployee(Employee());
		this->dataProcessor->getDeviceModel().updateDeviceInList(searchDevice);
	} else if (!(searchDevice.hasEmployee()) && (!(searchDevice.getTag().checkTagTimeoutSeconds(this->previousDevice.getTag(), 3)))) {
		this->display->updateHomeText("No Tagged\nEmplpoyee", STATE_DEVICE_TAGGED, 3);
	}

	// add employee to device if there is an active employee.
	if (searchDevice.getTag().checkTagTimeoutSeconds(this->previousEmployee.getTag(), MAX_SECONDS)) {
		ESP_LOGE(TAG_PROCESSING, "SCAN IN TIME");
		if (this->previousEmployee.isActive()) {
			ESP_LOGE(TAG_PROCESSING, "OK");

			this->empIsActive(searchDevice);
			this->display->updateHomeText("Device Tagged\nTo Person", STATE_DEVICE_TAGGED, 3); // This should have a timer function to go back to original display
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
	char buffer[50];
	sprintf(buffer, "Buffer: %lld \n", emp.getTag().getUUID());
	this->dataProcessor->getMessagingService().sendMessage("                                                                                                                                                                                                                                     -employee", buffer, DEFAULT_QOS);
	this->display->updateHomeText("Please Register\nTag", STATE_EMPLOYEE_TAGGED, 5);
}





