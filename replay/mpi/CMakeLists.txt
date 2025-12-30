add_library(MPI
        mpi.cpp
        reflection.cpp
        ObjectDispatcher.cpp
        MPLayer.cpp
        netUtils.cpp
        serializers.cpp
)


target_link_libraries(MPI
        FieldSerializer:FieldSerializer
        BLK::BLK
        FS::FS
        VROMFS::VROMFS
        BLK::BLK
        # REGEX::REGEX
        XXH3
        NET::NET
        ZSTD::ZSTD
)
add_library(MPI::MPI ALIAS MPI)

set(INCLUDES
        "${BASE_DIR}/replay/include"
        "${BASE_DIR}/src/include"
        "${BASE_DIR}/3rdParty"
)

target_include_directories(MPI
        PUBLIC
        ${INCLUDES}
)

