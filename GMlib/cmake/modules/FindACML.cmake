#
# Try to find ACML library and include path.
# Once done this will define
#
# ACML_FOUND
# ACML_INCLUDE_DIRS
# ACML_LIBRARIES
# 

option(ACML_MULTITHREAD "Multithreaded ACML")
if(ACML_MULTITHREAD)
  set(ACML_MP_SUFFIX "_mp")
else(ACML_MULTITHREAD)
  set(ACML_MP_SUFFIX "")
endif(ACML_MULTITHREAD)

option(ACML_FMA4 "Utilize FMA4 instruction set (AMD only)")

IF (WIN32)
  set(ACML_PATH_WIN32 $ENV{PROGRAMFILES}/AMD CACHE PATH "ACML root directory.")
  if(ACML_FMA4)
	set(ACML_FMA4_SUFFIX "_fma4")
  else(ACML_FMA4)
	set(ACML_FMA4_SUFFIX "")
  endif(ACML_FMA4)
  
  FIND_PATH(ACML_INCLUDE_DIR acml.h
    PATHS
    ${ACML_PATH_WIN32}/win64${ACML_FMA4_SUFFIX}${ACML_MP_SUFFIX}/include
	DOC "The directory of acml.h")
	
  FIND_LIBRARY(ACML_LIBRARY
    NAMES libacml${ACML_MP_SUFFIX}_dll
	PATHS
	${ACML_PATH_WIN32}/win64${ACML_FMA4_SUFFIX}${ACML_MP_SUFFIX}/lib
	DOC "nuino")
  
  if(ACML_INCLUDE_DIR AND ACML_LIBRARY)
    mark_as_advanced(FORCE ACML_PATH_WIN32)
  else(ACML_INCLUDE_DIR AND ACML_LIBRARY)
    mark_as_advanced(CLEAR ACML_PATH_WIN32)
  endif(ACML_INCLUDE_DIR AND ACML_LIBRARY)

ELSEIF(APPLE)
#TODO
ELSE (WIN32) #Linux
  unset(ACML_INCLUDE_DIR CACHE)
  unset(ACML_LIBRARY CACHE)


  file(GLOB ACML_H_DIRS "/opt/acml*/gfortran*${ACML_MP_SUFFIX}/include")
  if(ACML_H_DIRS)
    list(GET ACML_H_DIRS 0 ACML_H_DIRS)
  else(ACML_H_DIRS)
    set(ACML_H_DIRS "/usr/include")
  endif(ACML_H_DIRS)
  FIND_PATH( ACML_INCLUDE_DIR acml.h
    ${ACML_H_DIRS}
    /usr/include
    /usr/local/include
    /opt/local/include
    DOC "The directory where acml.h resides")

  file(GLOB ACML_SO_DIRS "/opt/acml*/gfortran*${ACML_MP_SUFFIX}/lib")
  file(GLOB ACML_SO_FMA4_DIRS "/opt/acml*/gfortran*_fma4*${ACML_MP_SUFFIX}/lib")
  if(ACML_SO_DIRS)
    if(NOT ACML_FMA4)
      list(REMOVE_ITEM ACML_SO_DIRS ${ACML_SO_FMA4_DIRS})
      list(GET ACML_SO_DIRS 0 ACML_SO_DIRS)
    else(NOT ACML_FMA4)
      list(GET ACML_SO_DIRS 0 ACML_SO_FMA4_DIRS)
    endif(NOT ACML_FMA4)
  else(ACML_SO_DIRS)
    set(ACML_SO_DIRS "/usr/lib")
  endif(ACML_SO_DIRS)
  FIND_LIBRARY( ACML_LIBRARY
    NAMES acml acml_mp
    PATHS
    ${ACML_SO_DIRS}
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
    DOC "The ACML library")
#TODO: 64 bit
ENDIF (WIN32)

set(ACML_INCLUDE_DIRS ${ACML_INCLUDE_DIR})
set(ACML_LIBRARIES ${ACML_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ACML "ACML not found!" ACML_LIBRARY ACML_INCLUDE_DIR)

MARK_AS_ADVANCED( ACML_INCLUDE_DIR ACML_LIBRARY )
