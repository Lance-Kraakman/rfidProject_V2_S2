/*
 * RfidTag.cpp
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include "RfidTag.h"

RfidTag::RfidTag() {
	// TODO Auto-generated constructor stub
	this->UUID = 0;
	this->scannedTime = SntpTime();
	this->msSinceStartup = 0;
}

RfidTag::RfidTag(int64_t UUID, SntpTime scannedTime, int64_t msSniceStartup) {
	// TODO Auto-generated constructor stub
	this->UUID = UUID;
	this->scannedTime = scannedTime;
	this->msSinceStartup = msSniceStartup;
	this->scannedTime = SntpTime();

}

RfidTag::RfidTag(int64_t UUID) {
	this->setUUID(UUID);
}

void RfidTag::setUUID(int64_t UUID) {
	this->UUID = UUID;
}

int64_t RfidTag::getUUID() {
	return this->UUID;
}

void RfidTag::updateTagScannedTime() {
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
	if (this->getUUID() == 0) {
		return true;
	}
	return false;
}

void RfidTag::printTag() {
	ESP_LOGI(RFID_TAG, "%lld : %lld", this->getUUID(), this->getStartupTime());
	this->getScannedTime().printTime();
}

