idf_component_register(SRCS "main.cpp" INCLUDE_DIRS "/." REQUIRES MainApplication)

set_source_files_properties(main.cpp PROPERTIES COMPILE_FLAGS -fexceptions)
