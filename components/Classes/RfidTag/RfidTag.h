/*
 * RfidTag.h
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include <string>

#ifndef COMPONENTS_CLASSES_RFID_RFIDTAG_RFIDTAG_H_
#define COMPONENTS_CLASSES_RFID_RFIDTAG_RFIDTAG_H_

class RfidTag {
private:
	std::string UUID;
public:
	RfidTag();
	RfidTag(std::string UUID);
	void setUUID(std::string);
	std::string getUUID();

};

#endif /* COMPONENTS_CLASSES_RFID_RFIDTAG_RFIDTAG_H_ */
