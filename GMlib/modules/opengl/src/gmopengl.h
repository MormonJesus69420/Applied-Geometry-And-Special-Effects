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



#ifndef GM_OPENGL_OPENGL_H
#define GM_OPENGL_OPENGL_H


//#if defined(_WIN32)
////  To avoid including <windows.h> to avoid name space pollution,
////	but Win32's <GL/gl.h> needs APIENTRY and WINGDIAPI defined properly.
//# if 1
//#  define  WIN32_LEAN_AND_MEAN
//#  include <windows.h>
//# else
//   // XXX This is from Win32's <windef.h>
//#  ifndef APIENTRY
//#   define GLUT_APIENTRY_DEFINED
//#   if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
//#    define APIENTRY    __stdcall
//#   else
//#    define APIENTRY
//#   endif
//#  endif
//   // XXX This is from Win32's <winnt.h>
//#  ifndef CALLBACK
//#   if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
//#    define CALLBACK __stdcall
//#   else
//#    define CALLBACK
//#   endif
//#  endif
//   // XXX This is from Win32's <wingdi.h> and <winnt.h>
//#  ifndef WINGDIAPI
//#   define GLUT_WINGDIAPI_DEFINED
//#   define WINGDIAPI __declspec(dllimport)
//#  endif
//   // XXX This is from Win32's <ctype.h>
//#  ifndef _WCHAR_T_DEFINED
//typedef unsigned short wchar_t;
//#   define _WCHAR_T_DEFINED
//#  endif
//# endif
//#endif




// gmlib
#include <core/containers/gmdmatrix.h>
#include <core/types/gmpoint.h>
#include <core/types/gmmatrix.h>
#include <core/utils/gmcolor.h>
#include <core/utils/gmstream.h>

// Include Glew header and leave the gl mangling to GLEW
#include <GL/glew.h>

// local
#include "utils/gmutils.h"

// STL
#include <vector>
#include <map>
#include <string>
#include <iostream>



namespace GMlib {

namespace GL {

  /***********
   * Constants
   */
  const unsigned int GM_TEXTURE_1D = GL_TEXTURE_1D;
  const unsigned int GM_TEXTURE_2D = GL_TEXTURE_2D;

  const unsigned int GM_OBJECT_LINEAR = GL_OBJECT_LINEAR;
  const unsigned int GM_EYE_LINEAR = GL_EYE_LINEAR;
  const unsigned int GM_SPHERE_MAP = GL_SPHERE_MAP;



  inline
  void glClearColor( const Color& c ) {

    GL_CHECK(::glClearColor( GLclampf(c.getRedC()), GLclampf(c.getGreenC()), GLclampf(c.getBlueC()), GLclampf(c.getAlphaC()) ));
  }



  // ***********
  // SwapBuffers

  void swapBuffers(void);






  struct GLuintType {
    explicit GLuintType() {}
    explicit GLuintType( GLuint uint_data ) : data(uint_data) {}

    const GLuint& operator() () const { return data; }
    const GLuintType& operator = ( const GLuint& data_in ) { data = data_in; return *this; }

    GLuint data;
  };

//  struct AttribLocation : GLuintType {
//    explicit AttribLocation() {}
//    AttribLocation( GLuint location_id ) : GLuintType(location_id) {}
//  };


  typedef GLuintType    AttributeLocation;
  typedef GLuintType    UniformLocation;
  typedef GLuintType    UniformBlockIndex;





  struct GLViewVertex {
    GLclampf x, y;
  };

  struct GLVertex {
    GLfloat x, y, z;
  };

  struct GLNormal {
    GLfloat nx, ny, nz;
  };

  struct GLTex1D {
    GLfloat s;
  };

  struct GLTex2D : GLTex1D {
    GLfloat t;
  };

  struct GLTex3D : GLTex2D {
    GLfloat r;
  };

  struct GLVertexNormal : GLVertex, GLNormal {};
  struct GLVertexNormalTex2D : GLVertex, GLNormal, GLTex2D {};
  struct GLVertexTex1D : GLVertex, GLTex1D {};
  struct GLVertexTex2D : GLVertex, GLTex2D {};

  template <int n,typename T = GLfloat>
  struct GLVector {
    T p[n];
  };







  struct GLMaterial {

    GLVector<4>   amb;
    GLVector<4>   dif;
    GLVector<4>   spc;

    GLfloat       shininess;
    GLfloat       _padding[3];
  };










  // Structs modeled to reflect internal shader structs used with GLSL uniform layout std140
  // Complete spesification: https://www.opengl.org/registry/specs/ARB/uniform_buffer_object.txt
  // Section 2.15.3.1.2
  //
  // N = size of base unit (uint|int|float)

  struct GLLightHeader {      //  4 N
    GLuint    no_lights;
    GLuint    _pad_2;
    GLuint    _pad_3;
    GLuint    _pad_4;
  };

  struct GLLightBase {        // 12 N
    GLVector<3>   amb;
    GLfloat       _pad_amb_4;
    GLVector<3>   dif;
    GLfloat       _pad_dif_4;
    GLVector<3>   spc;
    GLfloat       _pad_spc_4;
  };

  struct GLAttenuation {      //  4 N
    GLfloat       constant;
    GLfloat       linear;
    GLfloat       quadratic;
    GLfloat       _pad_4;
  };

  struct GLDirectionalLight { // 16 N

    GLLightBase     base;
    GLVector<3>     dir;
    GLfloat         _dir_pad_4;
  };


  struct GLPointLight {       // 20 N

    GLLightBase     base;
    GLVector<3>     pos;
    GLfloat         _pos_pad_4;
    GLAttenuation   attenuation;
  };

  struct GLSpotLight {        // 24 N

    GLPointLight    pointlight;

    GLVector<3>     dir;
    GLfloat         cutoff;
  };










  typedef std::map< GLuint, int > GLuintCMap;



} // END namespace GL

} // END namespace GMlib


#endif // GM_OPENGL_OPENGL_H
