#####################
# perftest/includes #
#####################
find_package(Boost REQUIRED filesystem)

set(name perftest_includes)

list(APPEND depends Boost::filesystem)

list(APPEND files "./BoostPath.h")

message("Target: ${name}")
add_library( ${name} INTERFACE )
target_link_libraries( ${name} INTERFACE ${depends})

message("Target: ${name} [done] \t <- ${depends}")
