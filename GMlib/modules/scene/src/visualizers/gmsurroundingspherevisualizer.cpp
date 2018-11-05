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





#include "gmsurroundingspherevisualizer.h"

#include "../camera/gmcamera.h"
#include "../sceneobjects/gmsphere3d.h"



namespace GMlib {

  SurroundingSphereVisualizer::SurroundingSphereVisualizer() {

    _wireframe = true;
    _spheres = CLEAN_SPHERE;

    _cs_color = GMcolor::white();
    _ts_color = GMcolor::yellow();

    _sphere = new Sphere3D( 1.0 );
  }

  SurroundingSphereVisualizer::~SurroundingSphereVisualizer() {

    delete _sphere;
  }

  void SurroundingSphereVisualizer::render(const SceneObject *obj, const DefaultRenderer */*renderer*/) const{

    int poly_mode;
    glGetIntegerv( GL_POLYGON_MODE, &poly_mode );
    if( _wireframe )
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Display Clean Surrounding Sphere
    if( (_spheres & CLEAN_SPHERE) == CLEAN_SPHERE ) {

      displaySphere( obj->getSurroundingSphere() );
    }

    // Display Surrounding Sphere
    if( (_spheres & TOTAL_SPHERE) == TOTAL_SPHERE ) {

      displaySphere( obj->getSurroundingSphere() );
    }

    glPolygonMode(GL_FRONT_AND_BACK, poly_mode);
  }

  void SurroundingSphereVisualizer::displaySphere(const Sphere<float, 3> &ss ) const {

    float r = ss.getRadius();
    Point<float,3> pos = ss.getPos();

    HqMatrix<float,3> ss_scale_mat;
    ss_scale_mat[0][0] = r;
    ss_scale_mat[1][1] = r;
    ss_scale_mat[2][2] = r;
    ss_scale_mat[0][3] = pos[0];
    ss_scale_mat[1][3] = pos[1];
    ss_scale_mat[2][3] = pos[2];

//   _sphere->display( cam->getProjectionMatrix() * mvmat * ss_scale_mat );
  }

  void SurroundingSphereVisualizer::enableCleanSphere( bool enable ) {

    _spheres = (_spheres & TOTAL_SPHERE) | ( enable ? CLEAN_SPHERE : 0x0 );
  }

  void SurroundingSphereVisualizer::enableTotalSphere( bool enable ) {

    _spheres = (_spheres & CLEAN_SPHERE) | ( enable ? TOTAL_SPHERE : 0x0 );
  }

  const Color& SurroundingSphereVisualizer::getCleanSphereColor() const {

    return _cs_color;
  }

  const Color& SurroundingSphereVisualizer::getTotalSphereColor() const {

    return _ts_color;
  }

  bool SurroundingSphereVisualizer::isCleanSphereEnabled() const {

    return (_spheres & CLEAN_SPHERE) == CLEAN_SPHERE;
  }

  bool SurroundingSphereVisualizer::isTotalSphereEnabled() const {

    return (_spheres & TOTAL_SPHERE) == TOTAL_SPHERE;
  }

  void SurroundingSphereVisualizer::setCleanSphereColor( const Color& color ) {

    _cs_color = color;
  }

  void SurroundingSphereVisualizer::setTotalSphereColor( const Color& color ) {

    _ts_color = color;
  }

  void SurroundingSphereVisualizer::setWireframe( bool wireframe ) {

    _wireframe = wireframe;
  }

} // END namespace GMlib
