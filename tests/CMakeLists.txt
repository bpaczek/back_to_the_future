include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include)

FIND_PATH(archive_INCLUDE_DIR archive.h /usr/local/include)

add_executable(test_explorer test_explorer.cpp)
target_sources(test_explorer PRIVATE ${CMAKE_SOURCE_DIR}/src/explorer.cpp)
target_link_libraries(test_explorer gtest gtest_main)

add_executable(test_archiver test_archiver.cpp)
target_sources(test_archiver PRIVATE ${CMAKE_SOURCE_DIR}/src/archiver.cpp)
target_link_libraries(test_archiver gtest gmock gtest_main)
