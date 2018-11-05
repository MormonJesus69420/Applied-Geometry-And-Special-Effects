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
  gmscaleobject.h
  gmscene.h
  gmsceneobject.h
  gmvisualizer.h
)

list( APPEND SOURCES
  gmscene.cpp
  gmsceneobject.cpp
  gmvisualizer.cpp
)


###
# Camera
list( APPEND HEADERS
  camera/gmcamera.h
  camera/gmisocamera.h
)

list( APPEND SOURCES
  camera/gmcamera.cpp
  camera/gmisocamera.cpp
)


###
# Displayobjects
list( APPEND HEADERS
  sceneobjects/gmarrow3d.h
  sceneobjects/gmcoordinatesystem.h
  sceneobjects/gmpathtrack.h
  sceneobjects/gmpointlightg.h
  sceneobjects/gmsphere3d.h
  sceneobjects/gmspotlightg.h
)

list( APPEND SOURCES
  sceneobjects/gmcoordinatesystem.cpp
  sceneobjects/gmpathtrack.cpp
  sceneobjects/gmpointlightg.cpp
  sceneobjects/gmspotlightg.cpp
)


###
# Event
list( APPEND HEADERS
  event/gmevent.h
  event/gmeventcontroller.h
  event/gmeventmanager.h
  event/gmsceneobjectattribute.h
)

list( APPEND SOURCES
  event/gmevent.cpp
  event/gmeventcontroller.cpp
  event/gmeventmanager.cpp
  event/gmsceneobjectattribute.cpp
)


###
# Light
list( APPEND HEADERS
  light/gmlight.h
  light/gmpointlight.h
  light/gmspotlight.h
  light/gmsun.h
)

list( APPEND SOURCES
  light/gmlight.cpp
  light/gmpointlight.cpp
  light/gmspotlight.cpp
  light/gmsun.cpp
)


###
# Render
list( APPEND HEADERS
  render/gmdefaultrenderer.h
  render/gmdefaultselectrenderer.h
  render/gmrenderer.h
  render/gmrendertarget.h
  render/rendertargets/gmnativerendertarget.h
  render/rendertargets/gmtexturerendertarget.h
)

list( APPEND SOURCES
  render/gmdefaultrenderer.cpp
  render/gmdefaultselectrenderer.cpp
  render/gmrenderer.cpp
  render/rendertargets/gmtexturerendertarget.cpp
)


###
# Selector
list( APPEND HEADERS
  selector/gmselector.h
)

list( APPEND HEADER_SOURCES
  selector/gmselector.c
)


###
# Utils
list( APPEND HEADERS
  utils/gmmaterial.h
  utils/gmtexture.h
)

list( APPEND SOURCES
  utils/gmmaterial.cpp
  utils/gmtexture.cpp
)


###
# Visualizer
list( APPEND HEADERS
  visualizers/gmcameravisualizer.h
  visualizers/gmcoordsysrepvisualizer.h
  visualizers/gmselectorvisualizer.h
  visualizers/gmselectorgridvisualizer.h
  visualizers/gmsurroundingspherevisualizer.h
  visualizers/gmstdrepvisualizer.h
)

list( APPEND HEADER_SOURCES
  visualizers/gmselectorgridvisualizer.c
)

list( APPEND SOURCES
  visualizers/gmcameravisualizer.cpp
  visualizers/gmcoordsysrepvisualizer.cpp
  visualizers/gmselectorvisualizer.cpp
  visualizers/gmsurroundingspherevisualizer.cpp
  visualizers/gmstdrepvisualizer.cpp
)


###
# Window
#list( APPEND HEADERS
#  window/gmview.h
#  window/gmviewset.h
#  window/gmwindow.h
#)

#list( APPEND SOURCES
#  window/gmview.cpp
#  window/gmviewset.cpp
#  window/gmwindow.cpp
#)



GM_ADD_HEADERS(${HEADERS})
GM_ADD_HEADER_SOURCES(${HEADER_SOURCES})

GM_ADD_DEPENDENCY_TARGET(${HEADER_SOURCES})


GM_ADD_LIBRARY(${HEADERS} ${SOURCES} )
GM_SET_DEFAULT_TARGET_PROPERTIES()
GM_TARGET_LINK_LIBRARIES( gmopengl gmcore ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} )
GM_ADD_DEPENDENCIES(${GM_DEP_TARGET})















#addHeaders(
#  gmDisplayObject
#  gmFrustum
#  gmScaleObject
#  gmScene
#  gmSceneObject
#  gmVisualizer
#)

#addSources(
#  gmdisplayobject.cpp
#  gmscene.cpp
#  gmsceneobject.cpp
#  gmvisualizer.cpp
#)

#addSubDirectory(camera)           # <camera> sub-module
#addSubDirectory(displayobjects)   # <displayobject> sub-module
#addSubDirectory(event)            # <event> sub-module
#addSubDirectory(light)            # <light> sub-module
#addSubDirectory(render)           # <render> sub-module
#addSubDirectory(selector)         # <selector> sub-module
#addSubDirectory(utils)            # <utils> sub-module
#addSubDirectory(visualizers)      # <visualizers> sub-module
#addSubDirectory(window)           # <window> sub-module

