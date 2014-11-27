set(LIB_SRCS
    database.cpp
)

add_library(junglecommon SHARED ${LIB_SRCS})
target_link_libraries(junglecommon
    Qt5::Core
    KVariantStore
)

generate_export_header(junglecommon BASE_NAME JUNGLE EXPORT_FILE_NAME jungle_export.h)

install(TARGETS junglecommon EXPORT JungleLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})


#
# Executable
#
set(SRCS
    main.cpp
    baloovideosfetcher.cpp
    filesystemtracker.cpp

    themoviedbstore.cpp
    moviefetchjob.cpp
    tvshowfetchjob.cpp
    tvseasonfetchjob.cpp

    job.cpp
    queue.cpp
    asyncjobconsumer.cpp
    networkconsumer.cpp
    themoviedbconsumer.cpp
    guessitjob.cpp
    databaseconsumer.cpp
    tvshowgenerationconsumer.cpp
    processor.cpp
    jungleconfig.cpp

    networkimagefetchjob.cpp
)

add_executable(jungle ${SRCS})
target_link_libraries(jungle
    Qt5::Core
    Qt5::Qml
    Qt5::Gui
    Qt5::Quick
    Qt5::DBus
    Qt5::Widgets
    KF5::Baloo
    KF5::DBusAddons
    KF5::ConfigCore
    KF5::I18n
    TmdbQt::TmdbQt5
    KVariantStore
    junglecommon
)

install(TARGETS jungle DESTINATION ${BIN_INSTALL_DIR})

#
# QML Plugin
#
set(SRC
    qmlplugins.cpp
    cursor.cpp
    moviesmodel.cpp
    tvshowsmodel.cpp
    tvepisodemodel.cpp
    videosmodel.cpp
    sortmodel.cpp
)

add_library(jungleqmlplugin SHARED ${SRC})

target_link_libraries(jungleqmlplugin
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
    junglecommon
)

install(TARGETS jungleqmlplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/jungle)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/jungle)
install(FILES jungle.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

add_subdirectory(autotest)
