/*
 * DeviceModel.h
 *
 *  Created on: 9/04/2021
 *      Author: lance
 */
#include <vector>
#include <algorithm>
#include "../Device/Device.h"
#include "cJSON.h"

#ifndef COMPONENTS_DEVICEMODEL_DEVICEMODEL_H_
#define COMPONENTS_DEVICEMODEL_DEVICEMODEL_H_

/** \brief Represents a Device list and provides access and update abstraction.
 *
 */
class DeviceModel {
	std::vector<Device> deviceList;
public:
	DeviceModel();
	std::vector<Device>& getDeviceList();
	bool findDevice(Device& deviceToCheck);
	bool updateDeviceInList(Device deviceToUpdate);
	Device deviceFromJson(cJSON *recvdJSON);
	void addDevice(Device dev);
	cJSON *jsonDeviceList();
};

#endif /* COMPONENTS_DEVICEMODEL_DEVICEMODEL_H_ */
