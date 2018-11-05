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



include_directories(${CMAKE_BINARY_DIR}/include)


# Find OpenGL
find_package(OpenGL REQUIRED)
if( OPENGL_FOUND )
  foreach(OPENGL_INCLUDE_DIR ${OPENGL_INCLUDE_DIRS})
    set(OPENGL_INCLUDE_DIRS_CONC "${OPENGL_INCLUDE_DIRS_CONC}\ \"${OPENGL_INCLUDE_DIR}\"")
  endforeach(OPENGL_INCLUDE_DIR)
  foreach(OPENGL_LIBRARY ${OPENGL_LIBRARIES})
    set(OPENGL_LIBRARIES_CONC "${OPENGL_LIBRARIES_CONC}\ \"${OPENGL_LIBRARY}\"")
  endforeach(OPENGL_LIBRARY)
  GM_ADD_CUSTOM_CONFIG( "set(OPENGL_FOUND ${OPENGL_FOUND})" )
  GM_ADD_CUSTOM_CONFIG( "set(OPENGL_INCLUDE_DIRS${OPENGL_INCLUDE_DIRS_CONC})" )
  GM_ADD_CUSTOM_CONFIG( "set(OPENGL_LIBRARIES${OPENGL_LIBRARIES_CONC})" )
endif( OPENGL_FOUND )

# Find GLEW package (local package)
find_package(Glew)
if( GLEW_FOUND )
  foreach(GLEW_INCLUDE_DIR ${GLEW_INCLUDE_DIRS})
    set(GLEW_INCLUDE_DIRS_CONC "${GLEW_INCLUDE_DIRS_CONC}\ \"${GLEW_INCLUDE_DIR}\"")
  endforeach(GLEW_INCLUDE_DIR)
  foreach(GLEW_LIBRARY ${GLEW_LIBRARIES})
    set(GLEW_LIBRARIES_CONC "${GLEW_LIBRARIES_CONC}\ \"${GLEW_LIBRARY}\"")
  endforeach(GLEW_LIBRARY)

  GM_ADD_CUSTOM_CONFIG( "set(GLEW_FOUND ${GLEW_FOUND})" )
  GM_ADD_CUSTOM_CONFIG( "set(GLEW_INCLUDE_DIRS${GLEW_INCLUDE_DIRS_CONC})" )
  GM_ADD_CUSTOM_CONFIG( "set(GLEW_LIBRARIES${GLEW_LIBRARIES_CONC})" )
  include_directories(${GLEW_INCLUDE_DIRS})
endif( GLEW_FOUND )


# Core
add_subdirectory(core)

## OpenGL
add_subdirectory(opengl)
GM_ADD_MODULE_DEPENDENCIES(opengl core)

# Scene
add_subdirectory(scene)
GM_ADD_MODULE_DEPENDENCIES(scene core opengl)

# Parametrics
add_subdirectory(parametrics)     # Don't need dependecies as this is a pure template module
#GM_ADD_MODULE_DEPENDENCIES(parametrics core opengl scene)

# Trianglesystem
add_subdirectory(trianglesystem)  # Don't need dependecies as this is a pure template module
#GM_ADD_MODULE_DEPENDENCIES(trianglesystem core opengl scene)

# Stereolithography
#add_subdirectory(stereolithography)
#GM_ADD_MODULE_DEPENDENCIES(stereolithography core opengl scene)





#################################
## Process each individual module
## addModule( ModuleName [DependModuleName 1] [DependModuleName 2] ... )

#if( GMLIB_TEST )

#  ######################
#  # Add TestUtils module
#  addModule( TestUtils )

#  # Add core module
#  addModule(Core TestUtils)

#  # Add OpenGL module
#  addModule(OpenGL TestUtils Core)

#  # Add Scene module
#  addModule(Scene TestUtils Core OpenGL)

#  # Add Parametrics module
#  addModule(Parametrics TestUtils Core OpenGL Scene)

#  # Add TringleSystem module
#  addModule(TriangleSystem TestUtils Core OpenGL Scene)

#  # Add STereoLithography module
#  addModule(StereoLithography TestUtils Core OpenGL Scene Parametrics TriangleSystem)

#  # Add Script module
#  addModule(Script TestUtils Core)

#  if( OPENCL_ENABLED )

#    # Add OpenCL module
#    addModule(OpenCL TestUtils Core)

#    # Add Wavelet module
#    addModule(Wavelet OpenCL TestUtils Core)

#  endif( OPENCL_ENABLED )

#else( GMLIB_TEST )

#  # Add core module
#  addModule(Core)

#  # Add OpenGL module
#  addModule(OpenGL Core)

#  # Add Scene module
#  addModule(Scene Core OpenGL)

#  # Add Parametrics module
#  addModule(Parametrics Core OpenGL Scene)

#  # Add TringleSystem module
#  addModule(TriangleSystem Core OpenGL Scene)

#  # Add STereoLithography module
#  addModule(StereoLithography Core OpenGL Scene Parametrics TriangleSystem)

#  # Add Script module
#  addModule(Script Core)


#  if( OPENCL_ENABLED )

#    # Add OpenCL module
#    addModule(OpenCL Core)

#    # Add Wavelet module
#    addModule(Wavelet OpenCL Core)

#  endif( OPENCL_ENABLED )

#endif( GMLIB_TEST )




