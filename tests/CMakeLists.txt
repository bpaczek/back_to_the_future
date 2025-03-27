add_executable(test_explorer test_explorer.cpp)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

include_directories(${gmock_SOURCE_DIR}/include)

target_sources(test_explorer PRIVATE ${CMAKE_SOURCE_DIR}/src/explorer.cpp)

target_link_libraries(test_explorer gtest gtest_main)
