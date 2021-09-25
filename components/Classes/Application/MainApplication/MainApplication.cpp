/*
 * MainApplication.cpp
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */

#include "MainApplication.h"

// Creaete Pages
//httpResponse emps = httpResponse(std::string("<h1> Default Data <h1>"), HTTPD_RESP_USE_STRLEN, std::string("/emps"), std::string("application/json"), HTTP_GET);
//httpResponse devs = httpResponse(std::string("<h1> Default Data <h1>"), HTTPD_RESP_USE_STRLEN, std::string("/devs"), std::string("application/json"), HTTP_GET);


#define MAX_SECONDS 30

MainApplication::MainApplication() {

}

void MainApplication::runConfig() {

	/**
	 * SPI max speed/frequency depends on hardware, length etc.
	 */
	this->DisplayProcessor = DisplayDriver(5,10,20000000,21,26,19,9,18);
	this->DisplayProcessor.init();
	vTaskDelay(50/portTICK_RATE_MS);
	this->DataProcessor = DataProcessing();
	this->TagProcessor = TagProcessing(&this->DataProcessor, &DisplayProcessor);
//
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

	this->DataProcessor.getDeviceModel().updateDeviceResponse(); // Updates the JSON response string

	Employee emp1 = Employee("Lance", RfidTag(195232940));
	Employee emp2 = Employee("Nicole", RfidTag(194970784));
	Employee emp3 = Employee("Charlie", RfidTag(194577574));

	this->DataProcessor.getEmployeeModel().addEmployee(emp1);
	this->DataProcessor.getEmployeeModel().addEmployee(emp2);
	this->DataProcessor.getEmployeeModel().addEmployee(emp3);

	this->DataProcessor.getEmployeeModel().updateEmployeeResponse();

}


// Basic State Machine
void MainApplication::runLoop() {

	while(1) {
		// Theese have to be sequential and not in parrellel because they access shared data
		this->TagProcessor.doProcessing();
		this->DataProcessor.doMessageProcessing(); // Does Fuck all <3
		this->DisplayProcessor.runDisplay();
		vTaskDelay(20/portTICK_RATE_MS);
	}
}









