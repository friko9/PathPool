message(STATUS "Target: path_ex1")
add_executable(path_ex1 ex1.cpp)
target_link_libraries(path_ex1 path)
message(STATUS "Target: path_ex1 [done]")
