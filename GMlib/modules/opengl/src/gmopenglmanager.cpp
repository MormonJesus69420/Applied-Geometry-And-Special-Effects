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

#include "gmopenglmanager.h"
#include "gmframebufferobject.h"
#include "gmbufferobject.h"
#include "gmprogrampipeline.h"

// stl
#include <cassert>


namespace GMlib {
namespace GL {


  bool OpenGLManager::_initialized = false;


  void OpenGLManager::init() {

    assert(!_initialized);
    _initialized = true;

    // Init GLEW
    glewExperimental = true;
    GLenum err = glewInit();
    if( err == GLEW_OK )
      std::cout << "GLEW Init OK - using GLEW version " << glewGetString(GLEW_VERSION) << std::endl;
    else
      std::cout << "GLEW Init FAILED!!" << std::endl;
    std::cout << std::flush;

    initSystemWideBuffers();
    initSystemWideShadersAndPrograms();

  }

  void OpenGLManager::cleanUp() {

    //! \todo Implement this... It crashes bad!!
    purgeAllBuffers();
    purgeAllShadersAndPrograms();
  }




  ///////////////////////
  ///
  /// System wide buffers
  ///

  void OpenGLManager::initSystemWideBuffers() {

    // std radius of inscribed circle
    float ir = 0.07;

    // Vertices
    GLfloat cube[] = {

    /* 0 */     -ir,    -ir,    -ir,      // Back/Left/Down
    /* 1 */      ir,    -ir,    -ir,
    /* 2 */      ir,     ir,    -ir,
    /* 3 */     -ir,     ir,    -ir,
    /* 4 */     -ir,    -ir,     ir,      // Front/Left/Down
    /* 5 */      ir,    -ir,     ir,
    /* 6 */      ir,     ir,     ir,
    /* 7 */     -ir,     ir,     ir
    };

    // Indice Coords
    GLushort cube_indices[] = {

      4,  5,  6,  7,    // Front
      1,  2,  6,  5,    // Right
      0,  1,  5,  4,    // Bottom
      0,  3,  2,  1,    // Back
      0,  4,  7,  3,    // Left
      2,  3,  7,  6     // Top
    };

    // Frame indice coords
    GLushort frame_indices [] = {

      0,  1,    // x-axis
      0,  3,    // y-axis
      0,  4,    // z-axis

      // Remaining frame
      2,  3,
      2,  1,
      2,  6,

      7,  6,
      7,  4,
      7,  3,

      5,  4,
      5,  6,
      5,  1

    };


    VertexBufferObject cube_vbo;
    cube_vbo.create("std_rep_cube");
    cube_vbo.setPersistent(true);
    cube_vbo.bufferData( 24 * sizeof(GLfloat), cube, GL_STATIC_DRAW );

    IndexBufferObject cube_ibo;
    cube_ibo.create("std_rep_cube_indices");
    cube_ibo.setPersistent(true);
    cube_ibo.bufferData( 24 * sizeof(GLushort), cube_indices, GL_STATIC_DRAW );

    IndexBufferObject cube_frame_ibo;
    cube_frame_ibo.create("std_rep_frame_indices");
    cube_frame_ibo.setPersistent(true);
    cube_frame_ibo.bufferData( 24 * sizeof(GLushort), frame_indices, GL_STATIC_DRAW );

    UniformBufferObject lights_ubo;
    lights_ubo.create("lights_ubo");
    lights_ubo.setPersistent(true);
  }

  void OpenGLManager::purgeAllBuffers() {

    BufferObject bo_purge_obj;
    bo_purge_obj.purgeAll();

    RenderbufferObject rbo_purge_obj;
    rbo_purge_obj.purgeAll();

    FramebufferObject fbo_purge_obj;
    fbo_purge_obj.purgeAll();

    ProgramPipeline progpipeline_purge_obj;
    progpipeline_purge_obj.purgeAll();

    Texture texture_purge_obj;
    texture_purge_obj.purgeAll();
  }





  ////////////////////////////////////
  ///
  /// System wide Programs and Shaders
  ///

  void OpenGLManager::initSystemWideShadersAndPrograms() {

    initPhongProg();
    initBlinnPhongProg();
    initDirectionalLightingProg();
    initColorProg();


//    initPCurveContoursProg();
    //    initPSurfContours();
  }

