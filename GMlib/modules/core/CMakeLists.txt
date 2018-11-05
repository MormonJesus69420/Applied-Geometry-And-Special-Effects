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


# Set name of module
project(core)

# Add source directory
add_subdirectory(src)

# Add unit test directory
include_directories(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)

#STRINGFIRSTTOUPPER( ${PROJECT_NAME} PROJECT_NAME_CAMEL )
#set( FULL_PROJECT_NAME ${CMAKE_PROJECT_NAME}${PROJECT_NAME_CAMEL} )

#set( ${FULL_PROJECT_NAME}_VERSION 0.5.0 )




##set( INCLUDE_INSTALL_DIR    include/${CMAKE_PROJECT_NAME} )
##set( LIB_INSTALL_DIR        lib/ )
##set( SYSCONFIG_INSTALL_DIR  etc/${CMAKE_PROJECT_NAME}/ )


include(CMakePackageConfigHelpers)

##configure_package_config_file(
##  ${CMAKE_SOURCE_DIR}/cmake/common/config.cmake.in
##  ${CMAKE_CURRENT_BINARY_DIR}/${FULL_PROJECT_NAME}Config.cmake
##  INSTALL_DESTINATION ${CMAKE_LIBRARY_PATH}/GMlib/cmake
##  PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR
##)

#write_basic_package_version_file(
#  ${CMAKE_CURRENT_BINARY_DIR}/${FULL_PROJECT_NAME}ConfigVersion.cmake
#  VERSION ${GMlib_VERSION}
#  COMPATIBILITY ExactVersion
#)
