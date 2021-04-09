idf_component_register(SRCS "DataProcessing.cpp" INCLUDE_DIRS "/." REQUIRES DeviceModel EmployeeModel json Wifi RfidScanner MyTime Employee Device RfidTag Messaging MqttService nvs_flash protocol_examples_common)

set_source_files_properties(DataProcessing.cpp PROPERTIES COMPILE_FLAGS -fexceptions)






