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
	this->scannedTime = SntpTime();
	this->msSinceStartup = 0;
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

void RfidTag::setTimeAsCurrent() {
	this->getScannedTime().updateToCurrentTime();
}

void RfidTag::setScannedTime(SntpTime scannedTime) {
	this->scannedTime = scannedTime;
}

// Returns when the tag was scanned
SntpTime RfidTag::getScannedTime() {
	return this->scannedTime;
}

void RfidTag::setStartupTime(int64_t startupTime) {
	this->msSinceStartup = startupTime;
}

int64_t RfidTag::getStartupTime() {
	return this->msSinceStartup;
}

bool RfidTag::isEmpty() {
	if (this->getUUID().compare(std::string("")) == 0) {
		return false;
	}
	return true;
}

