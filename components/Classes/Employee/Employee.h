/*
 * Employee.h
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */
#define EMPLOYEE_TAG "EMPLOYEE TAG"

#ifndef COMPONENTS_CLASSES_EMPLOYEE_EMPLOYEE_H_
#define COMPONENTS_CLASSES_EMPLOYEE_EMPLOYEE_H_

#include <vector>
#include <algorithm>
#include <string>

#include "../RfidTag/RfidTag.h"
#include "esp_log.h"
#include "esp_system.h"
#include "soc/rtc_wdt.h"
#include "esp_int_wdt.h"
#include "esp_task_wdt.h"

/** \brief Employee Class represents a device with an RfidTag 'attached'
 *
 */
class Employee {
private:
	std::string name;
	RfidTag tag;
	//std::vector<Device> deviceList;
	bool activeStatus; // We can only add active status if employee is tagged
public:
	Employee();
	Employee(std::string name, RfidTag tag);

	void setName(std::string name);
	std::string getName();
	void setTag(RfidTag tag);
	RfidTag getTag();
	friend bool operator== (const Employee &E1, const Employee &E2); // Search Function for has Device and getDeviceWithUUID <3
	void setActive(bool active);
	void printEmployee();
	bool isActive();
	std::string timeString();
};

#endif /* COMPONENTS_CLASSES_EMPLOYEE_EMPLOYEE_H_ */
