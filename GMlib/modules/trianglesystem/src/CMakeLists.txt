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



###
# <global>
list( APPEND HEADERS
  gmtrianglesystem.h )

list( APPEND HEADER_SOURCES
  gmtrianglesystem.c
)



###
# Visualizers
list( APPEND HEADERS
  visualizers/gmtrianglefacetsdefaultvisualizer.h
  visualizers/gmtrianglefacetsvisualizer.h
  visualizers/gmtrianglefacetsvoronoivisualizer.h
)

list( APPEND HEADER_SOURCES
  visualizers/gmtrianglefacetsdefaultvisualizer.c
  visualizers/gmtrianglefacetsvisualizer.c
  visualizers/gmtrianglefacetsvoronoivisualizer.c
)



GM_ADD_HEADERS(${HEADERS})
GM_ADD_HEADER_SOURCES(${HEADER_SOURCES})

GM_ADD_TEMPLATE_TARGET(${HEADERS} ${HEADER_SOURCES})


#addHeaders(
#  gmTriangleSystem
#)

#addTemplateSources(
#  gmtrianglesystem.c
#)

#addSubDirectory(visualizers)
