#
# Try to find OpenBLAS library and include path.
# Once done this will define
#
# MKL_FOUND
# MKL_INCLUDE_DIRS
# MKL_LIBRARIES
# 

set(MKL_MULTITHREAD "YES" CACHE BOOL "Multithreaded MKL")
option(MKL_MULTITHREAD "Multithreaded MKL")
if(MKL_MULTITHREAD)
  set(MKL_THREAD_LIB "mkl_intel_thread")
else(MKL_MULTITHREAD)
  set(MKL_THREAD_LIB "mkl_sequential")
endif(MKL_MULTITHREAD)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(MKL_64 "YES" CACHE BOOL "64-bit MKL")
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(MKL_64 "NO" CACHE BOOL "64-bit MKL")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
option(MKL_64 "64-bit MKL")

IF (WIN32)
  set(MKL_DIR "$ENV{PROGRAMFILES}/Intel/Composer XE/mkl" CACHE PATH "Location of the MKL directory")
  
  if(MKL_64)
    set(MKL_SUFFIX "_lp64_dll")
    set(MKL_ARCHDIR "intel64")
  else(MKL_64)
    set(MKL_SUFFIX "_c_dll")
    set(MKL_ARCHDIR "ia32")
  endif(MKL_64)
  
  FIND_PATH(MKL_INCLUDE_DIR mkl.h
    ${MKL_DIR}/include
	DOC "The directory where mkl.h resides")
  
  unset(MKL_MKL_LIBRARY CACHE)
  FIND_LIBRARY( MKL_MKL_LIBRARY
    NAMES mkl_intel${MKL_SUFFIX}
    PATHS
    ${MKL_DIR}/lib/${MKL_ARCHDIR}
    DOC "The MKL library")
  unset(MKL_CORE_LIBRARY CACHE)
  FIND_LIBRARY( MKL_CORE_LIBRARY
    NAMES mkl_core_dll
    PATHS
    ${MKL_DIR}/lib/${MKL_ARCHDIR}
    DOC "The core MKL library")
  unset(MKL_THREAD_LIBRARY CACHE)
  FIND_LIBRARY( MKL_THREAD_LIBRARY
    NAMES ${MKL_THREAD_LIB}_dll
    PATHS
    ${MKL_DIR}/lib/${MKL_ARCHDIR}
    DOC "The MKL threading library")
  
  set(MKL_INCLUDE_DIRS ${MKL_INCLUDE_DIR})
  set(MKL_LIBRARIES ${MKL_MKL_LIBRARY} ${MKL_CORE_LIBRARY} ${MKL_THREAD_LIBRARY})

  if(MKL_MULTITHREAD)
    unset(MKL_OMP_LIBRARY CACHE)
    FIND_LIBRARY( MKL_OMP_LIBRARY
      NAMES libiomp5md
      PATHS
      ${MKL_DIR}/../compiler/lib/${MKL_ARCHDIR}
      DOC "The Intel OpenMP library")
    set(MKL_LIBRARIES ${MKL_LIBRARIES} ${MKL_OMP_LIBRARY})
  endif(MKL_MULTITHREAD)
  
ELSEIF(APPLE)
#TODO
ELSE (WIN32)
  if(MKL_64)
    set(MKL_SUFFIX "_lp64")
    set(MKL_ARCHDIR "intel64")
  else(MKL_64)
    set(MKL_ARCHDIR "ia32")
  endif(MKL_64)

  FIND_PATH( MKL_INCLUDE_DIR mkl.h
    /opt/intel/mkl/include
    DOC "The directory where mkl.h resides")

  unset(MKL_MKL_LIBRARY CACHE)
  FIND_LIBRARY( MKL_MKL_LIBRARY
    NAMES mkl_intel${MKL_SUFFIX}
    PATHS
    /opt/intel/mkl/lib/${MKL_ARCHDIR}
    DOC "The MKL library")
  unset(MKL_CORE_LIBRARY CACHE)
  FIND_LIBRARY( MKL_CORE_LIBRARY
    NAMES mkl_core
    PATHS
    /opt/intel/mkl/lib/${MKL_ARCHDIR}
    DOC "The core MKL library")
  unset(MKL_THREAD_LIBRARY CACHE)
  FIND_LIBRARY( MKL_THREAD_LIBRARY
    NAMES ${MKL_THREAD_LIB}
    PATHS
    /opt/intel/mkl/lib/${MKL_ARCHDIR}
    DOC "The MKL threading library")

  set(MKL_INCLUDE_DIRS ${MKL_INCLUDE_DIR})
  set(MKL_LIBRARIES ${MKL_MKL_LIBRARY} ${MKL_CORE_LIBRARY} ${MKL_THREAD_LIBRARY})

  if(MKL_MULTITHREAD)
    unset(MKL_OMP_LIBRARY CACHE)
    FIND_LIBRARY( MKL_OMP_LIBRARY
      NAMES iomp5
      PATHS
      /opt/intel/lib/${MKL_ARCHDIR}
      DOC "The Intel OpenMP library")
    set(MKL_LIBRARIES ${MKL_LIBRARIES} ${MKL_OMP_LIBRARY})
  endif(MKL_MULTITHREAD)

ENDIF (WIN32)


include(FindPackageHandleStandardArgs)
if(MKL_MULTITHREAD)
  find_package_handle_standard_args(MKL "MKL not found!" MKL_CORE_LIBRARY MKL_MKL_LIBRARY MKL_THREAD_LIBRARY MKL_INCLUDE_DIR MKL_OMP_LIBRARY)
else(MKL_MULTITHREAD)
  find_package_handle_standard_args(MKL "MKL not found!" MKL_CORE_LIBRARY MKL_MKL_LIBRARY MKL_THREAD_LIBRARY MKL_INCLUDE_DIR)
endif(MKL_MULTITHREAD)

MARK_AS_ADVANCED( MKL_INCLUDE_DIR MKL_LIBRARIES MKL_CORE_LIBRARY MKL_MKL_LIBRARY MKL_OMP_LIBRARY MKL_THREAD_LIBRARY)
