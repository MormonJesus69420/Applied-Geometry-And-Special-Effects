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


#include "gmcameravisualizer.h"

#include "../gmsceneobject.h"
#include "../camera/gmcamera.h"
#include "../render/gmdefaultrenderer.h"

#include <opengl/gmopengl.h>


namespace GMlib {


  CameraVisualizer::CameraVisualizer() {

    _prog.acquire("blinn_phong");
    _color_prog.acquire("color");
    assert(_prog.isValid());
    assert(_color_prog.isValid());

    initGeometry();
  }

  void CameraVisualizer::render( const SceneObject* obj, const DefaultRenderer* renderer ) const {


    GL_CHECK(::glPolygonMode( GL_FRONT_AND_BACK, GL_FILL));


    const Camera* cam = renderer->getCamera();

    const HqMatrix<float,3> &mvmat = obj->getModelViewMatrix(cam);
    const HqMatrix<float,3> &pmat  = obj->getProjectionMatrix(cam);


    _prog.bind(); {

      // Model view and projection matrices
      _prog.uniform( "u_mvmat", mvmat );
      _prog.uniform( "u_mvpmat", pmat * mvmat );

      // Lights
      _prog.bindBufferBase( "DirectionalLights",  renderer->getDirectionalLightUBO(), 0 );
      _prog.bindBufferBase( "PointLights",        renderer->getPointLightUBO(), 1 );
      _prog.bindBufferBase( "SpotLights",         renderer->getSpotLightUBO(), 2 );

      static Material lens(
        Color( 0.19125f,  0.0735f,  0.0225f,    1.0f ),
        Color( 0.5038f,   0.27048f,  0.0828f,   1.0f ),
        Color( 0.256777f, 0.137622f, 0.086014f, 1.0f ),
        12.8f
      );

      static Material rubb(
        Color( 0.105882f, 0.058824f, 0.103725f, 1.0f ),
        Color( 0.427451f, 0.470588f, 0.501176f, 1.0f ),
        Color( 0.333333f, 0.333333f, 0.501569f, 1.0f ),
        9.84615f
      );




      Material mat = GMmaterial::polishedSilver();
      _prog.uniform( "u_mat_amb", mat.getAmb() );
      _prog.uniform( "u_mat_dif", mat.getDif() );
      _prog.uniform( "u_mat_spc", mat.getSpc() );
      _prog.uniform( "u_mat_shi", mat.getShininess() );


      GL::AttributeLocation vertex_loc = _prog.getAttributeLocation( "in_vertex" );
      GL::AttributeLocation normal_loc = _prog.getAttributeLocation( "in_normal" );

      _vbo_display.bind();
      _vbo_display.enable( vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), static_cast<const GLvoid*>(0x0) );
      _vbo_display.enable( normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), reinterpret_cast<const GLvoid*>(sizeof(Point<GLfloat,3>)) );

      GL_CHECK(::glDrawArrays( GL_QUADS, 0, 24 ));

      _vbo_display.disable(normal_loc);
      _vbo_display.disable(vertex_loc);
      _vbo_display.unbind();

