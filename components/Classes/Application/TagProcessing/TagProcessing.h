/*
 * tagProcessing.h
 *
 *  Created on: 7/04/2021
 *      Author: lance
 */
#include "../../RfidScanner/RfidScanner.h"
#include "../../Employee/Employee.h"
#include "../../Device/Device.h"
#include <algorithm>
#include "../DataProcessing/DataProcessing.h"
#include "../../Display/DisplayDriver.h"

#define TAG_PROCESSING "TAG PROCESSING"
#define MAX_SECONDS 30

#ifndef COMPONENTS_CLASSES_APPLICATION_TAGPROCESSING_TAGPROCESSING_H_
#define COMPONENTS_CLASSES_APPLICATION_TAGPROCESSING_TAGPROCESSING_H_

/** \brief Application code, Provides interaction between rfid scanner and data processoing class
 *	Provides a state machine which decides what to do with the scanned tags
 */
class TagProcessing {
private:


	RfidScanner rfidScanner;

	RfidTag previousTag; // Copy of previously scanned tag
	Employee previousEmployee; // Copy of previously scanned employee
	Device previousDevice;

	DisplayDriver *display;

	// private methods for state machine
	void deviceIsFound(Device& searchDevice, RfidTag& recvdTag);
	void empIsActive(Device& searchDevice);
	void foundEmployee(Employee& searchEmployee, RfidTag& recvdTag);
	void scanTimeout();
	void sendConfigRequest(Employee& emp);
public:
	DataProcessing *dataProcessor;
	TagProcessing();
	void doProcessing();
	TagProcessing(DataProcessing *dataProcessor, DisplayDriver *myDriver);
	void init();

	static httpResponse addDevToSys;
	static httpResponse addEmpToSys;
	static httpResponse unregisteredTag;
};

#endif /* COMPONENTS_CLASSES_APPLICATION_TAGPROCESSING_TAGPROCESSING_H_ */
