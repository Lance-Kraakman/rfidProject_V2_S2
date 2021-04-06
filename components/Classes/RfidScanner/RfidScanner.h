/*
 * RfidScanner.h
 *
 *  Created on: 5/04/2021
 *      Author: lance
 */

#include <vector>
#include <string>
#include "../RfidTag/RfidTag.h"
extern "C" {
	#include "../../Drivers/esp-idf-rc522/rc522.h"
}
#define RFID_SCANNER_TAG "RFID SCANNER"
#define MICROSECOND_TIMEOUT_PERIOD 500000

#ifndef COMPONENTS_CLASSES_RFIDSCANNER_RFIDSCANNER_H_
#define COMPONENTS_CLASSES_RFIDSCANNER_RFIDSCANNER_H_

class RfidScanner {
private:
	static std::vector<RfidTag> RfidTagList;
	static void tag_handler(uint8_t* serial_no);
public:
	RfidScanner();
	void startRfidScanner();	// configures the RFID driver and starts it
	std::vector<RfidTag> getRfidList(); // Returns the rfid list
	void setRfidList(std::vector<RfidTag> RfidTaglist);
	RfidTag popRfidList(); // Pops the next RFID tag from the list
	RfidTag readRfidList(); // Reads the next Rfid Tag from the list
	RfidTag readRfidListFromPosition(uint position);
	bool isDoubleTagged(RfidTag recvdTag,RfidTag previousTag, int microsecondTimeoutTime);


};

#endif /* COMPONENTS_CLASSES_RFIDSCANNER_RFIDSCANNER_H_ */
