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
  gmbufferobject.h
  gmframebufferobject.h
  gmglobject.h
  gmopengl.h
  gmopenglmanager.h
  gmprogrampipeline.h
  gmprogram.h
  gmrenderbufferobject.h
  gmshader.h
  gmtexture.h
)

list( APPEND SOURCES
  gmbufferobject.cpp
  gmframebufferobject.cpp
  gmopenglmanager.cpp
  gmprogrampipeline.cpp
  gmprogram.cpp
  gmrenderbufferobject.cpp
  gmshader.cpp
  gmtexture.cpp
)


###
# BufferObjects
list( APPEND HEADERS
  bufferobjects/gmindexbufferobject.h
  bufferobjects/gmtexturebufferobject.h
  bufferobjects/gmuniformbufferobject.h
  bufferobjects/gmvertexbufferobject.h
)

list( APPEND SOURCES
  bufferobjects/gmindexbufferobject.cpp
  bufferobjects/gmtexturebufferobject.cpp
  bufferobjects/gmuniformbufferobject.cpp
  bufferobjects/gmvertexbufferobject.cpp
)


###
# Shaders
list( APPEND HEADERS
  shaders/gmcomputeshader.h
  shaders/gmfragmentshader.h
  shaders/gmgeometryshader.h
  shaders/gmtesscontrolshader.h
  shaders/gmtessevaluationshader.h
  shaders/gmvertexshader.h
)

list( APPEND SOURCES
  shaders/gmcomputeshader.cpp
  shaders/gmfragmentshader.cpp
  shaders/gmgeometryshader.cpp
  shaders/gmtesscontrolshader.cpp
  shaders/gmtessevaluationshader.cpp
  shaders/gmvertexshader.cpp
)


###
# Utils
list( APPEND HEADERS
  utils/gmutils.h
)


######### Add to Module

GM_ADD_HEADERS(${HEADERS})

GM_ADD_LIBRARY(${SOURCES} ${HEADERS})
GM_SET_DEFAULT_TARGET_PROPERTIES()

GM_TARGET_LINK_LIBRARIES( ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} )



























#addHeaders(
#  gmBufferObject
#  gmFramebufferObject
#  gmGLObject
#  gmOpenGL
#  gmOpenGLManager
#  gmProgram
#  gmRenderbufferObject
#  gmShader
#  gmTexture
#)

#addSources(
#  gmbufferobject.cpp
#  gmframebufferobject.cpp
#  gmopenglmanager.cpp
#  gmprogram.cpp
#  gmrenderbufferobject.cpp
#  gmshader.cpp
#  gmtexture.cpp
#)

#addSubDirectory(bufferobjects)          # <bufferobjects> sub-module
#addSubDirectory(shaders)                # <shaders> sub-module
#addSubDirectory(utils)                  # <utils> sub-module

#moduleLinkLibrary( ${GLEW_LIBRARY} )
#moduleLinkLibrary( ${OPENGL_LIBRARIES} )

#addTestDir(test)