  void OpenGLManager::purgeAllShadersAndPrograms() {

    Program program_purge_obj;
    program_purge_obj.purgeAll();

    Shader shader_purge_obj;
    shader_purge_obj.purgeAll();
  }




  void OpenGLManager::initPhongProg() {

    ///////////////
    // Phong shader
    std::string vs_str =
        glslDefHeaderVersionSource() +

        "uniform mat4 u_mvmat, u_mvpmat;\n"
        "\n"
        "in vec4 in_vertex;\n"
        "in vec4 in_normal;\n"
        "\n"
        "out vec4 gl_Position;\n"
        "\n"
        "smooth out vec3 ex_pos;\n"
        "smooth out vec3 ex_normal;\n"
        "\n"
        "void main() {\n"
        "\n"
        "  // Transform the normal to view space\n"
        "  mat3 nmat = inverse( transpose( mat3( u_mvmat ) ) );\n"
        "  ex_normal = nmat * vec3(in_normal);\n"
        "\n"
        "  // Transform position into view space;\n"
        "  vec4 v_pos = u_mvmat * in_vertex;\n"
        "  ex_pos = v_pos.xyz * v_pos.w;\n"
        "\n"
        "  // Compute vertex position\n"
        "  gl_Position = u_mvpmat * in_vertex;\n"
        "}\n"
        ;

    std::string fs_str =
        glslDefHeaderVersionSource() +
        glslFnComputePhongLightingSource() +

        "uniform mat4      u_mvmat;\n"
        "\n"
        "uniform vec4      u_mat_amb;\n"
        "uniform vec4      u_mat_dif;\n"
        "uniform vec4      u_mat_spc;\n"
        "uniform float     u_mat_shi;\n"
        "\n"
        "smooth in vec3    ex_pos;\n"
        "smooth in vec3    ex_normal;\n"
        "\n"
        "out vec4 gl_FragColor;\n"
        "\n"
        "void main() {\n"
        "\n"
        "  vec3 normal = normalize( ex_normal );\n"
        "\n"
        "  Material mat;\n"
        "  mat.ambient   = u_mat_amb;\n"
        "  mat.diffuse   = u_mat_dif;\n"
        "  mat.specular  = u_mat_spc;\n"
        "  mat.shininess = u_mat_shi;\n"
        "\n"
        "  gl_FragColor = computePhongLighting( mat, ex_pos, normal );\n"
        "}\n"
        ;

    VertexShader vs;
    createAndCompilePersistenShader(vs,"phong_vs",vs_str);

    FragmentShader fs;
    createAndCompilePersistenShader(fs,"phong_fs",fs_str);

    Program prog;
    prog.create("phong");
    prog.attachShader( vs);
    prog.attachShader( fs);
    linkPersistentProgram(prog);
  }

  void OpenGLManager::initBlinnPhongProg() {

    /////////////////////
    // Blinn-Phong shader
    std::string vs_str =
        glslDefHeaderVersionSource() +

        "uniform mat4 u_mvmat, u_mvpmat;\n"
        "\n"
        "in vec4 in_vertex;\n"
        "in vec4 in_normal;\n"
        "\n"
        "out vec4 gl_Position;\n"
        "\n"
        "smooth out vec3 ex_pos;\n"
        "smooth out vec3 ex_normal;\n"
        "\n"
        "void main() {\n"
        "\n"
        "  // Transform the normal to view space\n"
        "  mat3 nmat = inverse( transpose( mat3( u_mvmat ) ) );\n"
        "  ex_normal = nmat * vec3(in_normal);\n"
        "\n"
        "  // Transform position into view space;\n"
        "  vec4 v_pos = u_mvmat * in_vertex;\n"
        "  ex_pos = v_pos.xyz * v_pos.w;\n"
        "\n"
        "  // Compute vertex position\n"
        "  gl_Position = u_mvpmat * in_vertex;\n"
        "}\n"
        ;

    std::string fs_str =
        glslDefHeaderVersionSource() +
        glslFnComputeBlinnPhongLightingSource() +

        "uniform mat4      u_mvmat;\n"
        "\n"
        "uniform vec4      u_mat_amb;\n"
        "uniform vec4      u_mat_dif;\n"
        "uniform vec4      u_mat_spc;\n"
        "uniform float     u_mat_shi;\n"
        "\n"
        "smooth in vec3    ex_pos;\n"
        "smooth in vec3    ex_normal;\n"
        "\n"
        "out vec4 gl_FragColor;\n"
        "\n"
        "void main() {\n"
        "\n"
        "  vec3 normal = normalize( ex_normal );\n"
        "\n"
        "  Material mat;\n"
        "  mat.ambient   = u_mat_amb;\n"
        "  mat.diffuse   = u_mat_dif;\n"
        "  mat.specular  = u_mat_spc;\n"
        "  mat.shininess = u_mat_shi;\n"
        "\n"
        "  gl_FragColor = computeBlinnPhongLighting( mat, ex_pos, normal );\n"
        "}\n"
        ;

    VertexShader vs;
    createAndCompilePersistenShader( vs, "blinn_phong_vs", vs_str );

    FragmentShader fs;
    createAndCompilePersistenShader( fs, "blinn_phong_fs", fs_str );

    Program prog;
    prog.create("blinn_phong");
    prog.attachShader( vs);
    prog.attachShader( fs);
    linkPersistentProgram(prog);
  }

