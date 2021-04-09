idf_component_register(SRCS "Device.cpp" INCLUDE_DIRS "/." REQUIRES json RfidTag MyTime Employee)

set_source_files_properties(Device.cpp PROPERTIES COMPILE_FLAGS -fexceptions)






