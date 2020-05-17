list(APPEND features cxx_std_14 )
list(APPEND compile_opt "-Winline")

############
# PathPool #
############
list(APPEND pp_headers "path_pool.h")
#list(APPEND pp_headers "path.h")


list(APPEND pp_deps Boost::boost)

message("Target: path_pool")
add_library( path_pool INTERFACE )
target_sources( path_pool INTERFACE ${pp_headers} )
target_compile_options(path_pool INTERFACE ${compile_opt} )
target_compile_features(path_pool INTERFACE ${features} )
target_include_directories(path_pool INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(path_pool INTERFACE ${pp_deps})

message("Target: path_tool \t <- ${pp_deps}")



list(APPEND dummy_files "main.cpp")
list(APPEND dummy_deps path_pool)
#list(APPEND dummy_compile_opt "-pthread")
list(APPEND dummy_deps "-pthread")
# list(APPEND dummy_compile_opt "-D_REENTRANT")
list(APPEND dummy_compile_opt "-Wall")

message("Target: dummy")
add_executable( dummy  )
target_sources( dummy PRIVATE ${dummy_files} )
target_compile_options(dummy PRIVATE ${dummy_compile_opt} )
target_compile_features(dummy PRIVATE ${features} )
target_include_directories(dummy PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features( dummy PRIVATE cxx_std_14 )

target_link_libraries(dummy PRIVATE ${dummy_deps})

message("Target: dummy \t <- ${deps}")
