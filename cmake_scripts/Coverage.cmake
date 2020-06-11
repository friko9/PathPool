if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
  set(CMAKE_BUILD_TYPE "Debug")
  include(CodeCoverage)
  SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
  SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
else()
  function (setup_target_for_coverage_gcovr_html)
  endfunction()
  function (setup_target_for_coverage_lcov)
  endfunction()
endif() #CMAKE_BUILD_TYPE STREQUAL "Coverage"
