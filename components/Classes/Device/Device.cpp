/*
 * Device.cpp
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include "Device.h"

Device::Device() {
	// TODO Auto-generated constructor stub

}

Device::Device(RfidTag tag) {
	// TODO Auto-generated constructor stub
	this->setTag(tag);

}

RfidTag Device::getTag() {
	return this->tag;
}

void Device::setTag(RfidTag tag) {
	this->tag = tag;
}

bool operator== (const Device &D1,const Device &D2) {

	return D1.tag.UUID == D2.tag.UUID;
}

