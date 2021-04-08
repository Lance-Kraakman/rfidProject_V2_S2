/*
 * EmployeeModel.h
 *
 *  Created on: 9/04/2021
 *      Author: lance
 */
#include <vector>
#include "../Employee/Employee.h"

#ifndef COMPONENTS_CLASSES_EMPLOYEEMODEL_EMPLOYEEMODEL_H_
#define COMPONENTS_CLASSES_EMPLOYEEMODEL_EMPLOYEEMODEL_H_

class EmployeeModel {
private:
	std::vector<Employee> employeeList;
public:
	EmployeeModel();
};

#endif /* COMPONENTS_CLASSES_EMPLOYEEMODEL_EMPLOYEEMODEL_H_ */
