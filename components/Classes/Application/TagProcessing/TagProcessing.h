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

#define TAG_PROCESSING "TAG PROCESSING"
#define MAX_SECONDS 30

#ifndef COMPONENTS_CLASSES_APPLICATION_TAGPROCESSING_TAGPROCESSING_H_
#define COMPONENTS_CLASSES_APPLICATION_TAGPROCESSING_TAGPROCESSING_H_

class TagProcessing {
private:
	RfidScanner rfidScanner;
	DataProcessing *dataProcessor;

	RfidTag previousTag; // Copy of previously scanned tag
	Employee previousEmployee; // Copy of previously scanned employee

	// private methods for state machine
	void deviceIsFound(Device& searchDevice, RfidTag& recvdTag);
	void empIsActive(Device& searchDevice);
	void foundEmployee(Employee& searchEmployee, RfidTag& recvdTag);
	void scanTimeout();
	void sendConfigRequest(Employee& emp);

public:
	TagProcessing();
	void doProcessing();
	TagProcessing(DataProcessing *dataProcessor);
	void init();
};

#endif /* COMPONENTS_CLASSES_APPLICATION_TAGPROCESSING_TAGPROCESSING_H_ */
