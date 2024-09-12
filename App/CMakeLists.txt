set(TARGET_NAME app)

add_library(${TARGET_NAME} app.c md_adc.c)
target_include_directories(${TARGET_NAME} PUBLIC .)

add_subdirectory(lib)

# link one lib: test_a or test_b
target_link_libraries(${TARGET_NAME} test_a)
#target_link_libraries(${TARGET_NAME} test_b)
