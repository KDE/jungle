add_subdirectory(autotest)

set(JSON_LIB_SRCS
    jsondatabase.cpp
    jsoncollection.cpp
)

add_library(kjsondatabase SHARED ${JSON_LIB_SRCS})
target_link_libraries(kjsondatabase
    Qt5::Core
    Qt5::Sql
    ${EJDB_LIBRARIES}
)

generate_export_header(kjsondatabase BASE_NAME JUNGLE EXPORT_FILE_NAME kjsondatabase_export.h)
install(TARGETS kjsondatabase EXPORT JungleLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
