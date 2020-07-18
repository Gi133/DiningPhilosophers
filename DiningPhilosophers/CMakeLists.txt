add_executable(${PROJECT_NAME})

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
		
		source.cpp
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		Headers
)

install(TARGETS ${PROJECT_NAME})