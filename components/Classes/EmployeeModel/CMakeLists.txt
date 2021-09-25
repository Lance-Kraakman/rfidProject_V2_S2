idf_component_register(SRCS "EmployeeModel.cpp" INCLUDE_DIRS "/." REQUIRES WebServer json Employee)


set_source_files_properties(EmployeeModel.cpp PROPERTIES COMPILE_FLAGS -fexceptions)





