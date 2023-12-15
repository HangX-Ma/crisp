set(CRISP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/crisp")

file(GLOB CRISP_APP_HEADERS "${CRISP_DIR}/app/*.h")
file(GLOB CRISP_CORE_HEADERS "${CRISP_DIR}/core/*.h")
file(GLOB CRISP_TINYFSM_HEADERS "${CRISP_DIR}/tinyfsm/*.h")

set(CRISP_ALL_HEADERS ${CRISP_APP_HEADERS} ${CRISP_CORE_HEADERS} ${CRISP_TINYFSM_HEADERS})

source_group("Header Files\\crisp\\app" FILES ${CRISP_APP_HEADERS})
source_group("Header Files\\crisp\\core" FILES ${CRISP_CORE_HEADERS})
source_group("Header Files\\crisp\\tinyfsm" FILES ${CRISP_TINYFSM_HEADERS})