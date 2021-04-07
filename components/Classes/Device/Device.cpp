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

Device::Device(RfidTag tag) {
	// TODO Auto-generated constructor stub
	this->setTag(tag);
	this->setName("No Name");
	this->setHasEmployee(false);


}

RfidTag Device::getTag() {
	return this->tag;
}

void Device::setTag(RfidTag tag) {
	this->tag = tag;
}

void Device::setName(std::string Name) {
	this->name = Name;
}

std::string Device::getName() {
	return this->name;
}

bool Device::hasEmployee() {
	return this->hasEmp;
}

void Device::setHasEmployee(bool hasEmp) {
	this->hasEmp = hasEmp;
}

void Device::setEmployee(Employee emp) {
	this->emp = emp;
}

Employee Device::getEmployee() {
	return this->emp;
}

bool operator== (const Device &D1,const Device &D2) {
	return D1.tag.UUID == D2.tag.UUID;
}

void Device::printDevice() {
	ESP_LOGI(DEVICE_TAG, "DEVICE NAME: %s, UUID: %lld, HAS EMP: %d, EMPLOYEE : %s", this->getName().c_str(),
			this->getTag().getUUID(), this->hasEmployee(), this->getEmployee().getName().c_str());
}

