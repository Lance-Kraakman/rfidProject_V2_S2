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

	this->DataProcessor = DataProcessing();
	this->TagProcessor = TagProcessing(&this->DataProcessor);

	this->DataProcessor.init();
	this->TagProcessor.init();

	// TODO Auto-generated constructor stub
	// Create some tags and devices for testing
	Device dev1 = Device(RfidTag(196609223));
	Device dev2 = Device(RfidTag(194315418));
	Device dev3 = Device(RfidTag(196347067));

	this->DataProcessor.getDeviceModel().addDevice(dev1);
	this->DataProcessor.getDeviceModel().addDevice(dev2);
	this->DataProcessor.getDeviceModel().addDevice(dev3);

	Employee emp1 = Employee("Lance", RfidTag(195232940));
	Employee emp2 = Employee("Nicole", RfidTag(194970784));
	Employee emp3 = Employee("Charlie", RfidTag(194577574));

	this->DataProcessor.getEmployeeModel().addEmployee(emp1);
	this->DataProcessor.getEmployeeModel().addEmployee(emp2);
	this->DataProcessor.getEmployeeModel().addEmployee(emp3);

}

// Basic State Machine
void MainApplication::runLoop() {

	while(1) {
		// Theese have to be sequential and not in parrellel because they access shared data
		this->TagProcessor.doProcessing();
		this->DataProcessor.doMessageProcessing();
		vTaskDelay(200/portTICK_RATE_MS);
	}
}




