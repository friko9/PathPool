list(APPEND features cxx_std_14 )
list(APPEND compile_opt "-Winline")

######################
# field_ref_iterator #
######################
list(APPEND fri_headers "./field_ref_iterator.h")

message("Target: field_ref_iterator")
add_library( fri INTERFACE )
target_sources( fri INTERFACE ${fri_headers} )
target_compile_options(fri INTERFACE ${compile_opt} )
target_compile_features(fri INTERFACE ${features} )
target_include_directories(fri INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

message("Target: fri [done]")
############
# PathPool #
############
list(APPEND pp_headers "path_pool.h")

list(APPEND pp_deps Boost::boost)
list(APPEND pp_deps fri)

message("Target: path_pool")
add_library( path_pool INTERFACE )
target_sources( path_pool INTERFACE ${pp_headers} )
target_compile_options(path_pool INTERFACE ${compile_opt} )
target_compile_features(path_pool INTERFACE ${features} )
target_include_directories(path_pool INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(path_pool INTERFACE ${pp_deps})

message("Target: path_tool [done] \t <- ${pp_deps}")
