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



#ifndef GM_PARAMETRICS_CURVES_PBASISCURVE_H
#define GM_PARAMETRICS_CURVES_PBASISCURVE_H


#include "../gmpcurve.h"


#include "../evaluators/gmbasisevaluator.h"


namespace GMlib {


  template <typename T, typename G = long double>
  class PBasisCurve : public PCurve<T,3> {
    GM_SCENEOBJECT(PBasisCurve)
  public:
    PBasisCurve(T scale=T(1));
    PBasisCurve( const PBasisCurve<T,G>& copy );

    ~PBasisCurve();

    // Public local functions
    void                       setDisplay( int display );
    void                       setEvaluator( BasisEvaluator<G>* e );

    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from PCurve
    bool                       isClosed() const override;

  protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void                       eval( T t, int d, bool l ) const override;
    T                          getEndP()   const override;
    T                          getStartP() const override;

  private:
    // Private data for the curve
    BasisEvaluator<G>*         _B;
    int                        _d_no;

    T                          _scale;

  }; // class ERBSBasisCurve

} // END namespace GMlib


// Include PBasisCurve class function implementations
#include "gmpbasiscurve.c"


#endif // GM_PARAMETRICS_CURVES_PBASISCURVE_H
