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



# JOIN function to join lists
# ref: Andrey Kamaev - stackoverflow.com
# http://stackoverflow.com/questions/7172670/best-shortest-way-to-join-a-list-in-cmake
function(GM_JOIN VALUES GLUE OUTPUT)
  string (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
  string (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

function(GM_STRINGFIRSTTOUPPER INPUT OUTPUT)
  string(SUBSTRING ${INPUT} 0 1 IN_PART0)
  string(SUBSTRING ${INPUT} 1 -1 IN_PART1)
  string(TOUPPER ${IN_PART0} IN_PART0)
  set(${OUTPUT} ${IN_PART0}${IN_PART1} PARENT_SCOPE)
endfunction()






function(GM_INIT)
  if(GM_MODULE_TARGETS)
    unset(GM_MODULE_TARGETS CACHE)
  endif(GM_MODULE_TARGETS)

  if(GM_CUSTOM_CONFIG)
    unset(GM_CUSTOM_CONFIG CACHE)
  endif(GM_CUSTOM_CONFIG)

  if(GM_DEFINITIONS)
    unset(GM_DEFINITIONS CACHE)
  endif(GM_DEFINITIONS)
endfunction(GM_INIT)

function(GM_ADD_LIBRARY)
  set( TARGET gm${PROJECT_NAME} )

  if(GM_BUILD_SHARED)
    add_library( ${TARGET} SHARED ${ARGN} )
  else(GM_BUILD_SHARED)
    add_library( ${TARGET} STATIC ${ARGN} )
  endif(GM_BUILD_SHARED)

  if( NOT GM_MODULE_TARGETS )
    set( GM_MODULE_TARGETS ${TARGET} CACHE INTERNAL "TMP module targets variable" )
  else()
    set( GM_MODULE_TARGETS ${TARGET} ${GM_MODULE_TARGETS} CACHE INTERNAL "TMP module targets variable" )
  endif()
endfunction(GM_ADD_LIBRARY)

function(GM_ADD_MODULE_DEPENDENCIES TARGET )
  foreach(ARG ${ARGN})
    add_dependencies( gm${TARGET} gm${ARG})
  endforeach(ARG)
endfunction(GM_ADD_MODULE_DEPENDENCIES)

function(GM_ADD_DEPENDENCIES)
  add_dependencies(gm${PROJECT_NAME} ${ARGN})
endfunction(GM_ADD_DEPENDENCIES)

function(GM_SET_DEFAULT_TARGET_PROPERTIES)
#  set_target_properties( gm${PROJECT_NAME} PROPERTIES
#    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
#  )
endfunction(GM_SET_DEFAULT_TARGET_PROPERTIES)





function(GM_TARGET_LINK_LIBRARIES)
  target_link_libraries(gm${PROJECT_NAME} ${ARGN})
endfunction(GM_TARGET_LINK_LIBRARIES)



function(GM_ADD_CUSTOM_CONFIG CONFIG)
#  message("add custom config: ${CONFIG}" )

  if( NOT GM_CUSTOM_CONFIG )
    set( GM_CUSTOM_CONFIG ${CONFIG} CACHE INTERNAL "TMP custom config var" )
  else()
    set( GM_CUSTOM_CONFIG ${CONFIG} ${GM_CUSTOM_CONFIG} CACHE INTERNAL "TMP custom config var" )
  endif()
endfunction(GM_ADD_CUSTOM_CONFIG)



function(GM_ADD_DEFINITION DEFINITION)
#  message( "Adding definition: ${DEFINITION}" )

  if( NOT GM_DEFINITIONS )
    set( GM_DEFINITIONS ${DEFINITION} CACHE INTERNAL "TMP definitions var" )
  else()
    set( GM_DEFINITIONS ${DEFINITION} ${GM_DEFINITIONS} CACHE INTERNAL "TMP definitions var" )
  endif()
endfunction(GM_ADD_DEFINITION)






function(GM_ADD_TEMPLATE_TARGET)

  add_custom_target( ${PROJECT_NAME}TemplateModuleTarget SOURCES ${ARGN} )

endfunction(GM_ADD_TEMPLATE_TARGET)

macro(GM_ADD_DEPENDENCY_TARGET)
  set( GM_DEP_TARGET ${PROJECT_NAME}DependencyTarget)
  add_custom_target( ${GM_DEP_TARGET} SOURCES ${ARGN} )
endmacro(GM_ADD_DEPENDENCY_TARGET)



function(GM_ADD_HEADERS)

  # Helper vars
  set( INCLUDE_DIR "${CMAKE_BINARY_DIR}/include" )
  set( MODULE_INCLUDE_DIR "${INCLUDE_DIR}/${PROJECT_NAME}" )

  # Create include directory if it doesn't exist
  file( MAKE_DIRECTORY ${MODULE_INCLUDE_DIR} )

  foreach(ARG ${ARGN})

    get_filename_component( HEADER_DIR ${ARG} DIRECTORY )
    if(HEADER_DIR)
      file( MAKE_DIRECTORY "${MODULE_INCLUDE_DIR}/${HEADER_DIR}" )
    endif(HEADER_DIR)
#    message( "ARG: ${ARG}" )
#    message( "...: ${MODULE_INCLUDE_DIR}/${ARG}" )
    if(GM_DEVELOPER_MODE)
      set( CONF_HEADER_INCLUDES "#include \"${CMAKE_CURRENT_SOURCE_DIR}/${ARG}\""  )
      configure_file( ${CMAKE_SOURCE_DIR}/cmake/common/redirect_header.h.in "${MODULE_INCLUDE_DIR}/${ARG}" @ONLY )
    else()
      configure_file( ${ARG} "${MODULE_INCLUDE_DIR}/${ARG}" COPYONLY )
    endif(GM_DEVELOPER_MODE)

    # Extract filename without directory and longest extension
    get_filename_component(CXX_HEADER ${ARG} NAME_WE)

    # Create C++ header
    list( APPEND CXX_HEADER_INCLUDES "#include \"${PROJECT_NAME}/${CXX_HEADER}\"" )
    set( CXX_HEADER "${MODULE_INCLUDE_DIR}/${CXX_HEADER}" )
    set( CONF_HEADER_INCLUDES "#include \"${ARG}\"" )
    configure_file( ${CMAKE_SOURCE_DIR}/cmake/common/redirect_header.h.in ${CXX_HEADER} @ONLY )

  endforeach(ARG)

  GM_STRINGFIRSTTOUPPER(${PROJECT_NAME} MODULE_NAME)
  set( CXX_MODULE_HEADER "${INCLUDE_DIR}/gm${MODULE_NAME}Module" )

  GM_JOIN( "${CXX_HEADER_INCLUDES}" "\n" CONF_HEADER_INCLUDES )
  configure_file( ${CMAKE_SOURCE_DIR}/cmake/common/redirect_header.h.in ${CXX_MODULE_HEADER} @ONLY )

endfunction(GM_ADD_HEADERS)

function(GM_ADD_HEADER_SOURCES)

  # Helper vars
  set( INCLUDE_DIR "${CMAKE_BINARY_DIR}/include" )
  set( MODULE_INCLUDE_DIR "${INCLUDE_DIR}/${PROJECT_NAME}" )

  # Create include directory if it doesn't exist
  file( MAKE_DIRECTORY ${MODULE_INCLUDE_DIR} )

  foreach(ARG ${ARGN})

    get_filename_component( HEADER_DIR ${ARG} DIRECTORY )
    if(HEADER_DIR)
      file( MAKE_DIRECTORY "${MODULE_INCLUDE_DIR}/${HEADER_DIR}" )
    endif(HEADER_DIR)

    if(NOT GM_DEVELOPER_MODE)
      configure_file( ${ARG} "${MODULE_INCLUDE_DIR}/${ARG}" COPYONLY )
    endif(NOT GM_DEVELOPER_MODE)

    # Extract filename without directory and longest extension
    get_filename_component(CXXHEADER ${ARG} NAME_WE)
  endforeach(ARG)

endfunction(GM_ADD_HEADER_SOURCES)
















function(GM_ADD_TEST FILENAME)
  if(GM_TESTING)
    set(TEST_NAME "gmtest_${FILENAME}")
    add_executable(${TEST_NAME} ${FILENAME}.cc)
    target_link_libraries(${TEST_NAME} ${ARGN} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
  endif(GM_TESTING)
endfunction(GM_ADD_TEST)

function(GM_ADD_TESTS FILENAME)
  if(GM_TESTING)
    set(TEST_SET_NAME "gmtest_${FILENAME}")
    add_executable( ${TEST_SET_NAME} ${FILENAME}.cc )
    target_link_libraries(${TEST_SET_NAME} ${ARGN} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread )
    gtest_add_tests( ${TEST_SET_NAME} "" ${FILENAME}.cc )
  endif(GM_TESTING)
endfunction(GM_ADD_TESTS)







function(GM_ADD_BENCHMARK FILENAME)
  if(GM_BENCHMARKING)

    set(BENCH_NAME "gmbench_${FILENAME}")
    add_executable(${BENCH_NAME} ${FILENAME}.cc)
    target_link_libraries(${BENCH_NAME} ${ARGN} benchmark::benchmark pthread)
    add_test(NAME ${BENCH_NAME} COMMAND ${BENCH_NAME})

  endif(GM_BENCHMARKING)
endfunction(GM_ADD_BENCHMARK)