      _vbo_viewfinder.bind();
      _vbo_viewfinder.enable( vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), static_cast<const GLvoid*>(0x0) );
      _vbo_viewfinder.enable( normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), reinterpret_cast<const GLvoid*>(sizeof(Point<GLfloat,3>)) );

      GL_CHECK(::glDrawArrays( GL_QUADS, 0, 40 ));

      _vbo_viewfinder.disable(normal_loc);
      _vbo_viewfinder.disable(vertex_loc);
      _vbo_viewfinder.unbind();



      mat = rubb;
      _prog.uniform( "u_mat_amb", mat.getAmb() );
      _prog.uniform( "u_mat_dif", mat.getDif() );
      _prog.uniform( "u_mat_spc", mat.getSpc() );
      _prog.uniform( "u_mat_shi", mat.getShininess() );


      _vbo_grip.bind();
      _vbo_grip.enable( vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), static_cast<const GLvoid*>(0x0) );
      _vbo_grip.enable( normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), reinterpret_cast<const GLvoid*>(sizeof(Point<GLfloat,3>)) );

      GL_CHECK(::glDrawArrays( GL_QUADS, 0, 24 ));

      _vbo_grip.disable(normal_loc);
      _vbo_grip.disable(vertex_loc);
      _vbo_grip.unbind();


      _vbo_viewfinder_2.bind();
      _vbo_viewfinder_2.enable( vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), static_cast<const GLvoid*>(0x0) );
      _vbo_viewfinder_2.enable( normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), reinterpret_cast<const GLvoid*>(sizeof(Point<GLfloat,3>)) );

      GL_CHECK(::glDrawArrays( GL_TRIANGLE_STRIP, 0, 18));

      _vbo_viewfinder_2.disable(normal_loc);
      _vbo_viewfinder_2.disable(vertex_loc);
      _vbo_viewfinder_2.unbind();




      mat = lens;
      _prog.uniform( "u_mat_amb", mat.getAmb() );
      _prog.uniform( "u_mat_dif", mat.getDif() );
      _prog.uniform( "u_mat_spc", mat.getSpc() );
      _prog.uniform( "u_mat_shi", mat.getShininess() );


      _vbo_lens.bind();
      _vbo_lens.enable( vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), static_cast<const GLvoid*>(0x0) );
      _vbo_lens.enable( normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), reinterpret_cast<const GLvoid*>(sizeof(Point<GLfloat,3>)) );

      GL_CHECK(::glDrawArrays( GL_TRIANGLE_STRIP, 0, 18));

      _vbo_lens.disable(normal_loc);
      _vbo_lens.disable(vertex_loc);
      _vbo_lens.unbind();

    } _prog.unbind();






    // If frustum visible - draw frustum
    const Camera * this_cam = dynamic_cast<const Camera*>(obj);
    if(this_cam && this_cam->isFrustumVisible()) {

      _vbo_frame.bufferSubData(  0x0, 8 * 3 * sizeof(GLfloat), this_cam->getFrustumFramePtr() );

      _color_prog.bind(); {

        GL::AttributeLocation vertex_loc = _color_prog.getAttributeLocation( "in_vertex" );
        _color_prog.uniform( "u_color", GMcolor::white() );
        _color_prog.uniform( "u_mvpmat", pmat * mvmat );

        _vbo_frame.bind();
        _vbo_frame.enable( vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, static_cast<const GLvoid*>(0x0) );
        _ibo_frame_np.bind();

        GL_CHECK(::glDrawElements( GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, static_cast<const GLvoid*>(0x0) ));
        GL_CHECK(::glDrawElements( GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(4 * sizeof(GLushort)) ));
        GL_CHECK(::glDrawElements( GL_LINES,     8, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(8 * sizeof(GLushort)) ));

        _ibo_frame_np.unbind();
        _vbo_frame.disable(vertex_loc);
        _vbo_frame.unbind();

      } _color_prog.unbind();
    }
  }

  void CameraVisualizer::renderGeometry( const SceneObject* obj, const Renderer* renderer, const Color& color ) const {

    _color_prog.bind(); {

      _color_prog.uniform( "u_color", color );
      _color_prog.uniform( "u_mvpmat", obj->getModelViewProjectionMatrix(renderer->getCamera()) );


      GL::AttributeLocation vertex_loc = _color_prog.getAttributeLocation( "in_vertex" );

      _vbo_display.bind();
      _vbo_display.enable( vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), static_cast<const GLvoid*>(0x0) );

      GL_CHECK(::glDrawArrays( GL_QUADS, 0, 24 ));

      _vbo_display.disable(vertex_loc);
      _vbo_display.unbind();

      _vbo_grip.bind();
      _vbo_grip.enable( vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), static_cast<const GLvoid*>(0x0) );

      GL_CHECK(::glDrawArrays( GL_QUADS, 0, 24 ));

      _vbo_grip.disable(vertex_loc);
      _vbo_grip.unbind();


      _vbo_viewfinder.bind();
      _vbo_viewfinder.enable( vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), static_cast<const GLvoid*>(0x0) );

      GL_CHECK(::glDrawArrays( GL_QUADS, 0, 40 ));

      _vbo_viewfinder.disable(vertex_loc);
      _vbo_viewfinder.unbind();


      _vbo_viewfinder_2.bind();
      _vbo_viewfinder_2.enable( vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), static_cast<const GLvoid*>(0x0) );

      GL_CHECK(::glDrawArrays( GL_TRIANGLE_STRIP, 0, 18));

      _vbo_viewfinder_2.disable(vertex_loc);
      _vbo_viewfinder_2.unbind();

      _vbo_lens.bind();
      _vbo_lens.enable( vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Arrow<GLfloat,3>), static_cast<const GLvoid*>(0x0) );

      GL_CHECK(::glDrawArrays( GL_TRIANGLE_STRIP, 0, 18));

      _vbo_lens.disable(vertex_loc);
      _vbo_lens.unbind();

    } _color_prog.unbind();

  }

  void CameraVisualizer::initGeometry() {


    _vbo_np.create();
    _vbo_fp.create();
    _vbo_flines.create();

    _vbo_np.bufferData(    4 * 3 * sizeof(GLfloat), 0x0, GL_DYNAMIC_DRAW);
    _vbo_fp.bufferData(    4 * 3 * sizeof(GLfloat), 0x0, GL_DYNAMIC_DRAW);
    _vbo_flines.bufferData(8 * 3 * sizeof(GLfloat), 0x0, GL_DYNAMIC_DRAW);

    _vbo_frame.create();
    _vbo_frame.bufferData( 8 * 3 * sizeof(GLfloat), 0x0, GL_DYNAMIC_DRAW);

    _ibo_frame_np.create();
    GLushort frame_indices[] = { 0, 1, 2 ,3, 4, 5, 6, 7, 0, 4, 1, 5, 2, 6, 3, 7 };
    _ibo_frame_np.bufferData( 16 * sizeof(GLushort), frame_indices, GL_STATIC_DRAW);





    // Buffer identifiers
    std::string vbo_display_str ("gm_vbo_camera_display");
    std::string vbo_grip_str ("gm_vbo_camera_grip");
    std::string vbo_viewfinder_str ("gm_vbo_camera_viewfinder");
    std::string vbo_viewfinder_2_str ("gm_vbo_camera_viewfinder_2");
    std::string vbo_lens_str ("gm_vbo_camera_lens");

    // IF first acquired acquire the rest and return; ELSE create
    if(_vbo_display.acquire(vbo_display_str)) {

      _vbo_grip.acquire(vbo_grip_str);
      _vbo_viewfinder.acquire(vbo_viewfinder_str);
      _vbo_viewfinder_2.acquire(vbo_viewfinder_2_str);
      _vbo_lens.acquire(vbo_lens_str);
      return;
    }
    else {

      _vbo_display.create(vbo_display_str);
      _vbo_display.setPersistent(true);
      _vbo_grip.create(vbo_grip_str);
      _vbo_grip.setPersistent(true);
      _vbo_viewfinder.create(vbo_viewfinder_str);
      _vbo_viewfinder.setPersistent(true);
      _vbo_viewfinder_2.create(vbo_viewfinder_2_str);
      _vbo_viewfinder_2.setPersistent(true);
      _vbo_lens.create(vbo_lens_str);
      _vbo_lens.setPersistent(true);
    }






    // Display
    std::vector< Arrow<GLfloat,3> > display(24);
    display[0]  = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,0.1,0.3),    Vector<float,3>(-1,0,0));
    display[1]  = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,0.1,-0.3),   Vector<float,3>(-1,0,0));
    display[2]  = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,-0.1,-0.3),  Vector<float,3>(-1,0,0));
    display[3]  = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,-0.1,0.3),   Vector<float,3>(-1,0,0));

    display[4]  = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,0.1,0.3),    Vector<float,3>(0,1,0));
    display[5]  = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,0.1,0.3),   Vector<float,3>(0,1,0));
    display[6]  = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,0.1,-0.3),  Vector<float,3>(0,1,0));
    display[7]  = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,0.1,-0.3),   Vector<float,3>(0,1,0));

    display[8]  = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,-0.1,0.3),  Vector<float,3>(1,0,0));
    display[9]  = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,-0.1,-0.3), Vector<float,3>(1,0,0));
    display[10] = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,0.1,-0.3),  Vector<float,3>(1,0,0));
    display[11] = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,0.1,0.3),   Vector<float,3>(1,0,0));

    display[12] = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,-0.1,0.3),   Vector<float,3>(0,-1,0));
    display[13] = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,-0.1,-0.3),  Vector<float,3>(0,-1,0));
    display[14] = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,-0.1,-0.3), Vector<float,3>(0,-1,0));
    display[15] = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,-0.1,0.3),  Vector<float,3>(0,-1,0));

    display[16] = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,0.1,0.3),    Vector<float,3>(0,0,1));
    display[17] = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,-0.1,0.3),   Vector<float,3>(0,0,1));
    display[18] = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,-0.1,0.3),  Vector<float,3>(0,0,1));
    display[19] = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,0.1,0.3),   Vector<float,3>(0,0,1));

    display[20] = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,-0.1,-0.3),  Vector<float,3>(0,0,-1));
    display[21] = Arrow<GLfloat,3>(Point<GLfloat,3>(0.07,0.1,-0.3),   Vector<float,3>(0,0,-1));
    display[22] = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,0.1,-0.3),  Vector<float,3>(0,0,-1));
    display[23] = Arrow<GLfloat,3>(Point<GLfloat,3>(-0.07,-0.1,-0.3), Vector<float,3>(0,0,-1));

    _vbo_display.bufferData( 24 * sizeof(Arrow<GLfloat,3>), display.data(), GL_STATIC_DRAW );







    // Grip
    std::vector< Arrow<GLfloat,3> > grip(24);
    grip[0] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.1,-0.02),    Vector<float,3>(-1,0,0));
    grip[1] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.1,-0.08),    Vector<float,3>(-1,0,0));
    grip[2] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.25,-0.06),   Vector<float,3>(-1,0,0));
    grip[3] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.25,0.0),     Vector<float,3>(-1,0,0));

    grip[4] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.1,-0.02),    Vector<float,3>(0,1,0));
    grip[5] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.1,-0.02),   Vector<float,3>(0,1,0));
    grip[6] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.1,-0.08),   Vector<float,3>(0,1,0));
    grip[7] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.1,-0.08),    Vector<float,3>(0,1,0));

    grip[8] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.25,0.0),    Vector<float,3>(1,0,0));
    grip[9] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.25,-0.06),  Vector<float,3>(1,0,0));
    grip[10] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.1,-0.08),  Vector<float,3>(1,0,0));
    grip[11] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.1,-0.02),  Vector<float,3>(1,0,0));

    grip[12] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.25,0.0),    Vector<float,3>(0,-1,0));
    grip[13] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.25,-0.06),  Vector<float,3>(0,-1,0));
    grip[14] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.25,-0.06), Vector<float,3>(0,-1,0));
    grip[15] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.25,0.0),   Vector<float,3>(0,-1,0));

    grip[16] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.1,-0.02),   Vector<float,3>(0,0,1));
    grip[17] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.25,0.0),    Vector<float,3>(0,0,1));
    grip[18] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.25,0.0),   Vector<float,3>(0,0,1));
    grip[19] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.1,-0.02),  Vector<float,3>(0,0,1));

    grip[20] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.25,-0.06),  Vector<float,3>(0,0,-1));
    grip[21] = Arrow<GLfloat,3>(Point<float,3>(0.02,-0.1,-0.08),   Vector<float,3>(0,0,-1));
    grip[22] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.1,-0.08),  Vector<float,3>(0,0,-1));
    grip[23] = Arrow<GLfloat,3>(Point<float,3>(-0.02,-0.25,-0.06), Vector<float,3>(0,0,-1));

    _vbo_grip.bufferData( 24 * sizeof(Arrow<GLfloat,3>), grip.data(), GL_STATIC_DRAW );



    // Viewfinder (part 1 and 2)
    std::vector< Arrow<GLfloat,3> > viewfinder(40);


    viewfinder[0] = Arrow<GLfloat,3>(Point<float,3>(-0.016,0.12,0.09),Vector<float,3>(-1,0,0));
    viewfinder[1] = Arrow<GLfloat,3>(Point<float,3>(-0.016,0.12,0.05),Vector<float,3>(-1,0,0));
    viewfinder[2] = Arrow<GLfloat,3>(Point<float,3>(-0.016,0.1,0.05),Vector<float,3>(-1,0,0));
    viewfinder[3] = Arrow<GLfloat,3>(Point<float,3>(-0.016,0.1,0.09),Vector<float,3>(-1,0,0));
    viewfinder[4] = Arrow<GLfloat,3>(Point<float,3>(-0.054,0.1,0.09),Vector<float,3>(1,0,0));
    viewfinder[5] = Arrow<GLfloat,3>(Point<float,3>(-0.054,0.1,0.05),Vector<float,3>(1,0,0));
    viewfinder[6] = Arrow<GLfloat,3>(Point<float,3>(-0.054,0.12,0.05),Vector<float,3>(1,0,0));
    viewfinder[7] = Arrow<GLfloat,3>(Point<float,3>(-0.054,0.12,0.09),Vector<float,3>(1,0,0));
    viewfinder[8] = Arrow<GLfloat,3>(Point<float,3>(-0.016,0.12,0.09),Vector<float,3>(0,0,1));
    viewfinder[9] = Arrow<GLfloat,3>(Point<float,3>(-0.016,0.1,0.09),Vector<float,3>(0,0,1));
    viewfinder[10] = Arrow<GLfloat,3>(Point<float,3>(-0.054,0.1,0.09),Vector<float,3>(0,0,1));
    viewfinder[11] = Arrow<GLfloat,3>(Point<float,3>(-0.054,0.12,0.09),Vector<float,3>(0,0,1));
    viewfinder[12] = Arrow<GLfloat,3>(Point<float,3>(-0.016,0.1,0.05),Vector<float,3>(0,0,-1));
    viewfinder[13] = Arrow<GLfloat,3>(Point<float,3>(-0.016,0.12,0.05),Vector<float,3>(0,0,-1));
    viewfinder[14] = Arrow<GLfloat,3>(Point<float,3>(-0.054,0.12,0.05),Vector<float,3>(0,0,-1));
    viewfinder[15] = Arrow<GLfloat,3>(Point<float,3>(-0.054,0.1,0.05),Vector<float,3>(0,0,-1));
    viewfinder[16] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.16,0.35),Vector<float,3>(-1,0,0));
    viewfinder[17] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.16,0.0),Vector<float,3>(-1,0,0));
    viewfinder[18] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.12,0.0),Vector<float,3>(-1,0,0));
    viewfinder[19] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.12,0.35),Vector<float,3>(-1,0,0));
    viewfinder[20] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.16,0.35),Vector<float,3>(0,1,0));
    viewfinder[21] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.16,0.35),Vector<float,3>(0,1,0));
    viewfinder[22] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.16,0.0),Vector<float,3>(0,1,0));
    viewfinder[23] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.16,0.0),Vector<float,3>(0,1,0));
    viewfinder[24] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.12,0.35),Vector<float,3>(1,0,0));
    viewfinder[25] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.12,0.0),Vector<float,3>(1,0,0));
    viewfinder[26] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.16,0.0),Vector<float,3>(1,0,0));
    viewfinder[27] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.16,0.35),Vector<float,3>(1,0,0));
    viewfinder[28] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.12,0.35),Vector<float,3>(0,-1,0));
    viewfinder[29] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.12,0.0),Vector<float,3>(0,-1,0));
    viewfinder[30] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.12,0.0),Vector<float,3>(0,-1,0));
    viewfinder[31] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.12,0.35),Vector<float,3>(0,-1,0));
    viewfinder[32] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.16,0.35),Vector<float,3>(0,0,1));
    viewfinder[33] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.12,0.35),Vector<float,3>(0,0,1));
    viewfinder[34] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.12,0.35),Vector<float,3>(0,0,1));
    viewfinder[35] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.16,0.35),Vector<float,3>(0,0,1));
    viewfinder[36] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.12,0.0),Vector<float,3>(0,0,-1));
    viewfinder[37] = Arrow<GLfloat,3>(Point<float,3>(-0.015,0.16,0.0),Vector<float,3>(0,0,-1));
    viewfinder[38] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.16,0.0),Vector<float,3>(0,0,-1));
    viewfinder[39] = Arrow<GLfloat,3>(Point<float,3>(-0.055,0.12,0.0),Vector<float,3>(0,0,-1));

    _vbo_viewfinder.bufferData( 40 * sizeof(Arrow<GLfloat,3>), viewfinder.data(), GL_STATIC_DRAW );






    // Viewfinder (part 3)
    std::vector< Arrow<GLfloat,3> >  viewfinder_2(18);

    HqMatrix<GLfloat,3> mat(Angle(M_PI_4),
                          Vector<GLfloat,3>(0,1,0), Vector<GLfloat,3>(1,0,0),
                          Point<GLfloat,3>(-0.035,0.14,0.0));

    viewfinder_2[1]  = Arrow<GLfloat,3>( Point<GLfloat,3>(-0.055,0.16,0.35), Vector<GLfloat,3>(-1,1,0) );
    viewfinder_2[5]  = Arrow<GLfloat,3>( Point<GLfloat,3>(-0.015,0.16,0.35), Vector<GLfloat,3>(1,1,0) );
    viewfinder_2[9]  = Arrow<GLfloat,3>( Point<GLfloat,3>(-0.015,0.12,0.35), Vector<GLfloat,3>(1,-1,0) );
    viewfinder_2[13] = Arrow<GLfloat,3>( Point<GLfloat,3>(-0.055,0.12,0.35), Vector<GLfloat,3>(-1,-1,0) );
    viewfinder_2[17] = Arrow<GLfloat,3>( Point<GLfloat,3>(-0.055,0.16,0.35), Vector<GLfloat,3>(-1,1,0) );

    viewfinder_2[3]  = Arrow<GLfloat,3>( Point<GLfloat,3>(-0.035,0.16,0.35), Vector<GLfloat,3>(0,1,0) );
    viewfinder_2[7]  = Arrow<GLfloat,3>( Point<GLfloat,3>(-0.015,0.14,0.35), Vector<GLfloat,3>(1,0,0) );
    viewfinder_2[11] = Arrow<GLfloat,3>( Point<GLfloat,3>(-0.035,0.12,0.35), Vector<GLfloat,3>(0,-1,0) );
    viewfinder_2[15] = Arrow<GLfloat,3>( Point<GLfloat,3>(-0.055,0.14,0.35), Vector<GLfloat,3>(-1,0,0) );

    viewfinder_2[0]  = viewfinder_2[1] + Arrow<GLfloat,3>(  Point<GLfloat,3>(-0.01,0.01,0.05), Vector<GLfloat,3>(0.0,0.0,0.02) );

    for(int i=2; i<18;i+=2)
      viewfinder_2[i] = Arrow<GLfloat,3>( mat * viewfinder_2[i-2].getPos(), mat * viewfinder_2[i-2].getDir() );

    _vbo_viewfinder_2.bufferData( 18 * sizeof(Arrow<GLfloat,3>), viewfinder_2.data(), GL_STATIC_DRAW );





    // Lens
    std::vector< Arrow<GLfloat,3> >  lens(18);

    HqMatrix<float,3> mat2(Angle(M_PI_4),Vector<float,3>(0,1,0),Vector<float,3>(1,0,0));

    Arrow<float,3> cp( Point<float,3>(0,0,-0.3), Vector<float,3>(0.0,1.0,0.15) );
    lens[0] = cp + Point<float,3>(0.0,0.05,0.0);
    lens[1] = cp + Point<float,3>(0.0,0.08,-0.2);

    for(int i=2; i<18;i+=2) {
      lens[i]   = Arrow<GLfloat,3>( mat2 * lens[i-2].getPos(), mat * lens[i-2].getDir() );
      lens[i+1] = Arrow<GLfloat,3>( mat2 * lens[i-1].getPos(), mat * lens[i-2].getDir() );
    }


    _vbo_lens.bufferData( 18 * sizeof(Arrow<GLfloat,3>), lens.data(), GL_STATIC_DRAW );

  }


} // END namespace GMlib
