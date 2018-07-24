# Global config
cmake_minimum_required (VERSION 3.0)
project (ca821x-api)

get_filename_component(TOPLEVEL "./" ABSOLUTE)

include_directories(
	.
	${TOPLEVEL}/include)

# Main library config
file(GLOB MAIN_SOURCES
	${TOPLEVEL}/source/*)

add_library(ca821x ${MAIN_SOURCES})
if (CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(ca821x PRIVATE "-Wall")
endif()

# Test app config
file(GLOB TEST_SOURCES
	${TOPLEVEL}/test/*.c)

add_executable(test_app ${TEST_SOURCES})
if (CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(test_app PRIVATE "-Wall")
endif()
target_link_libraries(test_app ca821x)