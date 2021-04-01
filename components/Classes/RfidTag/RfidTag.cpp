/*
 * RfidTag.cpp
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include "RfidTag.h"

RfidTag::RfidTag() {
	// TODO Auto-generated constructor stub
	this->UUID = "";
}

RfidTag::RfidTag(std::string UUID) {
	this->setUUID(UUID);
}

void RfidTag::setUUID(std::string UUID) {
	this->UUID = UUID;
}

std::string RfidTag::getUUID() {
	return this->UUID;
}
