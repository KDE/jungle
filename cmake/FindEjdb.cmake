# - Try to find EJDB using pkg-config
# Once done this will define
#  EJDB_FOUND - System has EJDB
#  EJDB_INCLUDE_DIRS - The EJDB include directories
#  EJDB_LIBRARIES - The libraries needed to use EJDB
#  EJDB_DEFINITIONS - Compiler switches required for using EJDB
#  EJDB_STATIC_LIB - Determines whether or not to use static EJDB
find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_EJDB tcejdb)
set(EJDB_DEFINITIONS ${PC_EJDB_CFLAGS_OTHER})

IF(EJDB_STATIC_LIB)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
ENDIF(EJDB_STATIC_LIB)

find_path(EJDB_INCLUDE_DIR tcejdb/ejdb.h
          HINTS ${PC_EJDB_INCLUDEDIR} ${PC_EJDB_INCLUDE_DIRS})

find_library(EJDB_LIBRARY NAMES tcejdb
             HINTS ${PC_EJDB_LIBDIR} ${PC_EJDB_LIBRARY_DIRS} ${EJDB_LIBRARY_DIR})

set(EJDB_LIBRARIES ${EJDB_LIBRARY})
set(EJDB_INCLUDE_DIRS ${EJDB_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set EJDB_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(EJDB DEFAULT_MSG
                                  EJDB_LIBRARY EJDB_INCLUDE_DIR)

mark_as_advanced(EJDB_INCLUDE_DIR EJDB_LIBRARY)
