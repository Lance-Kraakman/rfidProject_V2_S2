idf_component_register(SRCS "TagProcessing.cpp" INCLUDE_DIRS "/." REQUIRES Display DataProcessing Wifi RfidScanner MyTime Employee Device RfidTag Messaging MqttService nvs_flash protocol_examples_common)

set_source_files_properties(TagProcessing.cpp PROPERTIES COMPILE_FLAGS -fexceptions)






