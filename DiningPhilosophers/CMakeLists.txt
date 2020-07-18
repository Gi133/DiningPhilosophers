add_executable(${PROJECT_NAME})

if(UNIX)
target_link_options(${PROJECT_NAME}
	PUBLIC
		-lpthread
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

install(TARGETS ${PROJECT_NAME})