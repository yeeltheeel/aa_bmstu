cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(test_project LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")

add_executable(main "lab5.cpp" "conv.cpp")
add_executable(unittest "unit-testing.cpp" "conv.cpp")

set_target_properties(
	main PROPERTIES 
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

set_target_properties(
	unittest PROPERTIES 
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)
target_link_libraries(unittest gtest)
target_compile_features(unittest PRIVATE cxx_std_17)

enable_testing()
include(GoogleTest)
gtest_discover_tests(unittest)
add_test(NAME run_all COMMAND unittest)

find_program(GCOVR gcovr)
if (GCOVR)
	add_custom_target(
		coverage
		COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
		COMMAND ${GCOVR} -r ${CMAKE_SOURCE_DIR} --json-pretty --json cvrg.json
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMENT "gtest + gcovr"
		DEPENDS unittest
	)
endif()
