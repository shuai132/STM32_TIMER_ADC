set(TARGET_NAME app)

add_library(${TARGET_NAME} app.c md_adc.c)
target_include_directories(${TARGET_NAME} PUBLIC .)

add_subdirectory(lib)
target_link_libraries(${TARGET_NAME} test)
