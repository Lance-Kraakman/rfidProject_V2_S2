/*
 * Device.cpp
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include "Device.h"

Device::Device() {
	// TODO Auto-generated constructor stub
	this->setName("No Name");
	this->setHasEmployee(false);
}

/**
 *
 * @param tag
 */
Device::Device(RfidTag tag) {
	// TODO Auto-generated constructor stub
	this->setTag(tag);
	this->setName("No Name");
	this->setHasEmployee(false);


}

/**
 *
 * @return RfidTag
 */
RfidTag Device::getTag() {
	return this->tag;
}

/**
 *
 * @param tag
 */
void Device::setTag(RfidTag tag) {
	this->tag = tag;
}

/**
 *
 * @param Name
 */
void Device::setName(std::string Name) {
	this->name = Name;
}

/**
 *
 * @return Name
 */
std::string Device::getName() {
	return this->name;
}

/**
 *
 * @return bool hasEmp
 */
bool Device::hasEmployee() {
	return this->hasEmp;
}

/**
 *
 * @param hasEmp
 */
void Device::setHasEmployee(bool hasEmp) {
	this->hasEmp = hasEmp;
}

/**
 *
 * @param emp
 */
void Device::setEmployee(Employee emp) {
	this->emp = emp;
}

/**
 *
 * @return Devices employee
 */
Employee Device::getEmployee() {
	return this->emp;
}

/** Prints the devices paramaters
 *
 */
void Device::printDevice() {
	ESP_LOGI(DEVICE_TAG, "DEVICE NAME: %s, UUID: %lld, HAS EMP: %d, EMPLOYEE : %s", this->getName().c_str(),
			this->getTag().getUUID(), this->hasEmployee(), this->getEmployee().getName().c_str());
}

bool operator== (const Device &D1,const Device &D2) {
	return D1.tag.UUID == D2.tag.UUID;
}
