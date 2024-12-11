# *****************************************************
#    Copyright 2022 Videonetics Technology Pvt Ltd
# *****************************************************



add_executable(tests_frameutil
	src/test.cpp
)

target_link_libraries(tests_frameutil
	PRIVATE frameutil
)
