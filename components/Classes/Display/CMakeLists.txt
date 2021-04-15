idf_component_register(SRCS "DisplayDriver.cpp" INCLUDE_DIRS "/." REQUIRES boards screen lvgl_gui HomePage)

set_source_files_properties(DisplayDriver.cpp PROPERTIES COMPILE_FLAGS -fexceptions)





