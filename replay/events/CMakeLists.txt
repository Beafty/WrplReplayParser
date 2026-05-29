add_library(WTEvents
        STATIC
        BaseEntityES.gen.es.cpp
        UpdateMPlayerES.gen.es.cpp
        ecsPull.cpp
)


target_link_libraries(WTEvents
        dagECS::dagECS
        FieldSerializer::FieldSerializer
        BLK::BLK
        FS::FS
        VROMFS::VROMFS
        XXH3
        NET::NET
        EASTL::EASTL
        LOGGER::LOGGER

)
add_library(WTEvents::WTEvents ALIAS WTEvents)

set(INCLUDES
        "${BASE_DIR}/replay/include"
        "${BASE_DIR}/src/include"
)

target_include_directories(WTEvents
        PUBLIC
        ${INCLUDES}
)


SetGenFlags(WTEvents)