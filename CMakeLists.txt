cmake_minimum_required(VERSION 3.0)

# Define project name as the folder name.
get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})


if (UNIX)
    message(STATUS "UNIX")

    # Use C++ 11 standart.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package (Threads REQUIRED)
    
    # Use C 99 standart.
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endif (UNIX)


if (WIN32)
    message(STATUS "WIN32")

    if (MSVC)
        message(STATUS "MSVC")

        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /ZI /MP")

        # For precompiled header.
        # Set 
        # "Precompiled Header" to "Use (/Yu)"
        # "Precompiled Header File" to "stdafx.h"
        # set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Yustdafx.h /FIstdafx.h")

	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
    else()
        # Use C++ 11 standart.
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

        # Use C 99 standart.
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
    endif()

endif()


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${dir}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${dir}/bin")

# Add projects.
add_subdirectory(DiningPhilosophers)
