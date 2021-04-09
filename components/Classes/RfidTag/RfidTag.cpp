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

/**
 *
 * @param UUID
 * @param scannedTime
 * @param msSniceStartup
 */
RfidTag::RfidTag(int64_t UUID, SntpTime scannedTime, int64_t msSniceStartup) {
	// TODO Auto-generated constructor stub
	this->UUID = UUID;
	this->scannedTime = scannedTime;
	this->msSinceStartup = msSniceStartup;
	this->scannedTime = SntpTime();

}

/**
 *
 * @param UUID
 */
RfidTag::RfidTag(int64_t UUID) {
	this->setUUID(UUID);
}

/**
 *
 * @param UUID
 */
void RfidTag::setUUID(int64_t UUID) {
	this->UUID = UUID;
}

/**
 *
 * @return UUID
 */
int64_t RfidTag::getUUID() {
	return this->UUID;
}

/** sets the RfidTag scan time to the current time.
 *
 */
void RfidTag::updateTagScannedTime() {
	this->getScannedTime().updateToCurrentTime();
}

/**
 *
 * @param scannedTime
 */
void RfidTag::setScannedTime(SntpTime scannedTime) {
	this->scannedTime = scannedTime;
}

/**
 *
 * @return
 */
SntpTime RfidTag::getScannedTime() {
	return this->scannedTime;
}

/** Represents time in us since micro-controller start-up
 *
 * @param startupTime
 */
void RfidTag::setStartupTime(int64_t startupTime) {
	this->msSinceStartup = startupTime;
}

/**
 *
 * @return
 */
int64_t RfidTag::getStartupTime() {
	return this->msSinceStartup;
}

/**
 *
 * @return
 */
bool RfidTag::isEmpty() {
	if (this->getUUID() == 0) {
		return true;
	}
	return false;
}

/**
 *
 */
void RfidTag::printTag() {
	ESP_LOGI(RFID_TAG, "%lld : %lld", this->getUUID(), this->getStartupTime());
	this->getScannedTime().printTime();
}

/** Checks the time between the RfidTag and another dosen't exceed macSeconds
 *
 * @param tagTwo
 * @param maxSeconds
 * @return
 */
bool RfidTag::checkTagTimeoutSeconds(RfidTag tagTwo, int maxSeconds) {
	SntpTime first = this->getScannedTime();
	SntpTime second = tagTwo.getScannedTime();

	int firstTagSecondsToday = (first.hour * 60 * 60) + (first.minute * 60) + (first.second);
	int secondSecondsToday = (second.hour * 60 * 60) + (second.minute * 60) + (second.second);

	if ((abs(firstTagSecondsToday - secondSecondsToday) < maxSeconds)) {
		return true;
	}
	return false;
}

