add_subdirectory(autotest)

set(KVARIANTSTORE_LIB_SRCS
    kvariantstore.cpp
    kvariantcollection.cpp
    kvariantquery.cpp
)

add_library(kvariantstore SHARED ${KVARIANTSTORE_LIB_SRCS})
target_link_libraries(kvariantstore
    Qt5::Core
    ${EJDB_LIBRARIES}
)

generate_export_header(kvariantstore BASE_NAME KVARIANTSTORE EXPORT_FILE_NAME kvariantstore_export.h)
install(TARGETS kvariantstore EXPORT JungleLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
