FetchContent_Declare(
        ZLIB
        GIT_REPOSITORY https://github.com/zlib-ng/zlib-ng.git
        GIT_TAG 2.2.4
        GIT_SHALLOW TRUE
        OVERRIDE_FIND_PACKAGE
)

FetchContent_GetProperties(ZLIB)
if (NOT zlib_POPULATED)
    # zlib-ng specific options
    set(ZLIB_COMPAT ON CACHE BOOL "" FORCE)  # makes it a drop-in zlib replacement
    set(ZLIB_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
    set(ZLIBNG_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(ZLIB_INSTALL ON CACHE BOOL "" FORCE)  # avoids the export issues we hit earlier
    FetchContent_MakeAvailable(ZLIB)
endif ()

if (NOT TARGET ZLIB::ZLIB)
    add_library(ZLIB::ZLIB ALIAS zlib)
endif ()
