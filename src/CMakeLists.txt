list(APPEND features cxx_std_14 )
list(APPEND compile_opt "-Winline")

######################
# field_ref_iterator #
######################
list(APPEND fri_headers "./field_ref_iterator.h")

message("Target: field_ref_iterator")
add_library( field_ref_iterator INTERFACE )
target_sources( field_ref_iterator INTERFACE ${field_ref_iterator_headers} )
target_compile_options(field_ref_iterator INTERFACE ${compile_opt} )
target_compile_features(field_ref_iterator INTERFACE ${features} )
target_include_directories(field_ref_iterator INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

message("Target: field_ref_iterator [done]")
############
# PathPool #
############
list(APPEND pp_headers "path_pool.h")

list(APPEND pp_deps field_ref_iterator)

message("Target: path_pool")
add_library( path_pool INTERFACE )
target_sources( path_pool INTERFACE ${pp_headers} )
target_compile_options(path_pool INTERFACE ${compile_opt} )
target_compile_features(path_pool INTERFACE ${features} )
target_include_directories(path_pool INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(path_pool INTERFACE ${pp_deps})

message("Target: path_pool [done] \t <- ${pp_deps}")
