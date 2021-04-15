PROJECT_NAME := main
IOT_SOLUTION_PATH ?= $(abspath $(shell pwd)/../../)

EXTRA_COMPONENT_DIRS += ./components/
EXTRA_COMPONENT_DIRS += ./components/Classes/Networking/Messaging
EXTRA_COMPONENT_DIRS += ./components/Classes/Networking/MqttService
EXTRA_COMPONENT_DIRS += ENV{IDF_PATH}/examples/common_components/protocol_examples_common
EXTRA_COMPONENT_DIRS += ./components/Classes/RfidTag
EXTRA_COMPONENT_DIRS += ./components/Classes/Device
EXTRA_COMPONENT_DIRS += ./components/Classes/Employee
EXTRA_COMPONENT_DIRS += ./components/Classes/MyTime
EXTRA_COMPONENT_DIRS += ./components/Classes/RfidScanner
EXTRA_COMPONENT_DIRS += ./components/Classes/Application/DataProcessing
EXTRA_COMPONENT_DIRS += ./components/Classes/Application/TagProcessing
EXTRA_COMPONENT_DIRS += ./components/Classes/EmployeeModel
EXTRA_COMPONENT_DIRS += ./components/Classes/DeviceModel
EXTRA_COMPONENT_DIRS += ./components/Classes/Display
EXTRA_COMPONENT_DIRS += ./components/Classes/Application/MainApplication

EXTRA_INCLUDE_DIRS += ./components/
EXTRA_INCLUDE_DIRS += ./components/Classes/Networking/Messaging
EXTRA_INCLUDE_DIRS += ./components/Classes/Networking/MqttService
EXTRA_INCLUDE_DIRS += ENV{IDF_PATH}/examples/common_components/protocol_examples_common
EXTRA_INCLUDE_DIRS += ./components/Classes/RfidTag
EXTRA_INCLUDE_DIRS += ./components/Classes/Device
EXTRA_INCLUDE_DIRS += ./components/Classes/Employee
EXTRA_INCLUDE_DIRS += ./components/Classes/MyTime
EXTRA_INCLUDE_DIRS += ./components/Classes/RfidScanner
EXTRA_INCLUDE_DIRS += ./components/Classes/Application/DataProcessing
EXTRA_INCLUDE_DIRS += ./components/Classes/Application/TagProcessing
EXTRA_INCLUDE_DIRS += ./components/Classes/EmployeeModel
EXTRA_INCLUDE_DIRS += ./components/Classes/DeviceModel
EXTRA_INCLUDE_DIRS += ./components/Classes/Display
EXTRA_INCLUDE_DIRS += ./components/Classes/Application/MainApplication


