#####################
# perftest/includes #
#####################

set(name perftest_includes)

#list(APPEND files "./utest_tuple.h")

message("Target: ${name}")
add_library( ${name} INTERFACE )
#target_sources( ${name} INTERFACE ${files} )
#target_compile_features( ${name} INTERFACE ${features} )
#target_compile_options( ${name} INTERFACE ${compile_opt} )

#target_include_directories( ${name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )
#target_link_libraries(utest_includes INTERFACE ${depends})

message("Target: ${name} [done] \t <- ${depends}")
