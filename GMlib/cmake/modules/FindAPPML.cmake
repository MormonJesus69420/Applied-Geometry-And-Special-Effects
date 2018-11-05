#
# Try to find ACML library and include path.
# Once done this will define
#
# ACML_FOUND
# ACML_INCLUDE_DIRS
# ACML_LIBRARIES
# 

IF (WIN32)
  find_package(OpenCL REQUIRED)
  
  set( APPML_PATH_WIN32 $ENV{PROGRAMFILES}/AMD/clAmdBlas CACHE PATH "Path to APPML." )
  
  find_path(APPML_INCLUDE_DIR clAmdBlas.h
    PATHS
	${APPML_PATH_WIN32}/include
    DOC "The directory where clAmdBlas.h resides")
  
  find_library(APPML_LIBRARY clAmdBlas
    PATHS
	${APPML_PATH_WIN32}/lib64/import
	${APPML_PATH_WIN32}/lib32/import
    DOC "The APPML library")
  
  if(APPML_INCLUDE_DIR AND APPML_LIBRARY)
    mark_as_advanced(FORCE APPML_PATH_WIN32)
  else(APPML_INCLUDE_DIR AND APPML_LIBRARY)
    mark_as_advanced(CLEAR APPML_PATH_WIN32)
  endif(APPML_INCLUDE_DIR AND APPML_LIBRARY)
ELSEIF(APPLE)
#TODO
ELSE (WIN32) #Linux
  file(GLOB APPML_ROOT_DIR "/opt/clAmdBlas*")
  if(APPML_ROOT_DIR)
    list(GET APPML_ROOT_DIR 0 APPML_ROOT_DIR)
  else(APPML_ROOT_DIR)
    set(APPML_ROOT_DIR "/usr")
  endif(APPML_ROOT_DIR)
  find_path( APPML_INCLUDE_DIR clAmdBlas.h
    ${APPML_ROOT_DIR}/include
    /usr/include
    /usr/local/include
    /opt/local/include
    DOC "The directory where clAmdBlas.h resides")

  find_library(APPML_LIBRARY clAmdBlas
    PATHS
        ${APPML_ROOT_DIR}/lib64
        ${APPML_ROOT_DIR}/lib32
    DOC "The APPML library")
ENDIF (WIN32)

set(APPML_INCLUDE_DIRS ${APPML_INCLUDE_DIR} ${OPENCL_INCLUDE_DIRS})
set(APPML_LIBRARIES ${APPML_LIBRARY} ${OPENCL_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(APPML "APPML not found!" APPML_LIBRARY APPML_INCLUDE_DIRS)

MARK_AS_ADVANCED( APPML_INCLUDE_DIR APPML_LIBRARY )
