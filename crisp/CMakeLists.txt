cmake_minimum_required(VERSION 3.14)

file(GLOB CRISP_APP_SRCS ${CMAKE_CURRENT_LIST_DIR}/app/*.cpp)
file(GLOB CRISP_CORE_SRCS ${CMAKE_CURRENT_LIST_DIR}/core/*.cpp)
file(GLOB CRISP_TINYFSM_SRCS ${CMAKE_CURRENT_LIST_DIR}/tinyfsm/*.cpp)

set(CRISP_ALL_SOURCES ${CRISP_APP_SRCS} ${CRISP_CORE_SRCS}
    ${CRISP_TINYFSM_SRCS})

if(CRISP_BUILD_SHARED OR BUILD_SHARED_LIBS)
    add_library(crisp SHARED ${CRISP_ALL_SOURCES} ${CRISP_ALL_HEADERS})
else()
    add_library(crisp STATIC ${CRISP_ALL_SOURCES} ${CRISP_ALL_HEADERS})
endif()

add_library(crisp::crisp ALIAS crisp)
target_include_directories(
    crisp PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/app
    ${CMAKE_CURRENT_LIST_DIR}/core
    ${CMAKE_CURRENT_LIST_DIR}/tinyfsm)
target_link_libraries(crisp simplekv spdlog)

crisp_enable_warnings(crisp)

set_target_properties(
    crisp PROPERTIES VERSION ${CRISP_VERSION}
    SOVERSION ${CRISP_VERSION_MAJOR}.${CRISP_VERSION_MINOR})
set_target_properties(crisp PROPERTIES DEBUG_POSTFIX d)
