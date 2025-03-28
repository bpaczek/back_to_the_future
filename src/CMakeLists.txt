add_executable(BTTF
    main.cpp
    archiver.cpp
    explorer.cpp
    logs.cpp
)

set_target_properties(BTTF PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

set(CMAKE_CXX_FLAGS_DEBUG "-g")

FIND_PATH(archive_INCLUDE_DIR archive.h /usr/local/include)
FIND_LIBRARY(archive_LIB libarchive.a /usr/local/lib)
if(archive_INCLUDE_DIR AND archive_LIB)
  message(STATUS "Found libarchive")
  include_directories(${archive_INCLUDE_DIR})
  target_link_libraries(BTTF
      PUBLIC 
      ${archive_LIB} 
      z 
      bz2 
      lzma 
      iconv 
      xml2 
      crypto 
      ssl 
      nettle 
      acl 
      lz4 
      zstd)
else()
  message(FATAL_ERROR "libarchive not found")
endif()
