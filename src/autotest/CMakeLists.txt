ecm_add_test(guessitjobtest.cpp ../guessitjob.cpp ../job.cpp
    TEST_NAME "guessitjobtest"
    LINK_LIBRARIES Qt5::Test
)

ecm_add_test(thumbnailcreationjobtest.cpp ../thumbnailcreationjob.cpp
    TEST_NAME "thumbnailcreationjobtest"
    LINK_LIBRARIES Qt5::Test KF5::KIOWidgets
)

ecm_add_test(asyncjobconsumertest.cpp ../job.cpp ../asyncjobconsumer.cpp
    TEST_NAME "asyncjobconsumertest"
    LINK_LIBRARIES Qt5::Test
)

ecm_add_test(networkimagefetchjobtest.cpp ../job.cpp ../networkimagefetchjob.cpp
    TEST_NAME "networkimagefetchjobtest"
    LINK_LIBRARIES Qt5::Test Qt5::Network
)

ecm_add_test(moviefetchjobtest.cpp ../job.cpp ../moviefetchjob.cpp ../themoviedbstore.cpp ../tvshowfetchjob.cpp ../tvseasonfetchjob.cpp
    TEST_NAME "moviefetchjobtest"
    LINK_LIBRARIES Qt5::Test TmdbQt::TmdbQt5
)

ecm_add_test(tvshowfetchjobtest.cpp ../job.cpp ../moviefetchjob.cpp ../themoviedbstore.cpp ../tvshowfetchjob.cpp ../tvseasonfetchjob.cpp
    TEST_NAME "tvshowfetchjobtest"
    LINK_LIBRARIES Qt5::Test TmdbQt::TmdbQt5
)

ecm_add_test(tvseasonfetchjobtest.cpp ../job.cpp ../moviefetchjob.cpp ../themoviedbstore.cpp ../tvshowfetchjob.cpp ../tvseasonfetchjob.cpp
    TEST_NAME "tvseasonfetchjobtest"
    LINK_LIBRARIES Qt5::Test TmdbQt::TmdbQt5
)

ecm_add_test(queuetest.cpp ../queue.cpp
    TEST_NAME "queuetest"
    LINK_LIBRARIES Qt5::Test kvariantstore
)
