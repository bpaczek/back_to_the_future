add_executable(test_archiver test.cpp)

# Linkowanie z Google Test
target_link_libraries(test_archiver gtest gtest_main pthread)