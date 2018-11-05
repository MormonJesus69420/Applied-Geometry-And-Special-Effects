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



#include "gmdefaultrenderer.h"

// local
#include "../camera/gmcamera.h"
#include "../light/gmspotlight.h"
#include "../light/gmpointlight.h"
#include "../light/gmsun.h"
#include "rendertargets/gmnativerendertarget.h"

// gmlib
#include <opengl/gmopenglmanager.h>
#include <opengl/shaders/gmvertexshader.h>
#include <opengl/shaders/gmfragmentshader.h>

//stl
#include <cassert>



namespace GMlib {




  DefaultRenderer::DefaultRenderer() : _select_color(GMcolor::beige()) {


    // Acquire programs
    initRenderProgram();
    initRenderSelectProgram();

    // Create render targets
    _front_rt = new NativeRenderTarget;
    _back_rt = new NativeRenderTarget;
    setClearColor(GMcolor::grey());

    // Create buffers
    _fbo.create();

    _rbo_color.create(GL_TEXTURE_2D);
    _rbo_depth.create( GL_TEXTURE_2D);

    _fbo_select.create();
    _fbo_select_depth.create();

    _rbo_select.create(GL_TEXTURE_2D);
    _rbo_select_depth.create(GL_TEXTURE_2D);

    // Color rbo texture
    _rbo_color.texParameteri( GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    _rbo_color.texParameteri( GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _rbo_color.texParameterf( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    _rbo_color.texParameterf( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Depth rbo texture
    _rbo_depth.texParameteri( GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    _rbo_depth.texParameteri( GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _rbo_depth.texParameterf( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    _rbo_depth.texParameterf( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Select rbo texture
    _rbo_select.texParameteri( GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    _rbo_select.texParameteri( GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _rbo_select.texParameterf( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    _rbo_select.texParameterf( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Select depth rbo texture
    _rbo_select_depth.texParameteri( GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    _rbo_select_depth.texParameteri( GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _rbo_select_depth.texParameterf( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    _rbo_select_depth.texParameterf( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Bind renderbuffers to framebuffer.
    _fbo.attachTexture2D( _rbo_color,  GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 );
    _fbo.attachTexture2D( _rbo_depth, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT );

    // Bind select renderbuffer to select framebuffer
    _fbo_select.attachTexture2D( _rbo_select, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 );
    _fbo_select.attachTexture2D( _rbo_select_depth, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT );

    // Bind depth buffer to depthbuffer framebuffer
    _fbo_select_depth.attachTexture2D( _rbo_select_depth, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT );


    _coord_sys_visu = new CoordSysRepVisualizer;

    if(!_dirlight_ubo.isValid()) _dirlight_ubo.create();
    if(!_pointlight_ubo.isValid()) _pointlight_ubo.create();
    if(!_spotlight_ubo.isValid()) _spotlight_ubo.create();


    // Render quad

    DVector< GL::GLVertexTex2D > data(4);
    data[0].x = 0.0f; data[0].y = 0.0f; data[0].z = 0.0f;
    data[0].s = 0.0f; data[0].t = 0.0f;

    data[1].x = 0.0f; data[1].y = 1.0f; data[1].z = 0.0f;
    data[1].s = 0.0f; data[1].t = 1.0f;

    data[2].x = 1.0f; data[2].y = 1.0f; data[2].z = 0.0f;
    data[2].s = 1.0f; data[2].t = 1.0f;

    data[3].x = 1.0f; data[3].y = 0.0f; data[3].z = 0.0f;
    data[3].s = 1.0f; data[3].t = 0.0f;

    _quad_vbo.create();
    _quad_vbo.bufferData( 4 * sizeof(GL::GLVertexTex2D),
                          data.getPtr(), GL_STATIC_DRAW );


    // Build an orthogonal matrix
    float near_plane = -1.0f;
    float far_plane = 1.0f;

    float l, r, b, t, n, f;
    l = 0.0f;
    r = 1.0f;
    b = 0.0f;
    t = 1.0f;
    n = near_plane;
    f = far_plane;

    float A, B, C;
    A = - ( r + l ) / ( r - l );
    B = - ( t + b ) / ( t - b );
    C = - ( f + n ) / ( f - n );

    _ortho_mat[0][0] = 2 / (r - l);
    _ortho_mat[0][1] = 0.0f;
    _ortho_mat[0][2] = 0.0f;
    _ortho_mat[0][3] = A;

    _ortho_mat[1][0] = 0.0f;
    _ortho_mat[1][1] = 2 / ( t - b );
    _ortho_mat[1][2] = 0.0f;
    _ortho_mat[1][3] = B;

    _ortho_mat[2][0] = 0.0f;
    _ortho_mat[2][1] = 0.0f;
    _ortho_mat[2][2] = - 2.0f / (f-n);
    _ortho_mat[2][3] = C;

    _ortho_mat[3][0] = 0.0f;
    _ortho_mat[3][1] = 0.0f;
    _ortho_mat[3][2] = 0.0f;
    _ortho_mat[3][3] = 1.0f;
  }

  DefaultRenderer::~DefaultRenderer() {

    delete _coord_sys_visu;
    delete _front_rt;
    delete _back_rt;
  }

  void DefaultRenderer::render()  {

    render(*_back_rt);
  }

  void DefaultRenderer::render(RenderTarget& target) {

    // Update lights
    updateLightUBO();
    getCamera()->updateCameraOrientation();

    // Prepare
    prepare(getCamera());

    // Render scene
    renderScene();

    // Set render to target
    target.prepare();
    target.bind();
    renderToTarget();
    target.unbind();
  }

  void DefaultRenderer::swap() {  std::swap(_back_rt, _front_rt); }

  void DefaultRenderer::initRenderProgram() {


    const std::string prog_name    = "render_prog";
    if( _render_prog.acquire(prog_name) ) return;


    std::string vs_src(
          "#version 150 core\n"
          "\n"
          "uniform mat4 u_mvpmat;\n"
          "\n"
          "in vec4 in_vertex;\n"
          "in vec2 in_tex_coord;\n"
          "\n"
          "out vec2 ex_tex_coord;\n"
          "out vec4 gl_Position;\n"
          "\n"
          "void main() {\n"
          "\n"
          "  ex_tex_coord = in_tex_coord;\n"
          "  gl_Position = u_mvpmat * in_vertex;\n"
          "}\n"
          );

    std::string fs_src(
          "#version 150 core\n"
          "\n"
          "uniform sampler2D u_tex;\n"
          "uniform sampler2D u_tex_selected;\n"
          "uniform float u_buf_w;\n"
          "uniform float u_buf_h;\n"
          "uniform vec4 u_select_color;\n"
          "\n"
          "in vec2 ex_tex_coord;\n"
          "\n"
          "out vec4 gl_FragColor;\n"
          "\n"
          "float avg( vec4 v ) {\n"
          "\n"
          "  return ( v.r + v.g + v. b ) / 4.0;\n"
          "}\n"
          "\n"
          "\n"
          "float runFilter( mat3 mask, float w, float h ) {\n"
          "\n"
          "  float result = 0; //vec4( 0.0, 0.0, 0.0, 0.0 );\n"
          "\n"
          "  for( float i = -1.0; i <= 1.0; i = i + 1.0 ) {\n"
          "    for( float j = -1.0; j <= 1.0; j = j + 1.0 ) {\n"
          "\n"
          "      float x = ex_tex_coord.x + ( (i + 0.5) / w );\n"
          "      float y = ex_tex_coord.y + ( (j + 0.5) / h );\n"
          "\n"
          "      float sub_res = avg( texture2D( u_tex_selected, vec2( x, y ) ) );\n"
          "\n"
          "      result += sub_res * avg(\n"
          "        vec4(\n"
          "          mask[int(i+1.0)][int(j+1.0)],\n"
          "          mask[int(i+1.0)][int(j+1.0)],\n"
          "          mask[int(i+1.0)][int(j+1.0)],\n"
          "          mask[int(i+1.0)][int(j+1.0)]\n"
          "        )\n"
          "        );\n"
          "    }\n"
          "  }\n"
          "\n"
          "  return result;\n"
          "}\n"
          "\n"
          "void main() {\n"
          "\n"
          "  // x and y sobel filters\n"
          "  mat3 mask_x = mat3(\n"
          "    -1.0, 0.0, 1.0,\n"
          "    -2.0, 0.0, 2.0,\n"
          "    -1.0, 0.0, 1.0\n"
          "    );\n"
          "\n"
          "  mat3 mask_y =  mat3(\n"
          "     1.0,  2.0,  1.0,\n"
          "     0.0,  0.0,  0.0,\n"
          "    -1.0, -2.0, -1.0\n"
          "    );\n"
          "\n"
          "  // summation vars.\n"
          "  float res_x = runFilter( mask_x, u_buf_w, u_buf_h );\n"
          "  float res_y = runFilter( mask_y, u_buf_w, u_buf_h );\n"
          "  float res = sqrt( res_x * res_x + res_y * res_y );\n"
          "\n"
          "  gl_FragColor = texture( u_tex, ex_tex_coord );\n"
          "\n"
          "  if( res > 0.0 )\n"
          "   gl_FragColor = u_select_color;\n"
          "\n"
//          "  float bah = texture( u_tex_selected, ex_tex_coord ).b; \n"
//          "  if( bah > 0.0 )\n"
//          "   gl_FragColor = vec4( bah, 0.0, 0.0, 1.0 );\n"
//          "   gl_FragColor = vec4( res, 0.0, 0.0, 1.0 );\n"
//          "   gl_FragColor = vec4( res_y, 0.0, 0.0, 1.0 );\n"
//          "   gl_FragColor = vec4( 0.0, res_y, 0.0, 1.0 );\n"
          "}\n"
          );

    bool compile_ok, link_ok;

    GL::VertexShader vshader;
    vshader.create("render_vs");
    vshader.setPersistent(true);
    vshader.setSource(vs_src);
    compile_ok = vshader.compile();
    if( !compile_ok ) {
      std::cout << "Src:" << std::endl << vshader.getSource() << std::endl << std::endl;
      std::cout << "Error: " << vshader.getCompilerLog() << std::endl;
    }
    assert(compile_ok);

    GL::FragmentShader fshader;
    fshader.create("render_fs");
    fshader.setPersistent(true);
    fshader.setSource(fs_src);
    compile_ok = fshader.compile();
    if( !compile_ok ) {
      std::cout << "Src:" << std::endl << fshader.getSource() << std::endl << std::endl;
      std::cout << "Error: " << fshader.getCompilerLog() << std::endl;
    }
    assert(compile_ok);

    _render_prog.create(prog_name);
    _render_prog.setPersistent(true);
    _render_prog.attachShader(vshader);
    _render_prog.attachShader(fshader);
    link_ok = _render_prog.link();
    if( !link_ok ) {
      std::cout << "Error: " << _render_prog.getLinkerLog() << std::endl;
    }
    assert(link_ok);
  }

  void DefaultRenderer::initRenderSelectProgram() {

    const std::string prog_name    = "render_select_prog";
    if( _render_select_prog.acquire(prog_name) ) return;


    std::string vs_src(
          "#version 150 compatibility \n"
          "uniform mat4 u_mvpmat; \n"
          "\n"
          "in vec4 in_vertex; \n"
          "\n"
          "out vec4 gl_Position; \n"
          "\n"
          "void main() \n"
          "{ \n"
          "  gl_Position = u_mvpmat * in_vertex; \n"
          "}\n"
          );

    std::string fs_src(
          "#version 150 compatibility \n"
          "\n"
          "out vec4 gl_FragColor; \n"
          "\n"
          "void main() \n"
          "{ \n"
          "  gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 ); \n"
          "} \n"
          );

    bool compile_ok, link_ok;

    GL::VertexShader vshader;
    vshader.create("render_select_vs");
    vshader.setPersistent(true);
    vshader.setSource(vs_src);
    compile_ok = vshader.compile();
    if( !compile_ok ) {
      std::cout << "Src:" << std::endl << vshader.getSource() << std::endl << std::endl;
      std::cout << "Error: " << vshader.getCompilerLog() << std::endl;
    }
    assert(compile_ok);

    GL::FragmentShader fshader;
    fshader.create("render_select_fs");
    fshader.setPersistent(true);
    fshader.setSource(fs_src);
    compile_ok = fshader.compile();
    if( !compile_ok ) {
      std::cout << "Src:" << std::endl << fshader.getSource() << std::endl << std::endl;
      std::cout << "Error: " << fshader.getCompilerLog() << std::endl;
    }
    assert(compile_ok);

    _render_select_prog.create(prog_name);
    _render_select_prog.setPersistent(true);
    _render_select_prog.attachShader(vshader);
    _render_select_prog.attachShader(fshader);
    link_ok = _render_select_prog.link();
    if( !link_ok ) {
      std::cout << "Error: " << _render_select_prog.getLinkerLog() << std::endl;
    }
    assert(link_ok);

  }

//  void DisplayRenderer::reshape(int x, int y, int w, int h) {

//    Renderer::setViewport(x,y,w,h);

//    _x = x;
//    _y = y;
//    _w = w;
//    _h = h;

//    _rbo_color.texImage2D( 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0x0 );
//    _rbo_depth.texImage2D( 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0x0 );

//    _rbo_select.texImage2D( 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0x0 );
//    _rbo_select_depth.texImage2D( 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0x0 );
//  }


  void DefaultRenderer::prepare(Camera *cam) {

    Scene *scene = cam->getScene();
    assert(scene);

    // Get displayable objects
    _objs.resetSize();
    scene->getRenderList( _objs, cam );
  }


  const GL::Texture& DefaultRenderer::getRenderTexture() const {

    return _rbo_color;
  }

  const GL::Texture& DefaultRenderer::getSelectTexture() const {

    return _rbo_select;
  }

  const Color&DefaultRenderer::getClearColor() const {

    return _clear_color;
  }

  void DefaultRenderer::setClearColor(const Color& color) {

    _clear_color = color;
//    if(_back_rt)  _back_rt->setClearColor(color);
//    if(_front_rt) _front_rt->setClearColor(color);
  }

  const Color&DefaultRenderer::getSelectColor() const {

    return _select_color;
  }

  void DefaultRenderer::setSelectColor(const Color& color) {

    _select_color = color;
  }

  const Vector<int,2>& DefaultRenderer::getViewportSize() const {

    return _size;
  }



  void DefaultRenderer::render(const SceneObject* obj) const {

    if( obj != getCamera() && obj->isVisible() ) {

      if(obj->isCollapsed()) {

        VisualizerStdRep::getInstance()->render(obj,this);
      }
      else {

        const Array<Visualizer*>& visus = obj->getVisualizers();
        for( int i = 0; i < visus.getSize(); ++i ) {

          visus(i)->render(obj,this);
        }

        obj->localDisplay(this);
      }
    }
  }

  void DefaultRenderer::renderSelectedGeometry( const SceneObject* obj) const {

    const Color sel_true_color = GMcolor::white();

    if( obj != getCamera() && obj->isSelected() && obj->isVisible()  ) {

      if(obj->isCollapsed()) {

        VisualizerStdRep::getInstance()->renderGeometry(obj,this,sel_true_color);
      }
      else {

        const Array<Visualizer*>& visus = obj->getVisualizers();
        for( int i = 0; i < visus.getSize(); ++i )
          visus(i)->renderGeometry(obj,this,sel_true_color);

        obj->localSelect(this,sel_true_color);
      }
    }
  }

  void DefaultRenderer::renderCoordSys() const {

    _coord_sys_visu->render(getCamera(),this);
  }



  void DefaultRenderer::renderToTarget() {




//    GL_CHECK(::glViewport(0,0,_size(0),_size(1)));

    GL_CHECK(::glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ));
    GL_CHECK(::glDisable(GL_DEPTH_TEST));

    // Draw scene composition
    {
      _render_prog.bind();

      _render_prog.uniform( "u_mvpmat", _ortho_mat );
      _render_prog.uniform( "u_tex", getRenderTexture(), static_cast<GLenum>(GL_TEXTURE0), 0 );
      _render_prog.uniform( "u_tex_selected", getSelectTexture(), static_cast<GLenum>(GL_TEXTURE1), 1 );
      _render_prog.uniform( "u_buf_w", static_cast<float>(_size(0)) );
      _render_prog.uniform( "u_buf_h", static_cast<float>(_size(1)) );
      _render_prog.uniform( "u_select_color", _select_color );

      GL::AttributeLocation vert_loc = _render_prog.getAttributeLocation( "in_vertex" );
      GL::AttributeLocation tex_coord_loc = _render_prog.getAttributeLocation( "in_tex_coord" );

      _quad_vbo.bind();

      _quad_vbo.enable( vert_loc,      3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexTex2D), reinterpret_cast<const GLvoid*>(0x0) );
      _quad_vbo.enable( tex_coord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexTex2D), reinterpret_cast<const GLvoid*>(3*sizeof(GLfloat)) );

      GL_CHECK(::glDrawArrays( GL_QUADS, 0, 4 ));

      _quad_vbo.disable(tex_coord_loc);
      _quad_vbo.disable(vert_loc);

      _quad_vbo.unbind();


      _render_prog.unbind();
    }

    GL_CHECK(::glEnable(GL_DEPTH_TEST));
  }

  void DefaultRenderer::reshape( const Vector<int,2>& size ) {

    _size = size;

    _rbo_color.texImage2D( 0, GL_RGBA8, _size(0), _size(1), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0x0 );
    _rbo_depth.texImage2D( 0, GL_DEPTH_COMPONENT, _size(0), _size(1), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0x0 );

    _rbo_select.texImage2D( 0, GL_RGBA8, _size(0), _size(1), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0x0 );
    _rbo_select_depth.texImage2D( 0, GL_DEPTH_COMPONENT, _size(0), _size(1), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0x0 );


    _back_rt->resize(size);
    _front_rt->resize(size);
  }

  const RenderTarget&
  DefaultRenderer::getFrontRenderTarget() const {

    return *_front_rt;
  }

  const RenderTarget&
  DefaultRenderer::getBackRenderTarget() const {

    return *_back_rt;
  }


  void DefaultRenderer::renderScene() {

    // Setup size of viewport viewport to fit size of render target
    GL_CHECK(::glViewport(0, 0, _size(0), _size(1)));

    // Clear render buffers
    _fbo.clearColorBuffer( getClearColor() );
    _fbo.clear( GL_DEPTH_BUFFER_BIT );

    _fbo_select.clearColorBuffer( GMcolor::black() );
    _fbo_select_depth.clear( GL_DEPTH_BUFFER_BIT );

    // Object rendering
    _fbo.bind(); {

      // Render coordinate-system visualization
      renderCoordSys();

      // Render the scene objects
      for( int j = 0; j < _objs.getSize(); ++j )
        render(_objs[j]);

    } _fbo.unbind();

    // Selection rendering - render to depth buffer
    _fbo_select_depth.bind(); {

      GL_CHECK(::glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ));

      for( int j = 0; j < _objs.getSize(); ++j )
        renderSelectedGeometry(_objs[j]);

    } _fbo_select_depth.unbind();

    // Selection rendering - render
    _fbo_select.bind(); {

      GLint depth_mask, depth_func;
      GL_CHECK(::glGetIntegerv( GL_DEPTH_WRITEMASK, &depth_mask ));
      GL_CHECK(::glGetIntegerv( GL_DEPTH_FUNC, &depth_func));

      GL_CHECK(::glDepthFunc( GL_LEQUAL ));
      GL_CHECK(::glDepthMask( GL_TRUE ));

      GL_CHECK(::glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ));

      for( int j = 0; j < _objs.getSize(); ++j )
        renderSelectedGeometry(_objs[j]);

      GL_CHECK(::glDepthFunc( depth_func ));
      GL_CHECK(::glDepthMask( depth_mask ));

    } _fbo_select.unbind();

  }

  void DefaultRenderer::updateLightUBO() {

    Camera* camera = getCamera();
    const Scene* scene = camera->getScene();
    const Array<Light*> &lights_array = scene->getLights();
    const Sun* sun = scene->getSun();

    const HqMatrix<float,3> cammat = camera->SceneObject::getMatrix() * camera->getMatrixToSceneInverse();

    /*
     *  Light types of "sun", "point" and "spot" is supported.
     *  It is assumed that the lights is grouped and sorted in that order
     *  and that the "number of info" is recorded in the header block:
     *  <ul>
     *    <li>Total number of lights.</li>
     *    <li>Number of suns.</li>
     *    <li>Number of point lights.</li>
     *    <li>Number of spot lights.</li>
     *  </ul>
     */

    GL::GLLightHeader      dirlight_header;
    GL::GLLightHeader      pointlight_header;
    GL::GLLightHeader      spotlight_header;
    std::vector<GL::GLDirectionalLight> dirlights_data;
    std::vector<GL::GLPointLight> pointlights_data;
    std::vector<GL::GLSpotLight> spotlights_data;

    Array< SpotLight* > spot_lights;
    Array< PointLight* > point_lights;
    for( int i = 0; i < lights_array.size(); ++i ) {

      if(!lights_array(i)->isEnabled()) continue;

      if( SpotLight* spot_light = dynamic_cast<SpotLight*>( lights_array(i) ) )         spot_lights += spot_light;
      else if( PointLight* point_light = dynamic_cast<PointLight*>( lights_array(i) ) ) point_lights += point_light;
    }



    dirlight_header.no_lights   = sun ? 1 : 0;
    pointlight_header.no_lights = point_lights.size();
    spotlight_header.no_lights  = spot_lights.size();



    if( dirlight_header.no_lights + pointlight_header.no_lights + spotlight_header.no_lights <= 0 )
      return;

    // Add data to header array
    if( sun ) {

      GL::GLDirectionalLight dl;
      GL::OpenGLManager::fillGLDirectionalLight( dl,
                                                 sun->getGlobalAmbient(), GMcolor::black(), GMcolor::black(),
                                                 cammat * (sun->getMatrix() * sun->getDir())
                                                 );
      dirlights_data.push_back(dl);
    }

    for( int i = 0; i < point_lights.size(); ++i ) {

      PointLight *light = point_lights[i];

      GL::GLPointLight pl;
      GL::OpenGLManager::fillGLPointLight(pl,
                                          light->getAmbient(), light->getDiffuse(), light->getSpecular(),
                                          cammat * light->getGlobalPos(),
                                          light->getAttenuation()
                                          );
      pointlights_data.push_back(pl);
    }

    for( int i = 0; i < spot_lights.size(); ++i ) {

      SpotLight *light = spot_lights[i];

      GL::GLSpotLight sl;
      GL::OpenGLManager::fillGLSpotLight(sl,
                                         light->getAmbient(), light->getDiffuse(), light->getSpecular(),
                                         cammat * light->getGlobalPos(),
                                         light->getAttenuation(),
                                         cammat * light->getGlobalDir(),
                                         std::cos( light->getCutOff().getDeg() * M_PI / 180.0f )
                                         );
      spotlights_data.push_back(sl);
    }

    GL::OpenGLManager::fillLightUBO(_dirlight_ubo,   dirlight_header,   dirlights_data );
    GL::OpenGLManager::fillLightUBO(_pointlight_ubo, pointlight_header, pointlights_data );
    GL::OpenGLManager::fillLightUBO(_spotlight_ubo,  spotlight_header,  spotlights_data );








//    std::cout << "Updating light UBO!" << std::endl;
//    std::cout << "  - Total nr of Lights: " << dirlight_header.no_lights + pointlight_header.no_lights + spotlight_header.no_lights << std::endl;
//    std::cout << "  - Sun(s):             " << dirlight_header.no_lights << std::endl;
//    std::cout << "  - Point Light(s):     " << pointlight_header.no_lights << std::endl;
//    std::cout << "  - Spot Light(s):      " << spotlight_header.no_lights << std::endl;
//    std::cout << "  --------------------" << std::endl;

//    for( int i = 0; i < dirlights_data.size(); ++i) {

//      if( i < dirlight_header.no_lights) std::cout << "  Directional light";
//      else                                std::cout << "ERROR";
//      std::cout << " (" << i << ")" << std::endl;

//      const GL::GLDirectionalLight& light = dirlights_data.at(i);
//      const GLfloat *amb = light.base.amb.p;
//      const GLfloat *dif = light.base.dif.p;
//      const GLfloat *spc = light.base.spc.p;
//      const GLfloat *dir = light.dir.p;

//      std::cout << "    - Amb: " << amb[0] << ", " << amb[1] << ", " << amb[2] << std::endl;
//      std::cout << "    - Dif: " << dif[0] << ", " << dif[1] << ", " << dif[2] << std::endl;
//      std::cout << "    - Spc: " << spc[0] << ", " << spc[1] << ", " << spc[2] << std::endl;
//      std::cout << "    - Dir: " << dir[0] << ", " << dir[1] << ", " << dir[2] << std::endl;
//    }

//    for( int i = 0; i < pointlights_data.size(); ++i) {

//      if( i < pointlight_header.no_lights) std::cout << "  Pointlight";
//      else                                 std::cout << "ERROR";
//      std::cout << " (" << i << ")" << std::endl;

//      const GL::GLPointLight& light = pointlights_data.at(i);
//      const GLfloat *amb = light.base.amb.p;
//      const GLfloat *dif = light.base.dif.p;
//      const GLfloat *spc = light.base.spc.p;
//      const GLfloat *pos = light.pos.p;
//      const GL::GLAttenuation &att = light.attenuation;

//      std::cout << "    - Amb: " << amb[0]       << ", " << amb[1]     << ", " << amb[2]        << std::endl;
//      std::cout << "    - Dif: " << dif[0]       << ", " << dif[1]     << ", " << dif[2]        << std::endl;
//      std::cout << "    - Spc: " << spc[0]       << ", " << spc[1]     << ", " << spc[2]        << std::endl;
//      std::cout << "    - Pos: " << pos[0]       << ", " << pos[1]     << ", " << pos[2]        << std::endl;
//      std::cout << "    - Att: " << att.constant << ", " << att.linear << ", " << att.quadratic << std::endl;
//    }

//    for( int i = 0; i < spotlights_data.size(); ++i) {

//      if( i < spotlight_header.no_lights) std::cout << "  Spotlight";
//      else                                std::cout << "ERROR";
//      std::cout << " (" << i << ")" << std::endl;

//      const GL::GLSpotLight& light = spotlights_data.at(i);
//      const GLfloat *amb = light.pointlight.base.amb.p;
//      const GLfloat *dif = light.pointlight.base.dif.p;
//      const GLfloat *spc = light.pointlight.base.spc.p;
//      const GLfloat *pos = light.pointlight.pos.p;
//      const GL::GLAttenuation &att = light.pointlight.attenuation;
//      const GLfloat *dir = light.dir.p;
//      const GLfloat &cutoff = light.cutoff;

//      std::cout << "    - Amb: " << amb[0]       << ", " << amb[1]     << ", " << amb[2]        << std::endl;
//      std::cout << "    - Dif: " << dif[0]       << ", " << dif[1]     << ", " << dif[2]        << std::endl;
//      std::cout << "    - Spc: " << spc[0]       << ", " << spc[1]     << ", " << spc[2]        << std::endl;
//      std::cout << "    - Pos: " << pos[0]       << ", " << pos[1]     << ", " << pos[2]        << std::endl;
//      std::cout << "    - Att: " << att.constant << ", " << att.linear << ", " << att.quadratic << std::endl;
//      std::cout << "    - Dir: " << dir[0]       << ", " << dir[1]     << ", " << dir[2]        << std::endl;
//      std::cout << "    - Cut: " << cutoff                                                      << std::endl;
//    }

  }

  const GL::UniformBufferObject&
  DefaultRenderer::getDirectionalLightUBO() const { return _dirlight_ubo; }

  const GL::UniformBufferObject&
  DefaultRenderer::getPointLightUBO() const { return _pointlight_ubo; }

  const GL::UniformBufferObject&
  DefaultRenderer::getSpotLightUBO() const { return _spotlight_ubo; }


} // END namespace GMlib
