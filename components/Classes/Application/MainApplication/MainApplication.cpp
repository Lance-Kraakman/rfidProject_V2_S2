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

	this->DataProcessor = DataProcessing(&this->employeeList, &this->deviceList);
	this->TagProcessor = TagProcessing(&this->DataProcessor);

	this->DataProcessor.init();
	this->TagProcessor.init();

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




