set(DATABASE_SRC databasetest.cpp)

ecm_add_test(${DATABASE_SRC}
    TEST_NAME "databasetest"
    LINK_LIBRARIES Qt5::Test junglecommon
)
