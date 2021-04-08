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
	RfidTag scannedTag = RfidTag();
	int uuid = 0;
	for (int i=0; i<5; i++) {
		uuid += (*(serial_no+i)) << (i*8);
	}
	//std::string uuid_string = std::string((int *) serial_no);
	scannedTag.setUUID(uuid);
	ESP_LOGI(RFID_SCANNER_TAG, "SCANNED TAG");
	ESP_LOGI(RFID_SCANNER_TAG, "UUID %lld", scannedTag.getUUID());

	// Add data to tag to check scanned Tag
	scannedTag.updateTagScannedTime();
	scannedTag.setStartupTime(esp_timer_get_time());
	scannedTag.printTag();

	RfidScanner::RfidTagList.insert(RfidScanner::RfidTagList.begin(), scannedTag);

}

std::vector<RfidTag> RfidScanner::getRfidList() {
	return RfidScanner::RfidTagList;
}

// Pops the next RFID tag from the list
RfidTag RfidScanner::popRfidList() {

	if (!(RfidScanner::RfidTagList.empty())) {
		RfidTag returnItemRef = RfidScanner::RfidTagList.back();
		RfidScanner::RfidTagList.pop_back();
		RfidTag returnItem = RfidTag(returnItemRef.getUUID(), returnItemRef.getScannedTime(), returnItemRef.getStartupTime());
		return returnItem;
	} else {
		return RfidTag();
	}
}

// Reads the next Rfid Tag from the list
RfidTag RfidScanner::readRfidList() {
	if (!(RfidScanner::RfidTagList.empty())) {
			return RfidScanner::RfidTagList.front();
	} else {
		return RfidTag();
	}
}

// Reads the next Rfid Tag from the list
RfidTag RfidScanner::readRfidListFromPosition(uint position) {
	if (!(RfidScanner::RfidTagList.empty())) {
			return RfidScanner::RfidTagList.at(position);
	} else {
		return RfidTag();
	}
}

bool RfidScanner::isDoubleTagged(RfidTag recvdTag,RfidTag previousTag, int microsecondTimeoutTime) {
	if ((recvdTag.getStartupTime() - previousTag.getStartupTime()) < microsecondTimeoutTime) {
			return true;
		}
	return false;
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

