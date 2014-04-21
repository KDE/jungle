set(SRCS
    main.cpp
    database.cpp
    feeder.cpp
    movie.cpp
    themoviedbstore.cpp
    moviefetchjob.cpp
    show.cpp
    tvseason.cpp
    tvepisode.cpp
    tvshowfetchjob.cpp
)

add_executable(jungle ${SRCS})
target_link_libraries(jungle
    Qt5::Core
    Qt5::Qml
    Qt5::Gui
    Qt5::Sql
    ${BALOO_LIBRARIES}
    TmdbQt::TmdbQt5
)

install(TARGETS jungle DESTINATION ${BIN_INSTALL_DIR})

#
# QML Plugin
#
set(SRC
    qmlplugins.cpp
    videosmodel.cpp
    moviesmodel.cpp
    tvshowsmodel.cpp
    database.cpp
    movie.cpp
    show.cpp
)

add_library(jungleqmlplugin SHARED ${SRC})

target_link_libraries(jungleqmlplugin
    Qt5::Core
    Qt5::Qml
    ${BALOO_LIBRARIES}
)

install(TARGETS jungleqmlplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/jungle)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/jungle)
