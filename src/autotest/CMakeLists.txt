ecm_add_test(guessitjobtest.cpp ../guessitjob.cpp
    TEST_NAME "guessitjobtest"
    LINK_LIBRARIES Qt5::Test
)

ecm_add_test(thumbnailcreationjobtest.cpp ../thumbnailcreationjob.cpp
    TEST_NAME "thumbnailcreationjobtest"
    LINK_LIBRARIES Qt5::Test KF5::KIOWidgets
)
