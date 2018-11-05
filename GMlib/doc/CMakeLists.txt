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


find_package(Doxygen)

if(DOXYGEN_FOUND)

  # set vars
  set( VAR_LIBRARY_NAME         ${LIBRARY_NAME} )
  set( VAR_LIBRARY_VERSION      ${LIBRARY_VERSION} )
  set( VAR_DOXYGEN_OUTPUT_DIR   ${BUILD_DOC_DIR} )
  set( VAR_SOURCE_PATH          "${CMAKE_SOURCE_DIR}/src" )
#  set( VAR_SOURCE_PATH          ${BUILD_SRC_DIR} )

  # configure doxyfile
  configure_file(
    ${ECMAKE_TEMPLATE_DIR}/doxyfile.in
    ${BUILD_DOC_DIR}/Doxyfile
    @ONLY
  )

  # add custom doxygen target
  set( HTML_TARGET "html" )
  add_custom_target( ${HTML_TARGET} ALL
    ${DOXYGEN_EXECUTABLE} ${BUILD_DOC_DIR}/Doxyfile
    DEPENDS ${BUILD_DOC_DIR}/Doxyfile
  )

endif()
