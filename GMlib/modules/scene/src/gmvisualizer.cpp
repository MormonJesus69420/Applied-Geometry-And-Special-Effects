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





#include "gmvisualizer.h"
#include "gmsceneobject.h"

// gmlib
#include <core/gmglobal.h>

using namespace GMlib;


Visualizer::~Visualizer() {}



void Visualizer::glSetDisplayMode() const {

  if( this->_display_mode == Visualizer::DISPLAY_MODE_SHADED )
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  else if( this->_display_mode == Visualizer::DISPLAY_MODE_WIREFRAME )
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}



void Visualizer::simulate( double dt ) {
  GM_UNUSED(dt)
}



void Visualizer::toggleDisplayMode() {

  if( _display_mode == Visualizer::DISPLAY_MODE_SHADED )
    _display_mode = Visualizer::DISPLAY_MODE_WIREFRAME;
  else
    _display_mode = Visualizer::DISPLAY_MODE_SHADED;
}



bool Visualizer::operator == ( const Visualizer* v ) const {

  if( this == v )
    return true;
  return false;
}



void Visualizer::update() {

}
