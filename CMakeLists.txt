project(Jungle)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${ECM_MODULE_PATH})

find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Core Test Sql DBus Qml Quick Widgets)
find_package(KF5 REQUIRED COMPONENTS KDE4Support I18n Service Baloo)
find_package(TmdbQt5 REQUIRED)

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
)

add_subdirectory(src)
add_subdirectory(qml)

feature_summary(WHAT ALL
    INCLUDE_QUIET_PACKAGES
    FATAL_ON_MISSING_REQUIRED_PACKAGES
)
