/*
 * EmployeeModel.cpp
 *
 *  Created on: 9/04/2021
 *      Author: lance
 */

#include "EmployeeModel.h"

EmployeeModel::EmployeeModel() {}

/**
 *
 * @return a reference to the EmployeeModels device list
 */
std::vector<Employee>& EmployeeModel::getEmployeeList() {
	return this->employeeList;
}

/** returns true if the Employee is found (found by UUID), EmployeeToCheck is set to the values of the found device.
 *
 * @param &employeeToCheck
 * @return true if Employee is found
 */
bool EmployeeModel::findEmployee(Employee& employeeToCheck) {
	std::vector<Employee> EmployeeList = this->employeeList;

	for (Employee emp : EmployeeList) {
		emp.printEmployee();
	}

	auto it = std::find(EmployeeList.begin(), EmployeeList.end(), employeeToCheck);
	if (it != EmployeeList.end()) {
		employeeToCheck = *it;
		return true;
	} else {
		return false;
	}
}

/** Updates the EmployeeToUpdate with the same uuid in the list with the device passed
 *
 * @param EmployeeToUpdate
 * @return bool
 */
bool EmployeeModel::updateEmployeeInList(Employee EmployeeToUpdate) {
	std::replace(this->employeeList.begin(), this->employeeList.end(), EmployeeToUpdate, EmployeeToUpdate);
	return true;
}

/** Adds the passed Employee to the models internal list
 *
 * @param emp - Adds Employee dev to the Employee model list
 */
void EmployeeModel::addEmployee(Employee emp) {
	if (!(this->findEmployee(emp))) {
		this->getEmployeeList().insert(this->getEmployeeList().begin(), emp);
	}
}


/** returns a Employee object from a Employee JSON object
 *
 * @warning - It is up to the user to deallocate memory! - (Created with Malloc so must use free())
 * @param jsonString
 * @return Device
 */
Employee EmployeeModel::employeeFromJson(cJSON *jsonString) {

	cJSON* name  = cJSON_GetObjectItem(jsonString, "name");
	cJSON* uuid = cJSON_GetObjectItem(jsonString, "uuid");
	cJSON* cJSON_ARR[] = {name, uuid};

	for (int i=0; i<2; i++) {
		if (cJSON_ARR[i] == NULL) {
			throw "ADD EMPLOYEE OBJECT ERROR";
		}
	}

	Employee emp = Employee();
	emp.setName(name->valuestring);

	if (uuid->type == cJSON_String) {
		emp.setTag(RfidTag(std::stoi(uuid->string)));
	} else if (uuid->type == cJSON_Number) {
		emp.setTag(RfidTag(uuid->valueint));
	} else {
		throw "Couldn't resolve cJSON_type";
	}

	return emp;
}

/** Functon returns a pointer to a cJSON object of Employees.
 *
 * @warning - It is up to the user to deallocate memory! - (Created with Malloc so must use free())
 * @param -
 * @return
 *
 */
cJSON *EmployeeModel::jsonEmployeeList() {
	cJSON *finalJSON = cJSON_CreateObject();
	cJSON *jsonArray = cJSON_AddArrayToObject(finalJSON, "EMPLOYEE LIST");
	for (Employee emp : this->getEmployeeList()) {
		cJSON *employeeJsonObject = cJSON_CreateObject();

		cJSON_AddStringToObject(employeeJsonObject, "name", emp.getName().c_str());
		cJSON_AddNumberToObject(employeeJsonObject, "uuid", emp.getTag().getUUID());
		cJSON_AddStringToObject(employeeJsonObject, "time", emp.getTag().getScannedTime().timeString().c_str());
		cJSON_AddNumberToObject(employeeJsonObject, "active", emp.isActive());
		cJSON_AddItemToArray(jsonArray, employeeJsonObject);
	}
	return finalJSON;
}
