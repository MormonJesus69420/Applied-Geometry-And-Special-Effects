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



#include "gmcoordsysrepvisualizer.h"

// local
#include "gmstdrepvisualizer.h"
#include "../render/gmdefaultrenderer.h"
#include "../camera/gmcamera.h"



namespace GMlib {

  CoordSysRepVisualizer::CoordSysRepVisualizer() {

    _s_instance = VisualizerStdRep::getInstance();
  }

  void CoordSysRepVisualizer::render(const SceneObject* /*obj*/, const DefaultRenderer *renderer) const {

    const Camera *cam = renderer->getCamera();
    // Get matrix of present camera and
    HqMatrix<float,3> base_mvmat;
    HqMatrix<float,3> mvmat = cam->SceneObject::getMatrix();
    mvmat[0][3] = 0.0f;
    mvmat[1][3] = 0.0f;
    mvmat[2][3] = 0.0f;
    const HqMatrix<float,3> &pmat = cam->getProjectionMatrix();

    // Compute the position of the of the "coordinate-system visualization"
    // Relevant to the camera position
    float hh = -1.5*cam->getNearPlane()*cam->getAngleTan();
    Point<float,3> cp(cam->getAspectRatio()*hh, hh, -cam->getNearPlane()-1.0);
    base_mvmat.translateGlobal(cp);
    mvmat.translateGlobal(cp);

    _s_instance->render( mvmat, pmat );
  }


} // END namespace GMlib
