cmake_minimum_required(VERSION 3.14)

# ---------------------------------------------------------------------------------------
# Start crisp project
# ---------------------------------------------------------------------------------------
include(cmake/utils.cmake)
include(cmake/ide.cmake)

crisp_extract_version()

project(crisp VERSION ${CRISP_VERSION} LANGUAGES CXX)
message(STATUS "Build crisp: ${CRISP_VERSION}")

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE "release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

# ---------------------------------------------------------------------------------------
# Compiler config
# ---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------------------
# Set CRISP_MASTER_PROJECT to ON if we are building spdlog
# ---------------------------------------------------------------------------------------
# Check if crisp is being used directly or via add_subdirectory, but allow overriding
if(NOT DEFINED CRISP_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(CRISP_MASTER_PROJECT ON)
        message(STATUS "Build part: master-project")
    else()
        set(CRISP_MASTER_PROJECT OFF)
        message(STATUS "Build part: sub-porject")
    endif()
endif()

# build shared option
option(CRISP_BUILD_SHARED "Build shared library" OFF)

# build position independent code
option(CRISP_BUILD_PIC "Build position independent code (-fPIC)" OFF)

# sanitizer options
option(CRISP_SANITIZE_ADDRESS "Enable address sanitizer in tests" OFF)

# warning options
option(CRISP_BUILD_WARNINGS "Enable compiler warnings" ${CRISP_MASTER_PROJECT})

# testing options
option(CRISP_BUILD_TESTS "Build tests" OFF)

if(CRISP_BUILD_PIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

include(cmake/basic.cmake)