  void OpenGLManager::initDirectionalLightingProg() {


    /////////////////////
    // Blinn-Phong shader
    std::string vs_str =
        glslDefHeaderVersionSource() +

        "uniform mat4 u_mvmat, u_mvpmat;\n"
        "\n"
        "in vec4 in_vertex;\n"
        "in vec4 in_normal;\n"
        "\n"
        "out vec4 gl_Position;\n"
        "\n"
        "smooth out vec3 ex_pos;\n"
        "smooth out vec3 ex_normal;\n"
        "\n"
        "void main() {\n"
        "\n"
        "  // Transform the normal to view space\n"
        "  mat3 nmat = inverse( transpose( mat3( u_mvmat ) ) );\n"
        "  ex_normal = nmat * vec3(in_normal);\n"
        "\n"
        "  // Transform position into view space;\n"
        "  vec4 v_pos = u_mvmat * in_vertex;\n"
        "  ex_pos = v_pos.xyz * v_pos.w;\n"
        "\n"
        "  // Compute vertex position\n"
        "  gl_Position = u_mvpmat * in_vertex;\n"
        "}\n"
        ;

    std::string fs_str =
        glslDefHeaderVersionSource() +
        glslStructMaterialSource() +
        glslUniformLightsSource() +
        glslFnDirLightSource() +
        glslFnSpotCutoffFactorSource() +
        glslFnGammaCorrection() +

        "uniform mat4      u_mvmat;\n"
        "\n"
        "uniform vec4      u_mat_amb;\n"
        "uniform vec4      u_mat_dif;\n"
        "uniform vec4      u_mat_spc;\n"
        "uniform float     u_mat_shi;\n"
        "\n"
        "smooth in vec3    ex_pos;\n"
        "smooth in vec3    ex_normal;\n"
        "\n"
        "out vec4 gl_FragColor;\n"
        "\n"
        "void main() {\n"
        "\n"
        "  vec3 normal = normalize( ex_normal );\n"
        "\n"
        "  Material mat;\n"
        "  mat.ambient   = u_mat_amb;\n"
        "  mat.diffuse   = u_mat_dif;\n"
        "  mat.specular  = u_mat_spc;\n"
        "  mat.shininess = u_mat_shi;\n"
        "\n"
        "  vec3 color = vec3(0);\n"
        "\n"
        "  // Compute directional light contribution\n"
        "  for( uint i = uint(0); i < u_directionallights.info.no_lights; ++i )\n"
        "    color += directionalLighting( u_directionallights.lights[i], mat, ex_pos, normal );\n"
        "\n"
        "  gl_FragColor = vec4(correctGamma(color.rgb,2.2),mat.diffuse.a);\n"
        "}\n"
        ;

    VertexShader vs;
    createAndCompilePersistenShader(vs,"directional_lighting_vs", vs_str);

    FragmentShader fs;
    createAndCompilePersistenShader(fs,"directional_lighting_fs", fs_str);

    Program prog;
    prog.create("directional_lighting");
    prog.attachShader( vs);
    prog.attachShader( fs);
    linkPersistentProgram(prog);
  }



//  Program         OpenGLManager::_prog_color;
//  VertexShader    OpenGLManager::_vs_color;
//  FragmentShader  OpenGLManager::_fs_color;

