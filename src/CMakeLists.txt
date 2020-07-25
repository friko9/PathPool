list(APPEND features cxx_std_14 )
list(APPEND compile_opt "-Winline")

############
# PathPool #
############
list(APPEND pp_headers "impl/custom_iterators.h")
list(APPEND pp_headers "path_pool.h")
list(APPEND pp_headers "impl/impl_list_path_pool.h")
list(APPEND pp_headers "impl/impl_hash_path_pool.h")
list(APPEND pp_headers "impl/impl_path_pool_utils.h")

message("Target: path_pool")
add_library( path_pool INTERFACE )
target_sources( path_pool INTERFACE ${pp_headers} )
target_compile_options(path_pool INTERFACE ${compile_opt} )
target_compile_features(path_pool INTERFACE ${features} )
target_include_directories(path_pool INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(path_pool INTERFACE ${pp_deps})

message("Target: path_pool [done] \t <- ${pp_deps}")

########
# Path #
########
list(APPEND path_headers "path.h")

list(APPEND path_deps path_pool)

message("Target: path")
add_library( path INTERFACE )
target_sources( path INTERFACE ${path_headers} )
target_compile_options(path INTERFACE ${compile_opt} )
target_compile_features(path INTERFACE ${features} )
target_include_directories(path INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(path INTERFACE ${path_deps})

message("Target: path [done] \t <- ${path_deps}")
