find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/codegen.stamp
        COMMAND Python3::Interpreter ${CMAKE_CURRENT_SOURCE_DIR}/generate.py
        "${BASE_DIR}/replay/include/mpi/codegen"
        "${BASE_DIR}/replay/mpi/codegen"
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/codegen.stamp
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/builtin_types.py
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_types.py
        ${CMAKE_CURRENT_SOURCE_DIR}/objects/ReflectableObjects.py
        ${CMAKE_CURRENT_SOURCE_DIR}/objects/obj_base.py
        ${CMAKE_CURRENT_SOURCE_DIR}/generate.py
        ${CMAKE_CURRENT_SOURCE_DIR}/parser.py
        ${CMAKE_CURRENT_SOURCE_DIR}/write_header.py
        COMMENT "Generating code before build"
)

add_custom_target(CodeGeneration ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/codegen.stamp
)