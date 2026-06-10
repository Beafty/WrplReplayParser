
set(ZLIB_FOUND TRUE CACHE BOOL "ZLIB found" FORCE)
set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/arc/zlib-ng" CACHE STRING "ZLIB include dir")
set(ZLIB_LIBRARY Zlib CACHE STRING "ZLIB library" FORCE)

add_subdirectory(zstd-1.4.5)
add_subdirectory(zlib-ng)
add_subdirectory(lz4)
add_subdirectory(lzma-9.20)