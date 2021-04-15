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

/** Tag handler function which adds tags to the list of the class
 *
 * @param serial_no
 */
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

/**
 *
 * @return a copy of the classes RfidTag list
 */
std::vector<RfidTag> RfidScanner::getRfidList() {
	return RfidScanner::RfidTagList;
}

/** Pops the next rfid tag from the classes list
 *
 * @return RfidTag
 */
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

/** reads the front of the rfid list
 *
 * @return RfidTag
 */
RfidTag RfidScanner::readRfidList() {
	if (!(RfidScanner::RfidTagList.empty())) {
			return RfidScanner::RfidTagList.front();
	} else {
		return RfidTag();
	}
}

/** reads the rfid list at speciied position
 *
 * @param position
 * @return RfidTag
 */
RfidTag RfidScanner::readRfidListFromPosition(uint position) {
	if (!(RfidScanner::RfidTagList.empty())) {
			return RfidScanner::RfidTagList.at(position);
	} else {
		return RfidTag();
	}
}

/** Returns true if the device has been tagged within microsecondTimeoutTime of the last tag
 *
 * @param recvdTag
 * @param previousTag
 * @param microsecondTimeoutTime
 * @return
 */
bool RfidScanner::isDoubleTagged(RfidTag recvdTag,RfidTag previousTag, int microsecondTimeoutTime) {
	printf("current %lld,prev %lld,diff %lld\n",recvdTag.getStartupTime(), previousTag.getStartupTime(),(recvdTag.getStartupTime() - previousTag.getStartupTime()));
	if ((recvdTag.getStartupTime() - previousTag.getStartupTime()) < microsecondTimeoutTime) {
			return true;
		}
	return false;
}

/** starts the rfid scanner
 *
 */
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

