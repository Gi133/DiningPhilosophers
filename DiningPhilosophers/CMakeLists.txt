add_executable(${PROJECT_NAME})

if(UNIX)
target_link_options(${PROJECT_NAME}
	PUBLIC
		-pthread
)
endif(UNIX)

target_compile_features(${PROJECT_NAME} 
	PUBLIC 
		cxx_std_11
)

set_target_properties(${PROJECT_NAME} 
	PROPERTIES 
		CXX_EXTENSIONS OFF
)

target_sources(${PROJECT_NAME}
	PRIVATE
		Headers/Fork.h
		Headers/Philosopher.h
		Headers/PhilosopherThread.h
		
		Source/Fork.cpp
		Source/Philosopher.cpp
		Source/PhilosopherThread.cpp
		
		Source.cpp
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		Headers
)

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/PhilosopherNames.txt
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
)

install(TARGETS ${PROJECT_NAME}
	CONFIGURATIONS Debug
	RUNTIME DESTINATION Debug/bin
)

install(TARGETS ${PROJECT_NAME}
	CONFIGURATIONS Release
	RUNTIME DESTINATION Release/bin
)