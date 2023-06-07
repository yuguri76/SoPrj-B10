include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

function(check_submodules_present)
	file(READ "${CMAKE_SOURCE_DIR}/.gitmodules" gitmodules)
	string(REGEX MATCHALL "path *= *[^ \t\r\n]*" gitmodules ${gitmodules})

	foreach(module ${gitmodules})
		# Get module name
		string(REGEX REPLACE "path *= *" "" module ${module})

		# Stat the folder and get ammount of entries
		file(GLOB RESULT "${CMAKE_SOURCE_DIR}/${module}/*")
		list(LENGTH RESULT RES_LEN)

		# If the module has no files, bring fatal error
		if(RES_LEN EQUAL 0)
			# directory is empty
			message(FATAL_ERROR "Submodule ${module} is empty. "
				"Please run: git submodule update --init --recursive")
		endif()
	endforeach()
endfunction()

check_submodules_present()

## DIRENT for windows
add_library(dirent INTERFACE)
if(WIN32)
	target_include_directories(dirent INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/dirent/include")
endif()

## Unity
add_library(unity STATIC "${CMAKE_CURRENT_SOURCE_DIR}/unity/src/unity.c")
target_include_directories(unity PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/unity/src")
set_property(TARGET unity PROPERTY FOLDER externals)