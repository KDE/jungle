project(Jungle)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${ECM_MODULE_PATH})

find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Core Test DBus Qml Quick Widgets)
find_package(KF5 REQUIRED COMPONENTS KDELibs4Support I18n Service Baloo)
find_package(TmdbQt5 REQUIRED)
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)
find_package(Ejdb REQUIRED)

find_package(GuessIt 0.9.3 REQUIRED) # Remove 'REQUIRED' on release
set_package_properties(GuessIt PROPERTIES URL https://github.com/wackou/guessit
                                          DESCRIPTION "GuessIt library for guessing names"
                                          TYPE RUNTIME
)

include(ECMAddTests)
include(ECMSetupVersion)
include(ECMAddTests)

include(GenerateExportHeader)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

include_directories(
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/ejdb
  ${CMAKE_BINARY_DIR}/ejdb
  ${EJDB_INCLUDE_DIRS}
)

add_subdirectory(src)
add_subdirectory(ejdb)
add_subdirectory(qml)

feature_summary(WHAT ALL
    INCLUDE_QUIET_PACKAGES
    FATAL_ON_MISSING_REQUIRED_PACKAGES
)
