/*
 * EmployeeModel.cpp
 *
 *  Created on: 9/04/2021
 *      Author: lance
 */

#include "EmployeeModel.h"

EmployeeModel::EmployeeModel() {
	// TODO Auto-generated constructor stub
	// Dummy Objects
	Employee emp1 = Employee("Lance", RfidTag(195232940));
	Employee emp2 = Employee("Nicole", RfidTag(194970784));
	Employee emp3 = Employee("Charlie", RfidTag(194577574));

	this->employeeList.push_back(emp1);
	this->employeeList.push_back(emp2);
	this->employeeList.push_back(emp3);
}

std::vector<Employee>& EmployeeModel::getEmployeeList() {
	return this->employeeList;
}

/*
 * Searches for employee, if an employee is found it is updated with the values found in the list
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


bool EmployeeModel::updateEmployeeInList(Employee EmployeeToUpdate) {
	std::replace(this->employeeList.begin(), this->employeeList.end(), EmployeeToUpdate, EmployeeToUpdate);
	return true;
}

void EmployeeModel::addEmployee(Employee emp) {
	if (!(this->findEmployee(emp))) {
		this->getEmployeeList().insert(this->getEmployeeList().begin(), emp);
	}
}

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

/**
 * @Warning - It is up to the user to deallocate memory! - (Created with Malloc so must use free())
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
