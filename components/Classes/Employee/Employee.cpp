/*
 * Employee.cpp
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include "Employee.h"

Employee::Employee() {
	// TODO Auto-generated constructor stub
	//this->deviceList = std::vector<Device>();
	this->setName("");
	this->setTag(RfidTag());
	this->setActive(false);

}

/**
 *
 * @param name
 * @param tag
 */
Employee::Employee(std::string name, RfidTag tag) {
	this->setName(name);
	this->setTag(tag);
	//this->deviceList = std::vector<Device>();
	this->setActive(false);

}

/**
 *
 * @param name
 */
void Employee::setName(std::string name) {
	this->name = name;
}

/**
 *
 * @return name
 */
std::string Employee::getName() {
	return this->name;
}

/**
 *
 * @param tag
 */
void Employee::setTag(RfidTag tag) {
	this->tag = tag;
}

/**
 *
 * @return RfidTag
 */
RfidTag Employee::getTag() {
	return this->tag;
}

/**
 *
 * @param active
 */
void Employee::setActive(bool active) {
	this->activeStatus = active;
}

/**
 *
 * @return bool if employee tag is active - representing it has been scanned recently
 */
bool Employee::isActive() {
	return this->activeStatus;
}

bool operator== (const Employee &E1,const Employee &E2) {
	return E1.tag.UUID == E2.tag.UUID;
}

/** Prints the devices paramaters
 *
 */
void Employee::printEmployee() {
	ESP_LOGI(EMPLOYEE_TAG, "NAME: %s, UUID: %lld, STATUS: %d", this->getName().c_str(), this->getTag().getUUID(), this->isActive());
}
