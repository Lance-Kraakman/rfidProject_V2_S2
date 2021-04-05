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
}

Device::Device(RfidTag tag) {
	// TODO Auto-generated constructor stub
	this->setTag(tag);
	this->setName("No Name");

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

bool operator== (const Device &D1,const Device &D2) {
	return D1.tag.UUID == D2.tag.UUID;
}
