/*
 * DeviceModel.cpp
 *
 *  Created on: 9/04/2021
 *      Author: lance
 */

#include "DeviceModel.h"

DeviceModel::DeviceModel() {
	// TODO Auto-generated constructor stub
	// Create some tags and devices for testing
	Device dev1 = Device(RfidTag(196609223));
	Device dev2 = Device(RfidTag(194315418));
	Device dev3 = Device(RfidTag(196347067));

	this->deviceList.push_back(dev1);
	this->deviceList.push_back(dev2);
	this->deviceList.push_back(dev3);

}

std::vector<Device>& DeviceModel::getDeviceList() {
	return this->deviceList;
}

/**
 * Searches for Device, if an Device is found it is updated with the values found in the list
 */
bool DeviceModel::findDevice(Device& deviceToCheck) {
	std::vector<Device> DeviceList = this->deviceList;

	auto it = std::find(DeviceList.begin(), DeviceList.end(), deviceToCheck);
	if (it != DeviceList.end()) {
		deviceToCheck = *it; // Update the devices other params
		return true;
	}
	return false;
}

bool DeviceModel::updateDeviceInList(Device deviceToUpdate) {
	std::replace(this->deviceList.begin(), this->deviceList.end(), deviceToUpdate, deviceToUpdate);
	return true;
}

void DeviceModel::addDevice(Device dev) {
	if (!(this->findDevice(dev))) {
		this->getDeviceList().insert(this->getDeviceList().begin(), dev);
	}
}

Device DeviceModel::deviceFromJson(cJSON *recvdJSON) {

	cJSON* name  = cJSON_GetObjectItem(recvdJSON, "name");
	cJSON* uuid = cJSON_GetObjectItem(recvdJSON, "uuid");
	cJSON* cJSON_ARR[] = {name, uuid};

	for (int i=0; i<2; i++) {
		if (cJSON_ARR[i] == NULL) {
			throw "ADD DEVICE OBJECT ERROR";
		}
	}

	Device dev = Device();
	dev.setName(name->valuestring);
	if (uuid->type == cJSON_String) {
		dev.setTag(RfidTag(std::stoi(uuid->string)));
	} else if (uuid->type == cJSON_Number) {
		dev.setTag(RfidTag(uuid->valueint));
	} else {
		throw "Couldn't resolve cJSON_type";
	}

	return dev;
}

/*!
 * @Warn - It is up to the user to deallocate memory! - (Created with Malloc so must use free())
 *
 */
cJSON * DeviceModel::jsonDeviceList() {
	cJSON *finalJSON = cJSON_CreateObject();
	cJSON *jsonArray = cJSON_AddArrayToObject(finalJSON, "DEVICE LIST");
	for (Device dev : this->getDeviceList()) {
		cJSON *deviceJsonObject = cJSON_CreateObject();

		cJSON_AddStringToObject(deviceJsonObject, "name", dev.getName().c_str());
		cJSON_AddNumberToObject(deviceJsonObject, "uuid", dev.getTag().getUUID());
		cJSON_AddStringToObject(deviceJsonObject, "time", dev.getTag().getScannedTime().timeString().c_str());
		cJSON_AddNumberToObject(deviceJsonObject, "has-employee", dev.hasEmployee());
		cJSON_AddNumberToObject(deviceJsonObject, "employee-uuid", dev.getEmployee().getTag().getUUID() );

		cJSON_AddItemToArray(jsonArray, deviceJsonObject);
	}
	return finalJSON;
}

