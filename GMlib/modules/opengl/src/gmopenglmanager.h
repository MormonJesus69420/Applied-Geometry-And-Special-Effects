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



#ifndef GM_OPENGL_OPENGLMANAGER_H
#define GM_OPENGL_OPENGLMANAGER_H



#include "gmopengl.h"

#include "bufferobjects/gmvertexbufferobject.h"
#include "bufferobjects/gmindexbufferobject.h"
#include "bufferobjects/gmuniformbufferobject.h"

#include "gmprogram.h"
#include "shaders/gmvertexshader.h"
#include "shaders/gmfragmentshader.h"

namespace GMlib {
namespace GL {



  class OpenGLManager {

  public:

    // init functions
    static void                 init();
    static void                 cleanUp();

    // GLSL snipet functions
    static std::string            glslDefHeader150CoreSource();
    static std::string            glslDefHeader330CoreSource();
    static std::string            glslDefHeader330CompatibilitySource();
    static std::string            glslDefHeader400CoreSource();
    static std::string            glslDefHeader440CoreSource();
    static std::string            glslDefHeaderVersionSource();
    static std::string            glslStructMaterialSource();
    static std::string            glslStructLightSource();
    static std::string            glslUniformLightsSource();
    static std::string            glslFnDirLightSource();
    static std::string            glslFnPhongLightSource();
    static std::string            glslFnBlinnPhongLightSource();
    static std::string            glslFnSpotCutoffFactorSource();
    static std::string            glslFnLuminosityFromAttenuation();
    static std::string            glslFnGammaCorrection();
    static std::string            glslFnComputePhongLightingSource();
    static std::string            glslFnComputeBlinnPhongLightingSource();

  private:
    static bool                 _initialized;

    // System wide buffers
    static void                   initSystemWideBuffers();
    static void                   purgeAllBuffers();

    // Programs and shaders
    static void                   initSystemWideShadersAndPrograms();
    static void                   purgeAllShadersAndPrograms();

    // System wide programs/shaders
    static void                   initPhongProg();
    static void                   initBlinnPhongProg();
    static void                   initDirectionalLightingProg();
    static void                   initColorProg();

//    // "PCurve: Contours" program
//    static Program                _prog_pcurve_contours;
//    static VertexShader           _vs_pcurve_contours;
//    static FragmentShader         _fs_pcurve_contours;
//    static void                   initPCurveContoursProg();


//    // "PSurf: Contours" program
//    static Program                _prog_psurf_contours;
//    static VertexShader           _vs_psurf_contours;
//    static FragmentShader         _fs_psurf_contours;
//    static void                   initPSurfContours();


  public:

    template <typename T>
    static void
    fillLightUBO( GL::UniformBufferObject& ubo, const GL::GLLightHeader& header, const std::vector<T>& data ) {

      ubo.bufferData( sizeof(GL::GLLightHeader) + data.size() * sizeof(T), 0x0, GL_DYNAMIC_DRAW );
      ubo.bufferSubData( 0, sizeof(GL::GLLightHeader), &header );
      ubo.bufferSubData( sizeof(GL::GLLightHeader), data.size() * sizeof(T), data.data() );
    }

    static void
    fillGLLightBase( GL::GLLightBase& light,
                     const Color& amb, const Color& dif, const Color& spc ) {

      light.amb.p[0] = amb.getRedC();
      light.amb.p[1] = amb.getGreenC();
      light.amb.p[2] = amb.getBlueC();

      light.dif.p[0] = dif.getRedC();
      light.dif.p[1] = dif.getGreenC();
      light.dif.p[2] = dif.getBlueC();

      light.spc.p[0] = spc.getRedC();
      light.spc.p[1] = spc.getGreenC();
      light.spc.p[2] = spc.getBlueC();
    }

    static void
    fillGLDirectionalLight( GL::GLDirectionalLight& light,
                            const Color& amb, const Color& dif, const Color& spc,
                            const Vector<float,3>& dir ) {

      fillGLLightBase(light.base, amb, dif, spc);
      light.dir.p[0] = dir(0);
      light.dir.p[1] = dir(1);
      light.dir.p[2] = dir(2);
    }

    static void
    fillGLPointLight( GL::GLPointLight& light,
                      const Color& amb, const Color& dif, const Color& spc,
                      const Point<float,3>& pos, const Point<float,3>& att ) {

      fillGLLightBase(light.base,amb,dif,spc);
      light.pos.p[0] = pos(0);
      light.pos.p[1] = pos(1);
      light.pos.p[2] = pos(2);
      light.attenuation.constant  = att(0);
      light.attenuation.linear    = att(1);
      light.attenuation.quadratic = att(2);
    }

    static void
    fillGLSpotLight( GL::GLSpotLight& light,
                     const Color& amb, const Color& dif, const Color& spc,
                     const Point<float,3>& pos, const Point<float,3>& att,
                     const Vector<float,3>& dir, float cutoff ) {

      fillGLPointLight( light.pointlight, amb, dif, spc, pos, att );
      light.dir.p[0] = dir(0);
      light.dir.p[1] = dir(1);
      light.dir.p[2] = dir(2);
      light.cutoff   = cutoff;
    }

  private:

    template <typename Shader_T>
    static void
    createAndCompilePersistenShader( Shader_T& shader, const std::string& name, const std::string& source ) {

      shader.create(name);
      shader.setPersistent(true);
      shader.setSource(source);
      bool compile_ok = shader.compile();
      if( !compile_ok ) {
        std::cout << "Src:" << std::endl << shader.getSource() << std::endl << std::endl;
        std::cout << "Error: " << shader.getCompilerLog() << std::endl;
      }
      assert(compile_ok);
    }

    template <typename Program_T>
    static void
    linkPersistentProgram( Program_T& program ) {

      program.setPersistent(true);
      bool link_ok = program.link();
      if( !link_ok ) {
        std::cout << "Error: " << program.getLinkerLog() << std::endl;
      }
      assert(link_ok);
    }


  }; // END class OpenGL



} // END namespace GL
} // END namespace GMlib




#endif // GM_OPENGL_OPENGLMANAGER_H
