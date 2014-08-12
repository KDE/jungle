# set(LIB_SRCS
#     database.cpp
#     movie.cpp
#     tvshow.cpp
#     tvseason.cpp
#     tvepisode.cpp
# )
#
# add_library(junglecommon SHARED ${LIB_SRCS})
# target_link_libraries(junglecommon
#     Qt5::Core
#     Qt5::Sql
# )
#
# generate_export_header(junglecommon BASE_NAME JUNGLE EXPORT_FILE_NAME jungle_export.h)
#
# install(TARGETS junglecommon EXPORT JungleLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
#
#
# #
# # Executable
# #
# set(SRCS
#     main.cpp
#     feeder.cpp
#     themoviedbstore.cpp
#     moviefetchjob.cpp
#     tvshowfetchjob.cpp
#     tvseasonfetchjob.cpp
# )
#
# add_executable(jungle ${SRCS})
# target_link_libraries(jungle
#     Qt5::Core
#     Qt5::Qml
#     Qt5::Gui
#     Qt5::Quick
#     Qt5::Sql
#     Qt5::DBus
#     Qt5::Widgets
#     KF5::BalooCore
#     KF5::BalooFiles
#     TmdbQt::TmdbQt5
#     junglecommon
# )
#
# install(TARGETS jungle DESTINATION ${BIN_INSTALL_DIR})
#
# #
# # QML Plugin
# #
# set(SRC
#     qmlplugins.cpp
#     videosmodel.cpp
#     moviesmodel.cpp
#     tvshowsmodel.cpp
#     tvepisodemodel.cpp
# )
#
# add_library(jungleqmlplugin SHARED ${SRC})
#
# target_link_libraries(jungleqmlplugin
#     Qt5::Core
#     Qt5::Qml
#     Qt5::Sql
#     KF5::BalooCore
#     KF5::BalooFiles
#     junglecommon
# )
#
# install(TARGETS jungleqmlplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/jungle)
# install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/jungle)

add_subdirectory(autotest)
