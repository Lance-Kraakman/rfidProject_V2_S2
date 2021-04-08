/*
 * Employee.h
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */
#define EMPLOYEE_TAG "EMPLOYEE TAG"

#ifndef COMPONENTS_CLASSES_EMPLOYEE_EMPLOYEE_H_
#define COMPONENTS_CLASSES_EMPLOYEE_EMPLOYEE_H_

#include "../RfidTag/RfidTag.h"
#include "esp_log.h"
#include "esp_system.h"
#include <vector>



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
	//void addDevice(Device deviceToAdd); // Add device to device list
	//bool removeDevice(Device& deviceToRemove);  // Remove Device from UUID
	//Device getDevice(std::string UUID); // Get device with the corrosponding UUID
	//Device popDevice(std::string UUID); // Pop Device with the corrosponding UUID
	//std::vector<Device>& getDeviceList(); // Get the list by reference
	//bool findDevice(Device& deviceToFind); // Finds the device to check and updates any values
	//bool checkDataReturned(std::vector<Device>::iterator it);
	void setActive(bool active);
	void printEmployee();
	bool isActive();
	std::string timeString();
};

#endif /* COMPONENTS_CLASSES_EMPLOYEE_EMPLOYEE_H_ */
