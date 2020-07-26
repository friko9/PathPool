#####################
# Google Benchmark  #
# path     	    #
#####################

list(APPEND depends path)
list(APPEND depends perftest_includes)

list(APPEND files "random_insertion.cpp")
#list(APPEND files_short ${files})

#list(APPEND files_long "vergecase_long.cpp")
#list(APPEND files_long "crosscomp_long.cpp")
#list(APPEND files_long ${files})

add_perf_test(FNAME "path" DEPENDS ${depends} FILES ${files} )
