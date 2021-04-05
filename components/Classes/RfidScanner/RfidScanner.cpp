/*
 * RfidScanner.cpp
 *
 *  Created on: 5/04/2021
 *      Author: lance
 */

#include "RfidScanner.h"

std::vector<RfidTag> RfidScanner::RfidTagList = std::vector<RfidTag>();

RfidScanner::RfidScanner() {
	// TODO Auto-generated constructor stub

}

void RfidScanner::tag_handler(uint8_t* serial_no) {
	RfidTag scannedTag;
	std::string uuid_string = std::string((char *) serial_no);
	scannedTag.setUUID(uuid_string);
	ESP_LOGI(RFID_SCANNER_TAG, "SCANNED TAG");

	// Add data to tag to check scanned Tag
	scannedTag.setTimeAsCurrent();
	scannedTag.setStartupTime(esp_timer_get_time());

	RfidScanner::RfidTagList.insert(RfidScanner::RfidTagList.end(), scannedTag);

}

std::vector<RfidTag> RfidScanner::getRfidList() {
	return this->RfidTagList;
}

// Pops the next RFID tag from the list
RfidTag RfidScanner::popRfidList() {
	if (!(this->getRfidList().empty())) {
		RfidTag returnItem = this->getRfidList().front();
		this->getRfidList().erase(this->getRfidList().begin());
		return returnItem;
	} else {
		return RfidTag();
	}
}

// Reads the next Rfid Tag from the list
RfidTag RfidScanner::readRfidList() {
	if (!(this->getRfidList().empty())) {
			return this->getRfidList().front();
	} else {
		return RfidTag();
	}
}

// Config RFID Inputs
void RfidScanner::startRfidScanner() {

	const rc522_start_args_t start_args = {
	        .miso_io  = 17,
	        .mosi_io  = 16,
	        .sck_io   = 15,
	        .sda_io   = 14,
	        .callback = &(this->tag_handler)
	};

	rc522_start(start_args);
}

