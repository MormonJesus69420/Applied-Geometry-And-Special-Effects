#
# Try to find OpenBLAS library and include path.
# Once done this will define
#
# OPENBLAS_FOUND
# OPENBLAS_INCLUDE_DIRS
# OPENBLAS_LIBRARIES
# 


IF (WIN32)
#TODO
ELSEIF(APPLE)
#TODO
ELSE (WIN32)
  FIND_PATH( OPENBLAS_INCLUDE_DIR cblas.h
    /usr/include
    /usr/local/include
    /opt/local/include
    DOC "The directory where cblas.h resides")
  FIND_LIBRARY( OPENBLAS_LIBRARY
    NAMES openblas
    PATHS
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
    DOC "The OpenBLAS library")
ENDIF (WIN32)

set(OPENBLAS_INCLUDE_DIRS ${OPENBLAS_INCLUDE_DIR})
set(OPENBLAS_LIBRARIES ${OPENBLAS_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenBLAS "OpenBLAS not found!" OPENBLAS_LIBRARY OPENBLAS_INCLUDE_DIR)

MARK_AS_ADVANCED( OPENBLAS_INCLUDE_DIR OPENBLAS_LIBRARY )
