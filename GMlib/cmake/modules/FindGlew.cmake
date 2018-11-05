#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_DIRS
# GLEW_LIBRARIES
# 




IF (WIN32)
  set( GLEW_PATH_WIN32 $ENV{PROGRAMFILES}/glew CACHE PATH "Custom GLEW PATH." )
  
  FIND_PATH( GLEW_INCLUDE_DIR GL/glew.h
    ${GLEW_PATH_WIN32}/include
    DOC "The directory where GL/glew.h resides"
    NO_DEFAULT_PATH
    )
  FIND_LIBRARY( GLEW_LIBRARY
    NAMES glew GLEW glew32 glew32s
    PATHS
    ${GLEW_PATH_WIN32}/lib
    DOC "The GLEW library"
    NO_DEFAULT_PATH
    )
ELSE (WIN32)
  FIND_PATH( GLEW_INCLUDE_DIR GL/glew.h
    /usr/include
    /usr/local/include
    /opt/local/include
    DOC "The directory where GL/glew.h resides")
  FIND_LIBRARY( GLEW_LIBRARY
    NAMES GLEW glew
    PATHS
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
    DOC "The GLEW library")
ENDIF (WIN32)

set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
set(GLEW_LIBRARIES ${GLEW_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Glew "GLEW not found!" GLEW_LIBRARY GLEW_INCLUDE_DIR)

MARK_AS_ADVANCED( GLEW_INCLUDE_DIR GLEW_LIBRARY )