  void OpenGLManager::initColorProg() {

    std::string vs_src =
          glslDefHeaderVersionSource() +

          "uniform mat4 u_mvpmat;\n"
          "\n"
          "in vec4 in_vertex;\n"
          "\n"
          "out vec4 gl_Position;\n"
          "\n"
          "void main() {\n"
          "\n"
          "  gl_Position = u_mvpmat * in_vertex;\n"
          "}\n"
          ;


    std::string fs_src =
          glslDefHeaderVersionSource() +

          "uniform vec4 u_color;\n"
          "\n"
          "out vec4 gl_FragColor;\n"
          "\n"
          "void main() {\n"
          "\n"
          "  gl_FragColor = u_color;\n"
          "}\n"
          ;

    VertexShader vs;
    createAndCompilePersistenShader(vs,"color_vs",vs_src);

    FragmentShader fs;
    createAndCompilePersistenShader(fs,"color_fs",fs_src);

    Program prog;
    prog.create("color");
    prog.attachShader(vs);
    prog.attachShader(fs);
    linkPersistentProgram(prog);
  }




//  Program         OpenGLManager::_prog_pcurve_contours;
//  VertexShader    OpenGLManager::_vs_pcurve_contours;
//  FragmentShader  OpenGLManager::_fs_pcurve_contours;

//  void OpenGLManager::initPCurveContoursProg() {

//    std::string vs_src(
//          "#version 150 core\n"
//          "\n"
//          "uniform mat4 u_mvpmat;\n"
//          "\n"
//          "in vec4 in_vertex;\n"
//          "in vec4 in_color;\n"
//          "\n"
//          "out vec4 ex_color;\n"
//          "out vec4 gl_Position;\n"
//          "\n"
//          "void main() {\n"
//          "\n"
//          "  ex_color = in_color;\n"
//          "  gl_Position = u_mvpmat * in_vertex;\n"
//          "}\n"
//          );

//    std::string fs_src(
//          "#version 150 core\n"
//          "\n"
//          "uniform bool u_selected;\n"
//          "\n"
//          "in vec4 ex_color;\n"
//          "\n"
//          "out vec4 gl_FragColor;\n"
//          "\n"
//          "void main() {\n"
//          "\n"
//          "  gl_FragColor = ex_color;\n"
//          "}\n"
//          );

//  bool compile_ok, link_ok;

//    _vs_pcurve_contours.create("pcurve_contours_vs");
//    _vs_pcurve_contours.setSource(vs_src);
//    compile_ok = _vs_pcurve_contours.compile();
//  assert(compile_ok);

//    _fs_pcurve_contours.create("pcurve_contours_fs");
//    _fs_pcurve_contours.setSource(fs_src);
//    compile_ok = _fs_pcurve_contours.compile();
//  assert(compile_ok);

//    _prog_pcurve_contours.create("pcurve_contours");
//    _prog_pcurve_contours.attachShader(_vs_pcurve_contours);
//    _prog_pcurve_contours.attachShader(_fs_pcurve_contours);
//    link_ok = _prog_pcurve_contours.link();
//  assert(link_ok);
//  }



//  Program         OpenGLManager::_prog_psurf_contours;
//  VertexShader    OpenGLManager::_vs_psurf_contours;
//  FragmentShader  OpenGLManager::_fs_psurf_contours;

//  void OpenGLManager::initPSurfContours() {

//    std::string vs_src;
//    vs_src.append( glslDefHeader150Source() );
//    vs_src.append(
//      "uniform mat4 u_mvmat, u_mvpmat;\n"
//      "\n"
//      "in vec4 in_vertex;\n"
//      "in vec2 in_tex;\n"
//      "\n"
//      "out vec4 gl_Position;\n"
//      "\n"
//      "smooth out vec3 ex_pos;\n"
//      "smooth out vec2 ex_tex;\n"
//      "\n"
//      "void main() {\n"
//      "\n"
//      "  vec4 v_pos = u_mvmat * in_vertex;\n"
//      "  ex_pos = v_pos.xyz * v_pos.w;\n"
//      "\n"
//      "  ex_tex = in_tex;\n"
//      "\n"
//      "  gl_Position = u_mvpmat * in_vertex;\n"
//      "}\n"
//    );

//    std::string fs_src;
//    fs_src.append( glslDefHeader150Source() );
//    fs_src.append( glslStructMaterialSource() );
//    fs_src.append( glslUniformLightsSource() );
//    fs_src.append( glslFnSunlightSource() );
//    fs_src.append( glslFnPointlightSource() );
//    fs_src.append( glslFnSpotlightSource() );
//    fs_src.append( glslFnComputeLightingSource() );
//    fs_src.append(
//      "uniform bool      u_selected;\n"
//      "uniform vec4      u_color;\n"
//      "uniform sampler2D u_nmap;\n"
//      "uniform sampler2D u_du_map;\n"
//      "uniform sampler2D u_dv_map;\n"
//      "uniform mat4      u_mvmat;\n"
//      "\n"
//      "uniform vec4      u_mat_amb;\n"
//      "uniform vec4      u_mat_dif;\n"
//      "uniform vec4      u_mat_spc;\n"
//      "uniform float     u_mat_shi;\n"
//      "\n"
//      "smooth in vec3    ex_pos;\n"
//      "smooth in vec2    ex_tex;\n"
//      "\n"
//      "out vec4 gl_FragColor;\n"
//      "\n"
//      "void main() {\n"
//      "\n"
//      "  mat3 nmat = inverse( transpose( mat3( u_mvmat ) ) );\n"
//      "  vec3 nmap_normal = texture( u_nmap, ex_tex.st).xyz;\n"
//      "  vec3 normal = normalize( nmat * nmap_normal );\n"
//      "\n"
//      "  Material mat;\n"
//      "  mat.ambient   = u_mat_amb;\n"
//      "  mat.diffuse   = u_mat_dif;\n"
//      "  mat.specular  = u_mat_spc;\n"
//      "  mat.shininess = u_mat_shi;\n"
//      "\n"
//      "  vec4 light_color = vec4(0.0);\n"
//      "\n"
//      "  gl_FragColor = computeLighting( mat, normal, ex_pos );\n"
//      "}\n"
//    );

//  bool compile_ok, link_ok;

//    _vs_psurf_contours.create("psurf_contours_vs");
//    _vs_psurf_contours.setSource(vs_src);
//    compile_ok = _vs_psurf_contours.compile();
//  assert(compile_ok);

//    _fs_psurf_contours.create("psurf_contours_fs");
//    _fs_psurf_contours.setSource(fs_src);
//    compile_ok = _fs_psurf_contours.compile();
//  assert(compile_ok);

//    _prog_psurf_contours.create("psurf_contours");
//    _prog_psurf_contours.attachShader(_vs_psurf_contours);
//    _prog_psurf_contours.attachShader(_fs_psurf_contours);
//    link_ok = _prog_psurf_contours.link();
//  assert(link_ok);
//  }




