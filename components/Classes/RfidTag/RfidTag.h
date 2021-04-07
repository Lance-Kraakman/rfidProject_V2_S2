/*
 * RfidTag.h
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include <string>
#include "../MyTime/SntpTime.h"

#define RFID_TAG "RFID TAG"

#ifndef COMPONENTS_CLASSES_RFID_RFIDTAG_RFIDTAG_H_
#define COMPONENTS_CLASSES_RFID_RFIDTAG_RFIDTAG_H_

class RfidTag {
private:
	SntpTime scannedTime; // Returns when the RfidTag Was last scanned
	int64_t msSinceStartup;
public:
	int64_t UUID; // Hast to be public so Device search can access it

	RfidTag();
	RfidTag(int64_t UUID, SntpTime scannedTime, int64_t msSniceStartup);
	RfidTag(int64_t UUID);
	void setUUID(int64_t UUID);
	int64_t getUUID();
	void updateTagScannedTime();
	void setScannedTime(SntpTime scannedTime);
	SntpTime getScannedTime(); // Returns when the tag was scanned
	int64_t getStartupTime();
	void setStartupTime(int64_t startupTime);
	bool isEmpty();
	void printTag();

};

#endif /* COMPONENTS_CLASSES_RFID_RFIDTAG_RFIDTAG_H_ */
