# Define project name as the folder name.
get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})


if (WIN32)
	if (MSVC)
		# For precompiled header.
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Yustdafx.h /FIstdafx.h")
	endif()
endif()	

file(GLOB_RECURSE INCS "*.h")

aux_source_directory(src SRCS)


add_executable(${PROJECT_NAME} ${SRCS} ${INCS})

if (WIN32)
	if (MSVC)
		# For precompiled header.
		set_source_files_properties(src/stdafx.cpp
			PROPERTIES
			COMPILE_FLAGS "/Ycstdafx.h"
		)
	endif()
endif()