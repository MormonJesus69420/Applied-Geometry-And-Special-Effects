/**********************************************************************************
**
** Copyright (C) 1994 Narvik University College
** Contact: GMlib Online Portal at http://episteme.hin.no
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/



#ifndef GM_OPENGL_UTILS_UTILS_H
#define GM_OPENGL_UTILS_UTILS_H


// stl
#include <iostream>
#include <cassert>

namespace GMlib {

namespace GL {

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // Code curtesy of Stackoverflow
  // http://stackoverflow.com/questions/11256470/define-a-macro-to-facilitate-opengl-command-debugging
  //
  // Some modifications have been made to accomodate GMlib

  inline
  void CheckOpenGLError(const char* stmt, const char* fname, int line)
  {
      GLenum err = glGetError();
      if (err != GL_NO_ERROR)
      {
          std::cout << "OpenGL error " << err << ", at " << fname << ":" << line << " - for "
                    << stmt << "\n" << "\t Str: " << gluErrorString(err) << std::endl;

          assert(err == GL_NO_ERROR);
      }
  }


  #ifdef GM_GL_DEBUG
      #define GL_CHECK(stmt) do { \
              stmt; \
              GMlib::GL::CheckOpenGLError(#stmt, __FILE__, __LINE__); \
          } while (0)
  #else
      #define GL_CHECK(stmt) stmt
  #endif

  /////////////////////////////////////////////////////////////////////////////////////////////////////



} // END namespace GMutils

} // END namespace GMlib


#endif // GM_OPENGL_UTILS_UTILS_H
