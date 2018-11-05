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




#include "gmcoordinatesystem.h"

#include "gmsphere3d.h"




namespace GMlib {



  CoordinateSystem::CoordinateSystem() {

    setSurroundingSphere( Sphere<float,3>( 1.0f, 0.0f ) );
    _cs = GM_COORDSYS_LOCAL;
  }


  CoordinateSystem::CoordinateSystem( const CoordinateSystem& copy ) : SceneObject( copy ) {}


  CoordinateSystem::~CoordinateSystem() {}


  void CoordinateSystem::localDisplay(const DefaultRenderer*) const {

//    glPushMatrix(); {




//      if( this->_parent ) {

//        HqMatrix<float,3> matrix = getMatrixGlobal();
//        matrix.invertOrthoNormal();
//        glMultMatrix( matrix );

//        HqMatrix<float,3> tm;
//        Point<float,3> pos;

//        DisplayObject *dispo = dynamic_cast<DisplayObject*>(this->_parent);
//        if( !dispo ) {
//          HqMatrix<float,3> somat = this->_parent->getMatrixGlobal();
//          pos[0] = somat[0][3];
//          pos[1] = somat[1][3];
//          pos[2] = somat[2][3];
//        }
//        else
//         pos = dispo->getPos();
//        tm.translateGlobal( pos + _pos );

//        glMultMatrix( tm );


//        switch( _cs ) {
//          case GM_COORDSYS_GLOBAL:
//          break;

//          case GM_COORDSYS_PARENT:
//          if( this->_parent->getParent() ) {

//            HqMatrix<float,3> mat = this->_parent->getParent()->getMatrixGlobal();
//            mat[0][3] = 0.0f;
//            mat[1][3] = 0.0f;
//            mat[2][3] = 0.0f;
//            glMultMatrix( mat );
//          }
//          break;

//          case GM_COORDSYS_LOCAL:
//          default: {

//            HqMatrix<float,3> mat = this->_parent->getMatrixGlobal();
//            mat[0][3] = 0.0f;
//            mat[1][3] = 0.0f;
//            mat[2][3] = 0.0f;
//            glMultMatrix( mat );
//          }
//          break;
//        }
//      }



//      // Push GL Attributes
//      glPushAttrib( GL_LIGHTING_BIT ); {

//        // Disable Lighting
//        glDisable( GL_LIGHTING );

//        glBegin( GL_LINES ); {

//          // X (red)
//          glColor( GMcolor::red() );
//          glPoint( Point<float,3>( 0.0f, 0.0f, 0.0f ) );
//          glPoint( Point<float,3>( 1.0f, 0.0f, 0.0f ) );

//          // Y (green)
//          glColor( GMcolor::green() );
//          glPoint( Point<float,3>( 0.0f, 0.0f, 0.0f ) );
//          glPoint( Point<float,3>( 0.0f, 1.0f, 0.0f ) );

//          // Z (blue)
//          glColor( GMcolor::blue() );
//          glPoint( Point<float,3>( 0.0f, 0.0f, 0.0f ) );
//          glPoint( Point<float,3>( 0.0f, 0.0f, 1.0f ) );

//        } glEnd();

//      // Pop GL Attributes
//      } glPopAttrib();

//    } glPopMatrix();
  }


  GM_COORDSYS CoordinateSystem::getCoordinateSystem() const {

    return _cs;
  }

  void CoordinateSystem::setCoordinateSystem( GM_COORDSYS coordsys ) {

    _cs = coordsys;
  }


} // END namespace GMlib

