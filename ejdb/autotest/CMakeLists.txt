ecm_add_test(databasetest.cpp ../jsondatabase.cpp

    TEST_NAME "databaseTest"
    LINK_LIBRARIES Qt5::Test ${EJDB_LIBRARIES}
)