  /////////////////////////
  ///
  /// GLSL Helper functions
  ///

  std::string OpenGLManager::glslDefHeader150CoreSource() {

    return
        "#version 150 core\n"
        "\n"
        "layout(std140) uniform;\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslDefHeader330CoreSource() {

    return
        "#version 330 core \n"
        "\n"
        "layout(std140) uniform;\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslDefHeader330CompatibilitySource() {

    return
        "#version 330 compatibility \n"
        "\n"
        "layout(std140) uniform;\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslDefHeader400CoreSource() {

    return
        "#version 400 core \n"
        "\n"
        "layout(std140) uniform;\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslDefHeader440CoreSource() {

    return
        "#version 440 core \n"
        "\n"
        "layout(std140) uniform;\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslDefHeaderVersionSource() {

    return glslDefHeader330CompatibilitySource();
  }

  std::string OpenGLManager::glslStructMaterialSource() {

    return
        "\n"
        "struct Material {\n"
        "  vec4  ambient;\n"
        "  vec4  diffuse;\n"
        "  vec4  specular;\n"
        "  float shininess;\n"
        "};\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslStructLightSource() {

    return
        "struct LightBase {\n"
        "  vec3  ambient;\n"
        "  vec3  diffuse;\n"
        "  vec3  specular;\n"
        "};\n"
        "\n"
        "struct Attenuation {\n"
        "  float constant;\n"
        "  float linear;\n"
        "  float quadratic;\n"
        "};\n"
        "\n"
        "struct DirectionalLight {\n"
        "  LightBase base;\n"
        "  vec3      direction;\n"
        "};\n"
        "\n"
        "struct PointLight {\n"
        "  LightBase    base;\n"
        "  vec3         position;\n"
        "  Attenuation  attenuation;\n"
        "};\n"
        "\n"
        "struct SpotLight {\n"
        "  PointLight   pointlight; \n"
        "  vec3         direction;\n"
        "  float        cutoff;\n"
        "};\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslUniformLightsSource() {

    return
        glslStructLightSource() +

        "struct LightHeader {\n"
        "  uint no_lights;\n"
        "};\n"

        "uniform DirectionalLights {\n"
        "  LightHeader      info;\n"
        "  DirectionalLight lights[10];\n"
        "} u_directionallights;\n"
        "\n"

        "uniform PointLights {\n"
        "  LightHeader info;\n"
        "  PointLight  lights[50];\n"
        "} u_pointlights;\n"
        "\n"

        "uniform SpotLights {\n"
        "  LightHeader info;\n"
        "  SpotLight   lights[50];\n"
        "} u_spotlights;\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslFnDirLightSource() {

    return
        "vec3 \n"
        "directionalLighting( DirectionalLight light, Material mat, vec3 frag_pos, vec3 N ) { \n"
        "\n"
        "  vec3 light_dir = normalize(-light.direction); \n"
        "  return mat.diffuse.rgb * light.base.ambient * max( dot( light_dir, N ), 0.0 ); \n"
        "}\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslFnPhongLightSource() {

    return
        glslFnLuminosityFromAttenuation() +
        "vec3\n"
        "phongLighting( PointLight light, Material mat, vec3 frag_pos, vec3 N ) { \n"
        "\n"
        "  vec3 light_dir = normalize( light.position- frag_pos ); \n"
        "\n"
        "  float lambertian = max( dot( light_dir, N ), 0.0 ); \n"
        "  float specular = 0.0; \n"
        "\n"
        "  if(lambertian > 0.0) { \n"
        "\n"
        "    vec3 view_dir = normalize(-frag_pos); \n"
        "    vec3 reflect_dir = reflect(-light_dir,N); \n"
        "    float specular_angle = max( dot( reflect_dir, view_dir ), 0.0 ); \n"
        "    specular = pow( specular_angle, mat.shininess/4.0 ); \n"
        "  } \n"
        "\n"
        "  float luminosity = luminosityFromAttenuation(light,frag_pos);\n"
        "\n"
        "  return \n"
        "    luminosity * ("
        "      mat.ambient.rgb * light.base.ambient + \n"
        "      mat.diffuse.rgb * light.base.diffuse * lambertian + \n"
        "      mat.specular.rgb * light.base.specular * specular \n"
        "    ); \n"
        "}\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslFnBlinnPhongLightSource() {

    return
        glslFnLuminosityFromAttenuation() +
        "vec3\n"
        "blinnPhongLighting( PointLight light, Material mat, vec3 frag_pos, vec3 N ) { \n"
        "\n"
        "  vec3 light_dir = normalize( light.position- frag_pos ); \n"
        "\n"
        "  float lambertian = max( dot( light_dir, N ), 0.0 ); \n"
        "  float specular = 0.0; \n"
        "\n"
        "  if(lambertian > 0.0) { \n"
        "\n"
        "    vec3 view_dir = normalize(-frag_pos); \n"
        "    vec3 half_dir = normalize(light_dir + view_dir); \n"
        "    float specular_angle = max( dot( half_dir, N ), 0.0 ); \n"
        "    specular = pow( specular_angle, mat.shininess ); \n"
        "  } \n"
        "\n"
        "\n"
        "  float luminosity = luminosityFromAttenuation(light,frag_pos);\n"
        "\n"
        "  return \n"
        "    luminosity * ("
        "      mat.ambient.rgb * light.base.ambient + \n"
        "      mat.diffuse.rgb * light.base.diffuse * lambertian + \n"
        "      mat.specular.rgb * light.base.specular * specular \n"
        "    ); \n"
        "}\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslFnSpotCutoffFactorSource() {

    return
        "float\n"
        "spotCutoffFactor(SpotLight light, vec3 frag_pos) {\n"
        "\n"
        "  vec3 light_to_frag = normalize(frag_pos - light.pointlight.position); \n"
        "  vec3 spotlight_dir = normalize(light.direction); \n"
        "\n"
        "  float spot_fact = clamp( dot( light_to_frag, spotlight_dir ), 0.0, 1.0); \n"
        "\n"
        "  float cutoff_fact = 0.0; \n"
        "\n"
        "  if( spot_fact > light.cutoff ) {\n"
        "    cutoff_fact = (spot_fact - light.cutoff) / (1.0 - light.cutoff); \n"
        "  }\n"
        "\n"
        "  return cutoff_fact; \n"
        "}\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslFnLuminosityFromAttenuation() {

    return
        "float\n"
        "luminosityFromAttenuation(PointLight light, vec3 frag_pos) {\n"
        "\n"
        "  float dist = length( light.position- frag_pos ); \n"
        "\n"
        "  float constant  = light.attenuation.constant; \n"
        "  float linear    = light.attenuation.linear; \n"
        "  float quadratic = light.attenuation.quadratic; \n"
        "\n"
        "  return \n"
        "    constant + "
        "    linear * 1.0/dist +"
        "    quadratic * 1.0/(dist*dist);"
        "}\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslFnGammaCorrection() {

    return
        "vec3\n"
        "correctGamma(vec3 linear_color, float screen_gamma) {\n"
        "\n"
        "  return pow(linear_color, vec3(1.0/screen_gamma)); \n"
        "}\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslFnComputePhongLightingSource() {

    return
        glslStructMaterialSource() +
        glslUniformLightsSource() +
        glslFnDirLightSource() +
        glslFnPhongLightSource() +
        glslFnSpotCutoffFactorSource() +
        glslFnGammaCorrection() +

        "vec4\n"
        "computePhongLighting(Material mat, vec3 pos, vec3 normal ) {\n"
        "\n"
        "  vec3 color = vec3(0);\n"
        "\n"
        "  // Compute directional light contribution\n"
        "  for( uint i = uint(0); i < u_directionallights.info.no_lights; ++i )\n"
        "    color += directionalLighting( u_directionallights.lights[i], mat, pos, normal );\n"
        "\n"
        "  // Compute pointlight contribution\n"
        "  for( uint i = uint(0); i < u_pointlights.info.no_lights; ++i )\n"
        "    color += phongLighting( u_pointlights.lights[i], mat, pos, normal );\n"
        "\n"
        "  // Compute spotlight contribution\n"
        "  for( uint i = uint(0); i < u_spotlights.info.no_lights; ++i )\n"
        "    color += phongLighting( u_spotlights.lights[i].pointlight, mat, pos, normal) * spotCutoffFactor( u_spotlights.lights[i], pos ); \n"
        "\n"
        "  return vec4(correctGamma(color.rgb,2.2),mat.diffuse.a);\n"
        "}\n"
        "\n"
        ;
  }

