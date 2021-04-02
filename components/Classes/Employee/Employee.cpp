/*
 * Employee.cpp
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include "Employee.h"
#include <algorithm>
#include <string>

#define SQUARE(x) (x*x)

Employee::Employee() {
	// TODO Auto-generated constructor stub
	this->deviceList = std::vector<Device>();
}

Employee::Employee(std::string name, RfidTag tag) {
	this->setName(name);
	this->deviceList = std::vector<Device>();

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
	this->deviceList.push_back(deviceToAdd);
}

void Employee::removeDevice(std::string UUID) { // Remove Device from UUID

}

Device Employee::getDevice(std::string UUID) { // Get device with the corrosponding UUID
	return Device();
}

Device Employee::popDevice(std::string UUID) { // Pop Device with the corrosponding UUID
	return Device();
}

/*
 * Function returns true if the employee already has the device and false if the employee dosent have the device
 */
std::vector<Device>::iterator Employee::findDevice(Device deviceToCheck) {
	//std::vector<int>::iterator it = std::find(vecOfNums.begin(), vecOfNums.end(), 22);

	for (Device dev: this->getDeviceList()){
		printf("%s\n", dev.getTag().getUUID().c_str());
	}

	std::vector<Device>::iterator it = std::find(this->getDeviceList().begin(), this->getDeviceList().end(), deviceToCheck);

	if (it != this->getDeviceList().end()) {
		printf("Found\n");
		return it;
	} else {
		printf("Not Found\n");
		return it;
	}

}


bool Employee::checkDataReturned(std::vector<Device>::iterator it) {
	if (it != this->getDeviceList().end()) {
		return true;
	} else {
		return false;
	}
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



//	printf("Elements in list : %d\n", this->getDeviceList().size());
//	printf("Iterator at position %d\n", it-this->getDeviceList().end());
//	printf("END %d\n", this->getDeviceList().begin()-this->getDeviceList().end());
//
//	char size = this->getDeviceList().size();
//	char position = it-this->getDeviceList().end();
//	printf("size %d, position %d, size-position %d\n", size, position, (size-position));
//
//
//	if (!(size-position == 0)) {
//
//		printf("Device Found at position %d\n", it-this->getDeviceList().end());
//
//	} else {
//		printf("Device Not Found\n");
//	}
//}
