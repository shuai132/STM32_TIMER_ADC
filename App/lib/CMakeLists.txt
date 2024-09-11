set(TARGET_NAME test)

add_library(${TARGET_NAME} test.c)
target_include_directories(${TARGET_NAME} PUBLIC .)
