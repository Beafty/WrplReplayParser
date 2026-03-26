add_library(pyECS
        EntityId.cpp
        EntityManager.cpp
)

target_link_libraries(pyECS
        pybind11::module
        Python::Python
        dagECS::dagECS
)

add_library(pyECS::pyECS ALIAS pyECS)


# Set include dirs for Python if needed
target_include_directories(pyECS PRIVATE
        "${BASE_DIR}/replay/include"
        "${BASE_DIR}/python/include"
)