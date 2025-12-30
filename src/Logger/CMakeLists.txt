
add_library(LOGGER STATIC logger.cpp)

set(INCLUDES
        "${BASE_DIR}/src/include"
        "${BASE_DIR}/3rdParty/fmt-12.1.0/include"
)

target_include_directories(LOGGER
        PUBLIC
        ${INCLUDES}
)

target_link_libraries(LOGGER fmt::fmt)

add_library(LOGGER::LOGGER ALIAS LOGGER)