  std::string OpenGLManager::glslFnComputeBlinnPhongLightingSource() {

    return
        glslStructMaterialSource() +
        glslUniformLightsSource() +
        glslFnDirLightSource() +
        glslFnBlinnPhongLightSource() +
        glslFnSpotCutoffFactorSource() +
        glslFnGammaCorrection() +

        "vec4\n"
        "computeBlinnPhongLighting(Material mat, vec3 pos, vec3 normal ) {\n"
        "\n"
        "  vec3 color = vec3(0);\n"
        "\n"
        "  // Compute directional light contribution\n"
        "  for( uint i = uint(0); i < u_directionallights.info.no_lights; ++i )\n"
        "    color += directionalLighting( u_directionallights.lights[i], mat, pos, normal );\n"
        "\n"
        "  // Compute pointlight contribution\n"
        "  for( uint i = uint(0); i < u_pointlights.info.no_lights; ++i )\n"
        "    color += blinnPhongLighting( u_pointlights.lights[i], mat, pos, normal );\n"
        "\n"
        "  // Compute spotlight contribution\n"
        "  for( uint i = uint(0); i < u_spotlights.info.no_lights; ++i )\n"
        "    color += blinnPhongLighting( u_spotlights.lights[i].pointlight, mat, pos, normal ) * spotCutoffFactor( u_spotlights.lights[i], pos ); \n"
        "\n"
        "  return vec4(correctGamma(color.rgb,2.2),mat.diffuse.a);\n"
        "}\n"
        "\n"
        ;
  }


} // END namespace GL
} // END namespace GMlib
