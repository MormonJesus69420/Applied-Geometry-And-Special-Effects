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
# <Global>
list( APPEND HEADERS
  gmglobal.h
)

###
# Blas




###
# Containers
list( APPEND HEADERS
  containers/gmarray.h
  containers/gmarraylx.h
  containers/gmarrayt.h
  containers/gmdmatrix.h
  containers/gmdvector.h
  containers/gmdvectorn.h
)

list( APPEND HEADER_SOURCES
  containers/gmarray.c
  containers/gmarraylx.c
  containers/gmarrayt.c
  containers/gmdmatrix.c
  containers/gmdvector.c
  containers/gmdvectorn.c
)



###
# Static
list( APPEND HEADERS
  static/gmstaticproc.h
  static/gmstaticproc2.h
)

list( APPEND HEADER_SOURCES
  static/gmstaticproc2.c
)



###
# Types
list( APPEND HEADERS
  types/gmangle.h
  types/gmmatrix.h
  types/gmpoint.h
  types/gmsimplex.h
  types/gmsubspace.h
)

list( APPEND HEADER_SOURCES
  types/gmmatrix.c
  types/gmpoint.c
  types/gmsimplex.c
  types/gmsubspace.c
)



###
# Utils
list( APPEND HEADERS
  utils/gmcolor.h
  utils/gmdivideddifferences.h
  utils/gmrandom.h
  utils/gmsortobject.h
  utils/gmstream.h
  utils/gmstring.h
  utils/gmtimer.h
  utils/gmutils.h
)

list( APPEND HEADER_SOURCES
  utils/gmdivideddifferences.c
  utils/gmrandom.c
  utils/gmsortobject.c
  utils/gmstring.c
  utils/gmtimer.c
)

set( SOURCES
  utils/gmcolor.cpp
  utils/gmstream.cpp
)





######### Add to Module

GM_ADD_HEADERS(${HEADERS})
GM_ADD_HEADER_SOURCES(${HEADER_SOURCES})

GM_ADD_TEMPLATE_TARGET(${HEADERS} ${HEADER_SOURCES} )

GM_ADD_LIBRARY(${HEADERS} ${SOURCES})
GM_SET_DEFAULT_TARGET_PROPERTIES()






















#addHeaders( gmGlobal )

#addSubDirectory(containers)   # <containers> sub-module
#addSubDirectory(blas)         # <blas> sub-module
#addSubDirectory(static)       # <static> sub-module
#addSubDirectory(types)        # <types> sub-module
#addSubDirectory(utils)        # <utils> sub-module

#if( BLAS_FOUND )
#  moduleLinkLibrary( ${BLAS_LIBRARIES} )
#endif( BLAS_FOUND )
