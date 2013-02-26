set(framework_src
	FileLogPolicy.cpp
	PrintLogPolicy
	TimeUtil.cpp
	)
	
add_library(framework ${framework_src})
target_link_libraries(framework pthread rt)
install(TARGETS framework DESTINATION lib)

