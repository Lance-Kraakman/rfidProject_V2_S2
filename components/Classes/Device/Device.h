/*
 * Device.h
 *
 *  Created on: 1/04/2021
 *      Author: lance
 */

#include <string>
#include "../RfidTag/RfidTag.h"

#ifndef COMPONENTS_CLASSES_DEVICE_DEVICE_H_
#define COMPONENTS_CLASSES_DEVICE_DEVICE_H_

class Device {
private:
	std::string name;
	RfidTag tag;
	std::string taggedTime;
public:
	Device();
};

#endif /* COMPONENTS_CLASSES_DEVICE_DEVICE_H_ */
