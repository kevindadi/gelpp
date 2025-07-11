cmake_minimum_required (VERSION 3.5)
project (GEL++)
set(GEL_MAJOR	2)
set(GEL_MINOR	0)
enable_testing()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Wall" )

# OS dependencies
if(NOT WIN32)
	find_library(DL dl)
	set(LIBDIR "lib")
else(NOT WIN32)
	set(LIBDIR "bin")
endif(NOT WIN32)
set(ORIGIN $ORIGIN)
set(BINDIR "bin")

# check endianness
include(TestBigEndian)
TEST_BIG_ENDIAN(BIG_ENDIAN)
if(BIG_ENDIAN)
	add_compile_definitions(ENDIANNESS_BIG=1)
endif(BIG_ENDIAN)


# C++ 11 support
if(CMAKE_VERSION LESS "3.1")
	add_compile_options(--std=c++11)
	message(STATUS "C++11 set using cflags")
else()
	set(CMAKE_CXX_STANDARD 11)
	message(STATUS "C++ set using CMAKE_CXX_STANDARD")
endif()

# installation level
set(INSTALL_TYPE "all" CACHE STRING "Type of installation (one of all, dev, bin, int).")
if(INSTALL_TYPE MATCHES "dev")
    set(INSTALL_INC "yes")
    set(INSTALL_DOC "yes")
elseif(INSTALL_TYPE MATCHES "bin")
    set(INSTALL_BIN "yes")
elseif(INSTALL_TYPE MATCHES "int")
    set(INSTALL_BIN "yes")
    set(INSTALL_INC "yes")
else()
	set(INSTALL_INC "yes")
    set(INSTALL_BIN "yes")
    set(INSTALL_DOC "yes")
	if(NOT INSTALL_TYPE MATCHES "all")
    	message(ERROR ": unknown INSTALL_TYPE: ${INSTALL_TYPE}. Falling back to all.")
    endif()
endif()


# looking for elm
if(EXISTS "../elm")
	set(ELM_PATH "../elm/src/")
endif()
find_library(ELM_LIB elm HINTS "${ELM_PATH}" DOC "ELM library")
if(ELM_LIB)
	message(STATUS "elm found at ${ELM_LIB}")
else()
	message(FATAL_ERROR "elm library required")
endif()
get_filename_component(ELM_DIR "${ELM_LIB}" DIRECTORY)
set(ELM_INC "${ELM_DIR}/../include")
find_file(ELM_HEADERS "elm/io.h" "${ELM_INC}")
if(ELM_HEADERS)
	message(STATUS "elm headers at ${ELM_HEADERS}")
	include_directories("${ELM_INC}")
else()
	message(FATAL_ERROR "cannot find elm headers")
endif()

# look for COFFI
set(COFFI_PATH "${CMAKE_SOURCE_DIR}/coffi/" CACHE PATH "Path to COFFI")
if(EXISTS "${COFFI_PATH}/coffi/coffi.hpp")
	set(HAS_COFFI 1)
	include_directories("${COFFI_PATH}")
	message(STATUS "COFFI found at ${COFFI_PATH}.")
else()
	set(HAS_COFFI 0)
	message(STATUS "COFFI not found at ${COFFI_PATH}.")
endif()

# compilation configuration
include_directories("${CMAKE_SOURCE_DIR}/include" ${CMAKE_BINARY_DIR})

# build configuration file
configure_file (
  "${PROJECT_SOURCE_DIR}/config.in"
  "${PROJECT_BINARY_DIR}/config.h"
)

# components
add_subdirectory(src)
add_subdirectory(bin)

# installation
install(FILES "README.md" "COPYING.md" "AUTHORS" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/GEL++/")
install(FILES "include/gel++.h" DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
install(DIRECTORY "include/gel++" DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
