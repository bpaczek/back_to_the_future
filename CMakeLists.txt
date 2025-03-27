cmake_minimum_required(VERSION 3.10)

project(BTTF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#utests
add_subdirectory(googletest)
add_executable(tests test/test.cpp)
target_link_libraries(tests gtest gtest_main)

add_executable(BTTF main.cpp explorer.cpp archiver.cpp)

FIND_PATH(archive_INCLUDE_DIR archive.h /usr/local/include)
FIND_LIBRARY(archive_LIB libarchive.a /usr/local/lib)
if(archive_INCLUDE_DIR AND archive_LIB)
  message(STATUS "Found libarchive")
  include_directories(${archive_INCLUDE_DIR})
  # target_link_libraries(BTTF ${archive_LIB})
  target_link_libraries(BTTF PUBLIC ${archive_LIB} z bz2 lzma iconv xml2 crypto ssl nettle acl lz4 zstd)

else()
  message(FATAL_ERROR "libarchive not found")
endif()

option(DEBUG "Create binary with debug symbols" OFF)
if(DEBUG)
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()
