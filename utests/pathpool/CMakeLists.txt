################
# Google Tests #
# pathpool     #
################
list(APPEND depends path_pool)
list(APPEND depends utest_includes)

list(APPEND files "testobj.h")

list(APPEND files_short "empty_short.cpp")
list(APPEND files_short ${files})

list(APPEND files_long "vergecase_long.cpp")
list(APPEND files_long "crosscomp_long.cpp")
list(APPEND files_long ${files})

add_google_test(FNAME "pathpool" LABEL "interface_short" DEPENDS ${depends} FILES ${files_short} )
add_google_test(FNAME "pathpool" LABEL "interface_long" DEPENDS ${depends} FILES ${files_long} )
