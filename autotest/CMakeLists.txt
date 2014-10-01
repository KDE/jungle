# set(DATABASE_SRC databasetest.cpp)
#
# ecm_add_test(${DATABASE_SRC}
#     TEST_NAME "databasetest"
#     LINK_LIBRARIES Qt5::Test junglecommon
# )
#
# add_definitions(-DWEB_MOCK_DATA="${CMAKE_CURRENT_SOURCE_DIR}/webmockdata.sql")
# add_definitions(-DFILE_MAP_MOCK_DATA="${CMAKE_CURRENT_SOURCE_DIR}/filemapmockdata.sql")
# set(TVEPISODEMODEL_SRC tvepisodemodeltest.cpp modeltest.cpp ../tvepisodemodel.cpp)
#
# ecm_add_test(${TVEPISODEMODEL_SRC}
#     TEST_NAME "tvepisodemodeltest"
#     LINK_LIBRARIES Qt5::Test junglecommon
# )

ecm_add_test(jungleapptest.cpp
    ../src/jungleapp.cpp
    ../src/jungleinitiator.cpp
    ../src/jungleconfig.cpp

    TEST_NAME "jungleAppTest"
    LINK_LIBRARIES ${GMOCK_LIBRARIES} ${GTEST_LIBRARIES} Qt5::Test pthread KF5::ConfigCore
)

ecm_add_test(jungleconfigtest.cpp
    ../src/jungleconfig.cpp

    TEST_NAME "JungleConfigTest"
    LINK_LIBRARIES ${GMOCK_LIBRARIES} ${GTEST_LIBRARIES} Qt5::Test pthread KF5::ConfigCore
)

ecm_add_test(jungleinitiatortest.cpp
    ../src/jungleinitiator.cpp
    ../src/jungleconfig.cpp

    TEST_NAME "jungleInitiatorTest"
    LINK_LIBRARIES ${GMOCK_LIBRARIES} ${GTEST_LIBRARIES} Qt5::Test pthread KF5::ConfigCore
)

ecm_add_test(junglefirstruntest.cpp
    ../src/junglefirstrun.cpp
    ../src/jungleconfig.cpp

    TEST_NAME "jungleFirstRunTest"
    LINK_LIBRARIES ${GMOCK_LIBRARIES} ${GTEST_LIBRARIES} Qt5::Test pthread KF5::ConfigCore
)

ecm_add_test(junglenormalruntest.cpp
    ../src/junglenormalrun.cpp
    ../src/jungleconfig.cpp
    ../src/interfaces/IInitGui.cpp

    TEST_NAME "jungleNormalRunTest"
    LINK_LIBRARIES ${GMOCK_LIBRARIES} ${GTEST_LIBRARIES} Qt5::Test pthread KF5::ConfigCore
)

ecm_add_test(guessitjobtest.cpp
    ../src/guessitjob.cpp

    TEST_NAME "guessItJobTest"
    LINK_LIBRARIES ${GMOCK_LIBRARIES} ${GTEST_LIBRARIES} Qt5::Test pthread
)

ecm_add_test(tvdbjobtest.cpp
    ../src/tvdbjob.cpp

    TEST_NAME "tvDbJobTest"
    LINK_LIBRARIES ${GMOCK_LIBRARIES} ${GTEST_LIBRARIES} Qt5::Test pthread
                   TmdbQt::TmdbQt5
)

ecm_add_test(databasetest.cpp
    ../src/database.cpp

    TEST_NAME "databaseTest"
    LINK_LIBRARIES ${GMOCK_LIBRARIES} ${GTEST_LIBRARIES} Qt5::Test pthread
                   ${EJDB_LIBRARIES}
)
