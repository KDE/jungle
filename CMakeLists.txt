project(Jungle)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${ECM_MODULE_PATH})

find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Core Widgets Xml Test Sql)
find_package(KF5 REQUIRED NO_MODULE COMPONENTS KDE4Support I18n Service)
find_package(KFileMetaData REQUIRED)

include(ECMSetupVersion)
include(ECMAddTests)

include(GenerateExportHeader)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

include_directories(
  ${CMAKE_SOURCE_DIR}
)

add_subdirectory(src)

feature_summary(WHAT ALL
    INCLUDE_QUIET_PACKAGES
    FATAL_ON_MISSING_REQUIRED_PACKAGES
)
