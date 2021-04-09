/*
 * tagProcessing.cpp
 *
 *  Created on: 7/04/2021
 *      Author: lance
 */

#include "/home/lance/eclipse-workspace/rfidProject_V2_S2/components/Classes/Application/TagProcessing/TagProcessing.h"

TagProcessing::TagProcessing() {
	// TODO Auto-generated constructor stub
}

/*
 * Pass dataProcessor by reference and lists by address
 */
TagProcessing::TagProcessing(DataProcessing *dataProcessor) {
	this->dataProcessor = dataProcessor;
	this->previousTag = RfidTag();
	this->rfidScanner = RfidScanner();
}

void TagProcessing::init() {
	this->rfidScanner.startRfidScanner();
}

void TagProcessing::doProcessing() {
	RfidTag recvdTag = this->rfidScanner.popRfidList();

	if (!(recvdTag.isEmpty())) { // If we havent recieved an empty tag

			//If has not been scanned within a time frame of the last tag
			if (!(this->rfidScanner.isDoubleTagged(recvdTag, this->previousTag, MICROSECOND_TIMEOUT_PERIOD))) {
				//Dummy objects used for searching by UUID
				Employee searchEmployee = Employee("", recvdTag);
				Device searchDevice = Device(recvdTag);

				// Finds and updates Device if tag is a device tag
				if (this->dataProcessor->getDeviceModel().findDevice(searchDevice)) {
					ESP_LOGV(TAG_PROCESSING, "---found device----");
					this->deviceIsFound(searchDevice,recvdTag);
				}
				else if (this->dataProcessor->getEmployeeModel().findEmployee(searchEmployee)) // finds the employee if the tag is an employee tag
				{
					ESP_LOGV(TAG_PROCESSING, "---found employee----");
					this->foundEmployee(searchEmployee, recvdTag); // Finds and updates employee if found
				}
				else
				{
					this->sendConfigRequest(searchEmployee); // Request dataProcessor configuration
				}
				this->dataProcessor->printLists();

			}
		}
		previousTag = recvdTag; // Updated the previos tag
}

/*
 * PRIVATE METHODS
 */

void TagProcessing::deviceIsFound(Device& searchDevice, RfidTag& recvdTag) {
	searchDevice.setTag(recvdTag); // Keep tag of scanned tag to update the time

	// check if device has an employee, remove employee if so.
	searchDevice.setHasEmployee(false);
	searchDevice.setEmployee(Employee());
	this->dataProcessor->getDeviceModel().updateDeviceInList(searchDevice);

	// add employee to device if there is an active employee.
	if (searchDevice.getTag().checkTagTimeoutSeconds(this->previousEmployee.getTag(), MAX_SECONDS)) {
		ESP_LOGI(TAG_PROCESSING, "SCAN IN TIME");
		if (this->previousEmployee.isActive()) {
			this->empIsActive(searchDevice);
		}
	}
	else // Scan timeout so reset the active status
	{
		this->scanTimeout();
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
	this->dataProcessor->getMessagingService().sendMessage("config-employee", buffer, DEFAULT_QOS);
}





