# Get crisp version from crisp/core/version.h and put it in CRISP_VERSION
function(crisp_extract_version)
    file(READ "${CMAKE_CURRENT_LIST_DIR}/crisp/core/version.h" file_contents)
    string(REGEX MATCH "CRISP_VER_MAJOR ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract major version number from crisp/version.h")
    endif()
    set(ver_major ${CMAKE_MATCH_1})

    string(REGEX MATCH "CRISP_VER_MINOR ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract minor version number from crisp/version.h")
    endif()

    set(ver_minor ${CMAKE_MATCH_1})
    string(REGEX MATCH "CRISP_VER_PATCH ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract patch version number from crisp/version.h")
    endif()
    set(ver_patch ${CMAKE_MATCH_1})

    # With PARENT_SCOPE option you change value of variable in the parent scope, but a variable in the current scope is unchanged.
    set(CRISP_VERSION_MAJOR ${ver_major} PARENT_SCOPE)
    set(CRISP_VERSION_MINOR ${ver_minor} PARENT_SCOPE)
    set(CRISP_VERSION_PATCH ${ver_patch} PARENT_SCOPE)
    set(CRISP_VERSION "${ver_major}.${ver_minor}.${ver_patch}" PARENT_SCOPE)
endfunction()

# Turn on warnings on the given target
function(crisp_enable_warnings target_name)
    if(CRISP_BUILD_WARNINGS)
        target_compile_options(
            ${target_name}
            PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
                    -Wall
                    -Wextra
                    # -Wconversion
                    -pedantic
                    -Werror
                    -Wfatal-errors>)
    endif()
endfunction()

# Enable address sanitizer (gcc/clang only)
function(crisp_enable_sanitizer target_name)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "Sanitizer supported only for gcc/clang")
    endif()
    message(STATUS "Address sanitizer enabled")
    target_compile_options(${target_name} PRIVATE -fsanitize=address,undefined)
    target_compile_options(${target_name} PRIVATE -fno-sanitize=signed-integer-overflow)
    target_compile_options(${target_name} PRIVATE -fno-sanitize-recover=all)
    target_compile_options(${target_name} PRIVATE -fno-omit-frame-pointer)
    target_link_libraries(${target_name} PRIVATE -fsanitize=address,undefined)
endfunction()