add_executable(dagor_rpl_test
        TestLoadingReplay.cpp
)
include(FetchContent)
FetchContent_Declare(
        mimalloc
        GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
        GIT_TAG        v2.1.2 # use desired version
)
FetchContent_MakeAvailable(mimalloc)

target_link_libraries(dagor_rpl_test
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
        Zlib::Zlib
        # mimalloc

)

target_include_directories(dagor_rpl_test
        PUBLIC
        "${BASE_DIR}/replay/include"
        "${BASE_DIR}/src/include"
        # "${BASE_DIR}/3rdParty"
)

# SetGenFlags(dagor_rpl_test)