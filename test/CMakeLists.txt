add_executable(tests tests.c)
target_link_libraries(tests PRIVATE core unity)
set_property(TARGET tests PROPERTY FOLDER POS)
set_target_properties(tests PROPERTIES OUTPUT_NAME POS-Tests
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)
