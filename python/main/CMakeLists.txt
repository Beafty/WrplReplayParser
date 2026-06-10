
# Find the specified Python version, or use any found by default
if(PYTHON_VERSION)
    find_package(Python ${PYTHON_VERSION} REQUIRED COMPONENTS Interpreter Development)
else()
    find_package(Python 3.12...3.12.13 REQUIRED COMPONENTS Interpreter Development)
endif()

# Try to find pybind11 via find_package first
find_package(pybind11 QUIET)

# If not found, fetch pybind11 automatically
if(NOT pybind11_FOUND)
    message(STATUS "pybind11 not found, fetching via FetchContent")
    include(FetchContent)
    FetchContent_Declare(
            pybind11
            GIT_REPOSITORY https://github.com/pybind/pybind11.git
            GIT_TAG v2.11.1
    )
    FetchContent_MakeAvailable(pybind11)
endif()

# Now set up your module
pybind11_add_module(PyReplayParser
        # ecs.cpp
        main.cpp
)

target_link_libraries(PyReplayParser PRIVATE
        pybind11::module
        Python::Python
        PyReplay::PyReplay
        PyState::PyState
        PyECS::PyECS
)

target_include_directories(PyReplayParser PRIVATE
        "${BASE_DIR}/replay/include"
        "${BASE_DIR}/python/include"
)
attach_signal_tracer(PyReplayParser)
target_compile_options(PyReplayParser PRIVATE -fPIC)
target_link_options(PyReplayParser PRIVATE
        -static-libgcc
        -static-libstdc++
        # -static
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_custom_command(TARGET PyReplayParser POST_BUILD
            COMMAND ${CMAKE_STRIP} --strip-debug $<TARGET_FILE:PyReplayParser>
    )
endif ()