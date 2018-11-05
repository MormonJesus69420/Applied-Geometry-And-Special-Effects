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



#include "gmspotlightg.h"

// gmlib
#include "../render/gmdefaultrenderer.h"
#include <opengl/gmopenglmanager.h>
#include <opengl/shaders/gmgeometryshader.h>


namespace GMlib {


  SpotLightG::SpotLightG() {

    init();
  }


  SpotLightG::SpotLightG(
    const Point<float,3>& pos,
    const Vector<float,3>& dir,
    Angle cut_off
  ) : SpotLight( pos, dir, cut_off ) {

    init();
  }


  SpotLightG::SpotLightG(
    const Color& amb,
    const Color& dif,
    const Color& spe,
    const Point<float,3>& pos,
    const Vector<float,3>& dir,
    Angle cut_off
  ) : SpotLight( amb, dif, spe, pos, dir, cut_off ) {

    init();
  }


  SpotLightG::SpotLightG( const SpotLight& copy ) : SpotLight( copy ) {

    init();
  }


  SpotLightG::SpotLightG( const SpotLightG& copy) : SpotLight( copy ) {

    setSurroundingSphere( Sphere<float,3>( Point<float,3>( 0.0 ), 1.0 ) );
  }


  void SpotLightG::init() {

    setSurroundingSphere( Sphere<float,3>( Point<float,3>( 0.0 ), 1.0 ) );

    _color_prog.acquire("color");
    _shading_prog.acquire("directional_lighting");


    // Create geometry
    int i;
    double k = 3.3;
    double b = k/sqrt(1+pow(tan(_cutoff.getRad()),2));
    double a = sqrt(k*k-b*b);
    if(_cutoff.getDeg()>90) b = -b;

    Vector<float,3> d = _up;
    Vector<float,3> n = d^d.getLinIndVec();
    d.setLength(b);
    n.setLength(a);
    UnitVector<float,3> n2 = -d;
    if(_cutoff.getDeg()!=90) n2 = Vector<float,3>((b/a)*n-(a/b)*d);

    Vector<float,3> v = d ^ n;

    HqMatrix<float,3> m1(Angle(30),n,v,_pos);
    HqMatrix<float,3> m2(Angle(-30),n,v,_pos);

    Array<Point<float,3> > p1;
    p1 += _pos;
    p1 += _pos+d+n;
    for(i=1; i<12; i++) p1 += m1*p1.back();

    Vector<float,3> bb=0.6*d;
    if(b<0.0) bb.setLength(0.0);
    Array<Point<float,3> > p2;
    p2 += _pos+bb+0.1*_dir;
    p2 += _pos+0.6*d+0.6*n;
    for(i=1; i<12; i++) p2 += m2*p2.back();

    Array<Vector<float,3> > v1;
    v1 += UnitVector<float,3>(-d);
    v1 += n2;
    for(i=1; i<12; i++) v1 += m1*v1.back();

    Array<Vector<float,3> > v2;
    v2 += UnitVector<float,3>(d);
    v2 += UnitVector<float,3>(d+n);
    for(i=1; i<12; i++) v2 += m2*v2.back();


    // the light box geometry
    DVector<GL::GLVertexNormal> dp_box(p1.size()+1);
    for(i=0; i<p1.size(); i++) {
      GL::GLVertexNormal box_p;
      box_p.x   = p1(i)(0);
      box_p.y   = p1(i)(1);
      box_p.z   = p1(i)(2);
      box_p.nx  = v1(i)(0);
      box_p.ny  = v1(i)(1);
      box_p.nz  = v1(i)(2);
      dp_box[i] = box_p;
    }
    dp_box[dp_box.getDim()-1] = dp_box[1];

    _light_box_geom_elements = dp_box.getDim();
    _light_box_geom_vbo.create();
    _light_box_geom_vbo.bufferData( _light_box_geom_elements * sizeof(GL::GLVertexNormal), dp_box.getPtr(), GL_STATIC_DRAW );

    // the light geometry
    DVector<GL::GLVertexNormal> dp_light(p2.getSize()+1);
    for(i=0; i<p2.size(); i++) {
      GL::GLVertexNormal light_p;
      light_p.x   = p2(i)(0);
      light_p.y   = p2(i)(1);
      light_p.z   = p2(i)(2);
      light_p.nx  = v2(i)(0);
      light_p.ny  = v2(i)(1);
      light_p.nz  = v2(i)(2);
      dp_light[i] = light_p;
    }
    dp_light[dp_light.getDim()-1] = dp_light[1];

    _light_geom_elements = dp_light.getDim();
    _light_geom_vbo.create();
    _light_geom_vbo.bufferData( _light_geom_elements * sizeof(GL::GLVertexNormal), dp_light.getPtr(), GL_STATIC_DRAW );
  }

  void SpotLightG::setCutOff(const Angle& cut_off) {

    SpotLight::setCutOff( cut_off );
  }

  void SpotLightG::localDisplay(const DefaultRenderer* renderer) const {

    _color_prog.bind(); {

      // Common stuf for both geoemtry pieces
      _color_prog.uniform( "u_mvpmat", this->getModelViewProjectionMatrix(renderer->getCamera())  );
      GL::AttributeLocation vert_loc   = _shading_prog.getAttributeLocation( "in_vertex" );


      // Render light housing
      _color_prog.uniform( "u_color", GMcolor::darkGrey() );

      _light_box_geom_vbo.bind();
      _light_box_geom_vbo.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexNormal), reinterpret_cast<const GLvoid*>(0x0) );

      glDrawArrays( GL_TRIANGLE_FAN, 0, _light_box_geom_elements );

      _light_box_geom_vbo.disable(vert_loc);
      _light_box_geom_vbo.unbind();


      // Render light surface
      _color_prog.uniform( "u_color", getAmbient() );

      _light_geom_vbo.bind();
      _light_geom_vbo.enable( vert_loc,   3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexNormal), reinterpret_cast<const GLvoid*>(0x0) );

      glDrawArrays( GL_TRIANGLE_FAN, 0, _light_geom_elements );

      _light_geom_vbo.disable(vert_loc);
      _light_geom_vbo.unbind();

    } _color_prog.unbind();

  }


  void SpotLightG::localSelect(const Renderer* renderer, const Color& color) const {

    _color_prog.bind(); {

      // Model view and projection matrices
      _color_prog.uniform( "u_mvpmat", this->getModelViewProjectionMatrix(renderer->getCamera()) );
      _color_prog.uniform( "u_color",  color );

      GL::AttributeLocation vert_loc = _color_prog.getAttributeLocation( "in_vertex" );

      _light_box_geom_vbo.bind();
      _light_box_geom_vbo.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexNormal), reinterpret_cast<const GLvoid*>(0x0) );

      // Draw top and bottom caps
      glDrawArrays( GL_TRIANGLE_FAN, 0, _light_box_geom_elements );

      _light_box_geom_vbo.disable(vert_loc);
      _light_box_geom_vbo.unbind();

      _light_geom_vbo.bind();
      _light_geom_vbo.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexNormal), reinterpret_cast<const GLvoid*>(0x0) );

      // Draw top and bottom caps
      glDrawArrays( GL_TRIANGLE_FAN, 0, _light_geom_elements );

      _light_geom_vbo.disable(vert_loc);
      _light_geom_vbo.unbind();

    } _color_prog.unbind();
  }

} // END namespace GMlib

