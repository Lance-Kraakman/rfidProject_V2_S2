/*
 * Employee.cpp
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include "Employee.h"
#include <algorithm>
#include <string>
#include "soc/rtc_wdt.h"
#include "esp_int_wdt.h"
#include "esp_task_wdt.h"


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
 * Function Returns true and updates the device params if it finds the device!
 */
bool Employee::findDevice(Device& deviceToCheck) {

	std::vector<Device> DeviceList = this->getDeviceList();

	auto it = std::find(DeviceList.begin(), DeviceList.end(), deviceToCheck);

	if (it != DeviceList.end()) {
		printf("Find Complete -- Found\n");
		Device myDev = *it;
		std::string getName = myDev.getName();
		deviceToCheck.setName(getName);
		return true;
	} else {
		printf("Find Complete -- Not Found\n");
		return false;

	}

}


//bool Employee::checkDataReturned(std::vector<Device>::iterator it) {
//	if (it != this->getDeviceList().end()) {
//		return true;
//	} else {
//		return false;
//	}
//}

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
