# ###############################################################################
# #
# # Copyright (C) 1994 Narvik University College
# # Contact: GMlib Online Portal at http://episteme.hin.no
# #
# # This file is part of the Geometric Modeling Library, GMlib.
# #
# # GMlib is free software: you can redistribute it and/or modify
# # it under the terms of the GNU Lesser General Public License as published by
# # the Free Software Foundation, either version 3 of the License, or
# # (at your option) any later version.
# #
# # GMlib is distributed in the hope that it will be useful,
# # but WITHOUT ANY WARRANTY; without even the implied warranty of
# # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# # GNU Lesser General Public License for more details.
# #
# # You should have received a copy of the GNU Lesser General Public License
# # along with GMlib. If not, see <http://www.gnu.org/licenses/>.
# #
# ###############################################################################

# Minimum CMake version required
cmake_minimum_required(VERSION 3.4)

# Nice name for the IDEs
project(GMlib VERSION 0.6.9 LANGUAGES CXX)

# Specify compiler features
set(CMAKE_CXX_STANDARD 17)

# Compiler options
if(MSVC)
  message("Compiler: ${CMAKE_CXX_COMPILER_ID}")
elseif (CMAKE_CXX_COMPILER_ID MATCHES GNU)
  message("Compiler: ${CMAKE_CXX_COMPILER_ID}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Werror")
elseif (CMAKE_CXX_COMPILER_ID MATCHES Clang)
  message("Compiler: ${CMAKE_CXX_COMPILER_ID}")

  # Default compile flags (read: turn on all checks)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-documentation -pedantic -Werror")

  # Extra compile flags to turn of stuff we don't want to handle at the moment
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-destructor-override")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-variable-declarations")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-zero-as-null-pointer-constant")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conditional-uninitialized")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-statement-expression")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unreachable-code-return")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-member-function")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-covered-switch-default")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undefined-var-template")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exit-time-destructors")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-implicit-fallthrough")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-private-field")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-qualifiers")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-prototypes")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reserved-id-macro")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-double-promotion")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-non-virtual-dtor")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-redundant-parens")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unreachable-code") #main.cpp lines 19-21
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-conversion")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-old-style-cast")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-macros")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-shadow-field")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-float-equal")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-empty-body")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-shadow")
else()
  message(WARNING "Compiler <${CMAKE_CXX_COMPILER_ID}> is not a supported C++ compiler.")
endif()

# Extra CMake modules
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/" )

# Include some cmake utils
include(cmake/utils.cmake)
GM_INIT()


####### TEMP !!!!!!!!!!! TMP
#add_custom_target( ECMake SOURCES ecmake/enhanced.cmake )
add_subdirectory(cmake)
####### TEMP !!!!!!!!!!! TMP


# Options
option( GM_STREAM "Enable output stream operators for core types." ON )

# Developer/debug options
option( GM_DEVELOPER_MODE "Set gmlib in developer mode" ON )
#option( GM_DEBUG "Enable internal debugging, assertions, etc." FALSE )
option( GM_DEBUG_OPENGL "Enable opengl debug. OpenGL function calls are safe called." ON )
option( GM_TESTING "Create unit tests." OFF )
option( GM_BENCHMARKING "Create benchmarks." OFF )


# API documentation options
#option( GM_DOXYGEN "Create build target for doxygen documentation." FALSE )


#################
## Developer Mode
if(GM_DEVELOPER_MODE)

  ##########
  ## TESTING
  if(GM_TESTING)
    message("Finding GTest")
    find_package(GTest REQUIRED)
    if(GTEST_FOUND)
      include_directories(${GTEST_INCLUDE_DIRS})
      message("GTest include dir: ${GTEST_INCLUDE_DIRS}")
      enable_testing()
    else(GTEST_FOUND)
      message(WARNING "GTest NOT found: Unittests disabled")
      message(WARNING "GTest include dir: ${GTEST_INCLUDE_DIRS}")
      set(GM_TESTING FALSE)
    endif(GTEST_FOUND)

  endif(GM_TESTING)

  if(GM_BENCHMARKING)
    message("Finding Google Benchmark:")
    find_package(benchmark)
    if(benchmark_FOUND)
      message(WARNING "Google benchmark found: Benchmarking enabled!")
      enable_testing()
    else(benchmark_FOUND)
      message(WARNING "Google benchmark NOT fould: benchmarks disabled!")
      set(GM_TESTING FALSE)
    endif(benchmark_FOUND)
  endif(GM_BENCHMARKING)

  if(GM_DEBUG_OPENGL)
    GM_ADD_DEFINITION( GM_GL_DEBUG )
    add_definitions( -DGM_GL_DEBUG )
  endif(GM_DEBUG_OPENGL)

endif(GM_DEVELOPER_MODE)

##########################
# Add GM_STREAM definition
if(GM_STREAM)
  GM_ADD_DEFINITION( GM_STREAM )
  add_definitions(-DGM_STREAM)
  message("GMStream enabled")
endif(GM_STREAM)

##########################################
# Build shared libs instead of static libs
option( GM_BUILD_SHARED "Build shared libs instead of static libs." TRUE )
if(GM_BUILD_SHARED)
  message( "GMlib: Building shared libs!" )
else(GM_BUILD_SHARED)
  message( "GMlib: Building static libs!" )
endif(GM_BUILD_SHARED)

################################################
# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )

# Add modules directory
add_subdirectory(modules)


#message( "custom config: ${GM_CUSTOM_CONFIG}" )


GM_JOIN( "${GM_CUSTOM_CONFIG}" "\n" VAR_CUSTOM_CONFIG )


#foreach( TARGET ${GM_MODULE_TARGETS} )
##  get_target_property( TARGET_LOC ${TARGET} ${CMAKE_BUILD_TYPE}_LOCATION )
##  set( TARGET_LOC $<TARGET_FILE:${TARGET}>)
#  message( "target: ${TARGET}, LOC: ${TARGET_LOC}"  )
#endforeach()


set( GM_DEFINITIONS_STR "" )
foreach( DEFINITION ${GM_DEFINITIONS} )
  set( GM_DEFINITIONS_STR "-D${DEFINITION} ${GM_DEFINITIONS_STR}" )
endforeach()
set( VAR_DEFINITIONS "set( ${PROJECT_NAME}_DEFINITIONS ${GM_DEFINITIONS_STR})" )

GM_JOIN( "${GM_MODULE_TARGETS}" " " VAR_MODULE_TARGETS )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY ExactVersion
)

set(INCLUDE_INSTALL_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(SYSCONFIG_INSTALL_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
configure_package_config_file(
  ${CMAKE_SOURCE_DIR}/cmake/common/config.cmake.in
  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_LIBRARY_PATH}/GMlib/cmake
  PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR
)

add_custom_target( CustomTargetForOtherFiles SOURCES )
