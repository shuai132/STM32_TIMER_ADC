add_library(test_a test_a.c)
target_include_directories(test_a PUBLIC .)

add_library(test_b test_b.c)
target_include_directories(test_b PUBLIC .)

add_library(test_c test_c.c)
target_include_directories(test_c PUBLIC .)

target_link_libraries(test_a PRIVATE test_b)
target_link_libraries(test_b PRIVATE test_c)
target_link_libraries(test_c PRIVATE test_a)
