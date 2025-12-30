add_executable(TestNewBlk
        main.cpp
)

target_link_libraries(TestNewBlk
)


set(INCLUDES
        "${BASE_DIR}/src/include"
)

target_include_directories(TestNewBlk
        PUBLIC
        ${INCLUDES}
)
