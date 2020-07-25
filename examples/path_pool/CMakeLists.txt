message(STATUS "Target: pathpool_ex1")
add_executable(pathpool_ex1 ex1.cpp)
target_link_libraries(pathpool_ex1 path_pool)
message(STATUS "Target: pathpool_ex1 [done]")
