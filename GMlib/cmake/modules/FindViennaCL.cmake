#
# Try to find ViennaCL library and include path.
# Once done this will define
#
# VIENNACL_FOUND
# VIENNACL_INCLUDE_DIRS
# VIENNACL_LIBRARIES
# VIENNACL_WITH_OPENCL
# 


option(VIENNACL_WITH_OPENCL "Use ViennaCL with OpenCL" YES)

IF(VIENNACL_WITH_OPENCL)
  find_package(OpenCL REQUIRED)
ENDIF(VIENNACL_WITH_OPENCL)

IF (WIN32)
  set(VIENNACL_PATH_WIN32 $ENV{PROGRAMFILES}/ViennaCL CACHE PATH "ViennaCL root directory.")

  find_path(VIENNACL_INCLUDE_DIR viennacl/forwards.h
    PATHS ${VIENNACL_PATH_WIN32}/include
	DOC "The ViennaCL include path")
  
  if(VIENNACL_INCLUDE_DIR)
    mark_as_advanced(FORCE VIENNACL_PATH_WIN32)
  else(VIENNACL_INCLUDE_DIR)
    mark_as_advanced(CLEAR VIENNACL_PATH_WIN32)
  endif(VIENNACL_INCLUDE_DIR)

ELSEIF(APPLE)
#TODO
ELSE (WIN32) #Linux
  find_path(VIENNACL_INCLUDE_DIR viennacl/forwards.h
    PATHS /usr/local/include
    DOC "The ViennaCL include path")
ENDIF (WIN32)

include(FindPackageHandleStandardArgs)
if(VIENNACL_WITH_OPENCL)
  set(VIENNACL_INCLUDE_DIRS ${VIENNACL_INCLUDE_DIR} ${OPENCL_INCLUDE_DIRS})
  set(VIENNACL_LIBRARIES ${OPENCL_LIBRARIES})
  find_package_handle_standard_args(ViennaCL "ViennaCL not found!" VIENNACL_INCLUDE_DIR OPENCL_INCLUDE_DIRS OPENCL_LIBRARIES)
else(VIENNACL_WITH_OPENCL)
  set(VIENNACL_INCLUDE_DIRS ${VIENNACL_INCLUDE_DIR})
  set(VIENNACL_LIBRARIES "")
  find_package_handle_standard_args(ViennaCL "ViennaCL not found!" VIENNACL_INCLUDE_DIR)
endif(VIENNACL_WITH_OPENCL)


MARK_AS_ADVANCED( VIENNACL_INCLUDE_DIR )
