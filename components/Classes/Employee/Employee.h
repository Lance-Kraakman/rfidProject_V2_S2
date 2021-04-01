/*
 * Employee.h
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#ifndef COMPONENTS_CLASSES_EMPLOYEE_EMPLOYEE_H_
#define COMPONENTS_CLASSES_EMPLOYEE_EMPLOYEE_H_

#include "../RfidTag/RfidTag.h"
#include "../Device/Device.h"
#include <vector>


class Employee {
private:
	std::string name;
	RfidTag tag;
	std::vector<Device> deviceList;
	int activeStatus; // We can only add active status if employee is tagged
public:
	Employee();
	Employee::Employee(std::string name, RfidTag tag);

	void Employee::setName(std::string name);
	std::string Employee::getName();
	void setTag(RfidTag tag);
	RfidTag getTag();

	void addDevice(Device deviceToAdd); // Add device to device list
	void removeDevice(std::string UUID); // Remove Device from UUID
	Device getDevice(std::string UUID); // Get device with the corrosponding UUID
	Device popDevice(std::string UUID); // Pop Device with the corrosponding UUID
	std::vector<Device> getDeviceList(); // Get the list

	void setActive(int active);
	int isActive();
};

#endif /* COMPONENTS_CLASSES_EMPLOYEE_EMPLOYEE_H_ */
