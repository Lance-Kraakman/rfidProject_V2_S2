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
	//this->deviceList = std::vector<Device>();
	this->setName("");
	this->setTag(RfidTag());
	this->setActive(false);

}

Employee::Employee(std::string name, RfidTag tag) {
	this->setName(name);
	this->setTag(tag);
	//this->deviceList = std::vector<Device>();
	this->setActive(false);

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

void Employee::setActive(bool active) {
	this->activeStatus = active;
}

bool Employee::isActive() {
	return this->activeStatus;
}

bool operator== (const Employee &E1,const Employee &E2) {
	return E1.tag.UUID == E2.tag.UUID;
}

void Employee::printEmployee() {
	ESP_LOGI(EMPLOYEE_TAG, "NAME: %s, UUID: %lld, STATUS: %d", this->getName().c_str(), this->getTag().getUUID(), this->isActive());
}
