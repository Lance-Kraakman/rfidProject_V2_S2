idf_component_register(SRCS "DeviceModel.cpp" INCLUDE_DIRS "/." REQUIRES json Device)


set_source_files_properties(DeviceModel.cpp PROPERTIES COMPILE_FLAGS -fexceptions)





