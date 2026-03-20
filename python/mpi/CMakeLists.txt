add_library(PyMpi
        codegen_objects.cpp
        mpi.cpp
)

target_link_libraries(PyMpi
        pybind11::module
        Python::Python
        MPI::MPI
)

add_library(PyMpi::PyMpi ALIAS PyMpi)


# Set include dirs for Python if needed
target_include_directories(PyMpi PRIVATE
        "${BASE_DIR}/replay/include"
        "${BASE_DIR}/python/include"
)