/*
 * Device.h
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include "../RfidTag/RfidTag.h"

#ifndef COMPONENTS_CLASSES_DEVICE_DEVICE_H_
#define COMPONENTS_CLASSES_DEVICE_DEVICE_H_

class Device {
private:
	std::string name;
	RfidTag tag;
public:
	Device();
	Device(RfidTag tag);
	friend bool operator== (const Device &D1, const Device &D2); // Search Function for has Device and getDeviceWithUUID <3
	RfidTag getTag();
	void setTag(RfidTag tag);
	void setName(std::string name);
	std::string getName();
};

#endif /* COMPONENTS_CLASSES_DEVICE_DEVICE_H_ */
