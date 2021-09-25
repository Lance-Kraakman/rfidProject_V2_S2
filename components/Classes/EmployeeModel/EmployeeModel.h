/*
 * EmployeeModel.h
 *
 *  Created on: 9/04/2021
 *      Author: lance
 */
#include <vector>
#include <algorithm>
#include "../Employee/Employee.h"
#include "../Networking/WebServer/WebServer.h"
#include "cJSON.h"



#ifndef COMPONENTS_CLASSES_EMPLOYEEMODEL_EMPLOYEEMODEL_H_
#define COMPONENTS_CLASSES_EMPLOYEEMODEL_EMPLOYEEMODEL_H_

/** \brief Represents a Employee list and provides access, update functionality and some json abstraction
 *
 */
class EmployeeModel {
private:
	std::vector<Employee> employeeList;
public:
	EmployeeModel();
	std::vector<Employee>& getEmployeeList();
	bool findEmployee(Employee& employeeToCheck);
	bool updateEmployeeInList(Employee EmployeeToUpdate);
	Employee employeeFromJson(cJSON *jsonString);
	void addEmployee(Employee emp);
	cJSON *jsonEmployeeList();

	httpResponse employeeResponse;
	void updateEmployeeResponse();



};

#endif /* COMPONENTS_CLASSES_EMPLOYEEMODEL_EMPLOYEEMODEL_H_ */
