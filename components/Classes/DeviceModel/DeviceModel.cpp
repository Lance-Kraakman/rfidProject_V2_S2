/*
 * DeviceModel.cpp
 *
 *  Created on: 9/04/2021
 *      Author: lance
 */

#include "DeviceModel.h"

/** Default Constructor.
 *
 */
DeviceModel::DeviceModel() {
	this->deviceResponse = httpResponse("<h1> Default Data <h1>", HTTPD_RESP_USE_STRLEN, std::string("/devs"), std::string("application/json"), HTTP_GET);
}

/**
 *
 * @return a reference to the DeviceModels device list
 */
std::vector<Device>& DeviceModel::getDeviceList() {
	return this->deviceList;
}

/** returns true if the device is found (found by UUID), device to check is set to the values of the found device.
 *
 * @param &deviceToCheck
 * @return bool
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

/** Updates the device with the same uuid in the list with the device passed
 *
 * @param deviceToUpdate
 * @return bool
 */
bool DeviceModel::updateDeviceInList(Device deviceToUpdate) {
	std::replace(this->deviceList.begin(), this->deviceList.end(), deviceToUpdate, deviceToUpdate);
	return true;
}

/** Adds the passed device to the models internal list
 *
 * @param dev - Adds device dev to the device model list
 */
void DeviceModel::addDevice(Device dev) {
	if (!(this->findDevice(dev))) {
		this->getDeviceList().insert(this->getDeviceList().begin(), dev);
	}
}

/** returns a Device object from a Device JSON object
 *
 * @param recvdJSON
 * @return Device
 */
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

/** Functon returns a pointer to a cJSON object of devices.
 *
 * @author Lance Kraakman
 * @warning - It is up to the user to deallocate memory! - (Created with Malloc so must use free())
 * @param -
 * @return
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

void DeviceModel::updateDeviceResponse() {

	//this->jsonDeviceList()
	cJSON *finalJSON = this->jsonDeviceList();
	std::string dataString = std::string(cJSON_Print(finalJSON));

	this->deviceResponse.updateData(dataString);
	if (finalJSON != NULL) {
		free(finalJSON);
		finalJSON = NULL;

	}
}

