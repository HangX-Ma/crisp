add_subdirectory(library)
add_subdirectory(crisp)

if(CRISP_BUILD_TESTS)
  message(STATUS "Generating tests")
  enable_testing()
  add_subdirectory(tests)
endif()
