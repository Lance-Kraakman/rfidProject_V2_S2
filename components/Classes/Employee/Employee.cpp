/*
 * Employee.cpp
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include "Employee.h"

Employee::Employee() {
	// TODO Auto-generated constructor stub

}

Employee::Employee(std::string name, RfidTag tag) {
	this->setName(name);
}

void Employee::setName(std::string name) {
	this->name = name;
}

std::string Employee::getName() {
	return this->name;
}

void Employee::setTag(RfidTag tag) {
	this->tag = tag;
}

RfidTag Employee::getTag() {
	return this->tag;
}

void Employee::addDevice(Device deviceToAdd) { // Add device to device list
	this->getDeviceList().insert(this->getDeviceList().end(), deviceToAdd);
}

void Employee::removeDevice(std::string UUID) { // Remove Device from UUID

}

Device Employee::getDevice(std::string UUID) { // Get device with the corrosponding UUID

}

Device Employee::popDevice(std::string UUID) { // Pop Device with the corrosponding UUID

}

/*
 * Function returns true if the employee already has the device and false if the employee dosent have the device
 */
bool hasDevice(Device deviceToCheck) {

}

std::vector<Device> Employee::getDeviceList() { // Get the list
	return this->deviceList;
}

void Employee::setActive(int active) {
	this->activeStatus = active;
}

int Employee::isActive() {
	return this->activeStatus;
}
