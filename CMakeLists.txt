# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

#If IOT_SOLUTION_PATH is not defined, use relative path as default value
if(NOT DEFINED ENV{IOT_SOLUTION_PATH})
    get_filename_component(IOT_SOLUTION_PATH "${CMAKE_SOURCE_DIR}/../../.." ABSOLUTE)
    set(ENV{IOT_SOLUTION_PATH} ${IOT_SOLUTION_PATH})
    message(WARNING "Can't detect IOT_SOLUTION_PATH in your environment, we infer it is $ENV{IOT_SOLUTION_PATH}")
endif()

# CPP Component Dirs
set(EXTRA_COMPONENT_DIRS "./components/Classes/Networking/Messaging" "./components/Classes/Networking/MqttService" "$ENV{IDF_PATH}/examples/common_components/protocol_examples_common"
	"./components/Classes/RfidTag" "./components/Classes/Device" "./components/Classes/Employee" "./components/Classes/MyTime" "./components/Classes/RfidScanner"
		"./components/Drivers/esp-idf-rc522" "./components/Classes/Networking/Mdns" "./components/Classes/Application/MainApplication" "./components/Classes/Networking/Wifi"
		"./components/Classes/simpleson" "./components/Classes/Application/DataProcessing" "./components/Classes/Application/TagProcessing" "./components/Classes/EmployeeModel"
		"./components/Classes/DeviceModel")
		
set(EXTRA_INCLUDE "./components/Classes/Networking/Messaging" "./components/Classes/Networking/MqttService" "$ENV{IDF_PATH}/examples/common_components/protocol_examples_common"
	"./components/Classes/RfidTag" "./components/Classes/Device" "./components/Classes/Employee" "./components/Classes/MyTime" "./components/Classes/RfidScanner"
		"./components/Drivers/esp-idf-rc522" "./components/Classes/Networking/Mdns" "./components/Classes/Application/MainApplication" "./components/Classes/Networking/Wifi" 
		"./components/Classes/simpleson" "./components/Classes/Application/DataProcessing" "./components/Classes/Application/TagProcessing" "./components/Classes/EmployeeModel"
		"./components/Classes/DeviceModel")

# Set Compiler Flags <3
set_source_files_properties(SRCS ${EXTRA_COMPONENT_DIRS} PROPERTIES COMPILE_FLAGS "-fexceptions -Wno-missing-field-initializers") # add compiler flags to sources in COMPONENT_DIRS

include($ENV{IOT_SOLUTION_PATH}/component.cmake)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)


project(main)
