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




#ifndef GM_PARAMETRICS_TRIANGLES_PSUBTRIANGLE_H
#define GM_PARAMETRICS_TRIANGLES_PSUBTRIANGLE_H



#include "../gmpsurf.h"
#include "../gmptriangle.h"


namespace GMlib {

template <typename T>
class PSubTriangle : public PTriangle<T,3> {
  GM_SCENEOBJECT(PSubTriangle)
public:
  PSubTriangle( PSurf<T,3>* s, Vector<Point<T,2>,3> p );
  PSubTriangle( const PSubTriangle <T>& copy );
  virtual ~PSubTriangle ();


  void                  edit( SceneObject* d );
  void                  edit( int selector );

  void                  setBp( const Vector< Point<T,2>,3>& bp );
  void                  updateCoeffs( const Vector<T,3>& d );

  Vector<Point<T,3>,3>  getPoints();
  Vector<Point<T,2>,3>  getParPoints();

protected:
  void                  eval( T u, T v, T w, int d ) const override;

private:
  PSurf<T,3>*             _s;
  Vector<Point<T,2>,3>  _q;

  Vector<T,3> dS (const Vector<T,3>& Su, const Vector<T,3>& Sv, const Point<T,2>& p) const;
  Vector<T,3> dS2(const Vector<T,3>& Suu, const Vector<T,3>& Suv, const Vector<T,3>& Svv, const Point<T,2>& p) const;

}; // END class PSubTriangle

} // END namespace GMlib



// Include PSubTriangle  class function implementations
#include "gmpsubtriangle.c"


#endif // GM_PARAMETRICS_TRIANGLES_PSUBTRIANGLE_H
