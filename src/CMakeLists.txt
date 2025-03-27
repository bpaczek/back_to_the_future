add_executable(BTTF_src
    main.cpp
    archiver.cpp
    explorer.cpp
    logs.cpp
)

FIND_PATH(archive_INCLUDE_DIR archive.h /usr/local/include)
FIND_LIBRARY(archive_LIB libarchive.a /usr/local/lib)
if(archive_INCLUDE_DIR AND archive_LIB)
  message(STATUS "Found libarchive")
  include_directories(${archive_INCLUDE_DIR})
  target_link_libraries(BTTF_src
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