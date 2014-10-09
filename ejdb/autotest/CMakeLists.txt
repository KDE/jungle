ecm_add_test(databasetest.cpp ../database.cpp

    TEST_NAME "databaseTest"
    LINK_LIBRARIES Qt5::Test ${EJDB_LIBRARIES}
)
