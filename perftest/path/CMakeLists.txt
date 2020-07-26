#####################
# Google Benchmark  #
# path     	    #
#####################

list(APPEND depends path)
list(APPEND depends perftest_includes)

add_perf_test(FNAME "path_insertion" DEPENDS ${depends} FILES "insertion.cpp")
add_perf_test(FNAME "path_comparison" DEPENDS ${depends} FILES "comparison.cpp")
