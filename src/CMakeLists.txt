set(SRCS
    main.cpp
    database.cpp
    feeder.cpp
    movie.cpp
    moviefetchjob.cpp
)

add_executable(jungle ${SRCS})
target_link_libraries(jungle
    Qt5::Core
    Qt5::Qml
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
    database.cpp
    movie.cpp
)

add_library(jungleqmlplugin SHARED ${SRC})

target_link_libraries(jungleqmlplugin
    Qt5::Core
    Qt5::Qml
    ${BALOO_LIBRARIES}
)

install(TARGETS jungleqmlplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/jungle)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/jungle)
