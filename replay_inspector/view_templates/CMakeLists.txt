# Your application
add_executable(TemplateViewer viewer.cpp)
target_link_libraries(TemplateViewer PRIVATE
        imgui
        SDL2::SDL2main
        LOGGER::LOGGER
        fmt::fmt
        BS::BS
        BLK::BLK
        READER::READER
        FS::FS
        VROMFS::VROMFS
        FieldSerializer:FieldSerializer
        NET::NET
        dagECS::dagECS
        INIT::INIT
        Replay::Replay
)

if(WIN32)
    # Copy SDL2.dll to the output directory after build
    add_custom_command(TARGET TemplateViewer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2>
            $<TARGET_FILE_DIR:TemplateViewer>
    )
endif ()