/*
 * RfidTag.h
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include <string>
#include "../MyTime/SntpTime.h"

#ifndef COMPONENTS_CLASSES_RFID_RFIDTAG_RFIDTAG_H_
#define COMPONENTS_CLASSES_RFID_RFIDTAG_RFIDTAG_H_

class RfidTag {
private:
	SntpTime scannedTime; // Returns when the RfidTag Was last scanned
	int64_t msSinceStartup;
public:
	std::string UUID; // Hast to be public so Device search can access it

	RfidTag();
	RfidTag(std::string UUID);
	void setUUID(std::string);
	std::string getUUID();
	void setTimeAsCurrent();
	void setScannedTime(SntpTime scannedTime);
	SntpTime getScannedTime(); // Returns when the tag was scanned
	int64_t getStartupTime();
	void setStartupTime(int64_t startupTime);
	bool isEmpty();

};

#endif /* COMPONENTS_CLASSES_RFID_RFIDTAG_RFIDTAG_H_ */
