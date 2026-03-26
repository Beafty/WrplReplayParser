
# Find the specified Python version, or use any found by default
if(PYTHON_VERSION)
    find_package(Python ${PYTHON_VERSION} REQUIRED COMPONENTS Interpreter Development)
else()
    find_package(Python 3.12...3.12.9 REQUIRED COMPONENTS Interpreter Development)
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
            GIT_TAG        v2.11.1  # You can update the version/tag as needed
    )
    FetchContent_MakeAvailable(pybind11)
endif()

# Now set up your module
pybind11_add_module(PyReplayParser
        # ecs.cpp
        main.cpp
)  # <-- Replace src/main.cpp with your actual source file

# Link the right Python version
target_link_libraries(PyReplayParser PRIVATE
        pybind11::module
        Python::Python
        PyReplay::PyReplay
        PyState::PyState
)

# Set include dirs for Python if needed
target_include_directories(PyReplayParser PRIVATE
        "${BASE_DIR}/replay/include"
        "${BASE_DIR}/python/include"
)
attach_signal_tracer(PyReplayParser)

# target_link_options(PyReplayParser PRIVATE
#         -static-libgcc
#         -static-libstdc++
#         -static
# )