# Global config ---------------------------------------------------------------
cmake_minimum_required (VERSION 3.5)
project (ca821x-api)

# Set configuration variables -------------------------------------------------
set(CASCODA_CA_VERS 8210 8211)
set(CASCODA_CA_VER 8211 CACHE STRING "The CAxxxx chip number of the target part")
set_property(CACHE CASCODA_CA_VER PROPERTY STRINGS ${CASCODA_CA_VERS})

if(NOT CASCODA_CA_VER IN_LIST CASCODA_CA_VERS)
    message(FATAL_ERROR "CASCODA_CA_VER must be one of ${CASCODA_CA_VERS}")
endif()

# Header file setup -----------------------------------------------------------
include_directories(
	${PROJECT_SOURCE_DIR}/include
	)

configure_file(
	"${PROJECT_SOURCE_DIR}/include/ca821x_config.h.in"
	"${PROJECT_SOURCE_DIR}/include/ca821x_config.h"
	)

# Main library config ---------------------------------------------------------
file(GLOB MAIN_SOURCES
	${PROJECT_SOURCE_DIR}/source/*)

add_library(ca821x
	${PROJECT_SOURCE_DIR}/source/ca821x_api.c
	)

# Test app config -------------------------------------------------------------
add_executable(test_app
	${PROJECT_SOURCE_DIR}/test/test.c
	)

target_link_libraries(test_app ca821x)

# Run tests -------------------------------------------------------------------
include(CTest)
add_test(TestAppPasses test_app)
