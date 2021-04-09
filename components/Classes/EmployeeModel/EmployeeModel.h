/*
 * EmployeeModel.h
 *
 *  Created on: 9/04/2021
 *      Author: lance
 */
#include <vector>
#include <algorithm>
#include "../Employee/Employee.h"

#include "cJSON.h"



#ifndef COMPONENTS_CLASSES_EMPLOYEEMODEL_EMPLOYEEMODEL_H_
#define COMPONENTS_CLASSES_EMPLOYEEMODEL_EMPLOYEEMODEL_H_

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

};

#endif /* COMPONENTS_CLASSES_EMPLOYEEMODEL_EMPLOYEEMODEL_H_ */
