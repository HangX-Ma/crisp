cmake_minimum_required(VERSION 3.14)
project(crisp_utests CXX)

include(../cmake/utils.cmake)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

if (GTest_FOUND)
    message(STATUS "Packaged version of GTest will be used.")
else()
    message(STATUS "Bundled version of GTest will be downloaded and used.")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
    add_library(GTest::GTest INTERFACE IMPORTED)
    target_link_libraries(GTest::GTest INTERFACE gtest_main)
endif()

# compile all tests
enable_testing()

aux_source_directory(. crisp_utest_files)

if(CRISP_BUILD_TESTS)
    message(STATUS "Build crisp test unit")
    foreach(utest_file_name ${crisp_utest_files})
        get_filename_component(test_target ${utest_file_name} NAME_WE)
        add_executable(${test_target} ${utest_file_name})
        crisp_enable_warnings(${test_target})
        if(CRISP_SANITIZE_ADDRESS)
            crisp_enable_sanitizer(${test_target})
        endif()
        target_link_libraries(${test_target}
            PRIVATE
                crisp
                ${CMAKE_THREAD_LIBS_INIT}
                GTest::GTest
                GTest::Main
        )
        gtest_discover_tests(${test_target})
        add_test(NAME ${test_target} COMMAND ${test_target})
    endforeach()
    set_tests_properties(${test_target} PROPERTIES RUN_SERIAL ON)
endif()