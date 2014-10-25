#set(DATABASE_SRC databasetest.cpp)
#
#ecm_add_test(${DATABASE_SRC}
#    TEST_NAME "databasetest"
#    LINK_LIBRARIES Qt5::Test junglecommon
#)
#
add_definitions(-DWEB_MOCK_DATA="${CMAKE_CURRENT_SOURCE_DIR}/webmockdata.sql")
add_definitions(-DFILE_MAP_MOCK_DATA="${CMAKE_CURRENT_SOURCE_DIR}/filemapmockdata.sql")
set(TVEPISODEMODEL_SRC tvepisodemodeltest.cpp modeltest.cpp ../tvepisodemodel.cpp)

#ecm_add_test(${TVEPISODEMODEL_SRC}
#    TEST_NAME "tvepisodemodeltest"
#    LINK_LIBRARIES Qt5::Test junglecommon
#)

ecm_add_test(guessitjobtest.cpp ../guessitjob.cpp
    TEST_NAME "guessitjobtest"
    LINK_LIBRARIES Qt5::Test
)
