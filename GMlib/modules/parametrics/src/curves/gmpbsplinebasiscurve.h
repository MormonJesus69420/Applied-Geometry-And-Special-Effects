#ifndef GM_PARAMETRICS_CURVES_BSPLINEBASISCURVE_H
#define GM_PARAMETRICS_CURVES_BSPLINEBASISCURVE_H



// GMlib includes+
#include "../gmpcurve.h"


namespace GMlib {


  template <typename T>
  class PBSplineBasisCurve : public PCurve<T,3> {
      GM_SCENEOBJECT(PBSplineBasisCurve)
  public:
    PBSplineBasisCurve( const DVector<T>& t );
    PBSplineBasisCurve( const PBSplineBasisCurve<T>& copy );

    virtual ~PBSplineBasisCurve();

    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from PCurve
    bool            isClosed() const override;

  protected:
    // Virtual functions from PCurve, which have to be implemented locally
    void            eval( T t, int d = 0, bool l = false ) const override;
    T               getEndP()   const override;
    T               getStartP() const override;

    // Protected data for the curve
    DVector<T>      _t; // Knot vector

  }; // END class PBSplineBasisCurve

} // END namepace GMlib


// Include PBSplineBasisCurve class function implementations
#include "gmpbsplinebasiscurve.c"


#endif // GM_PARAMETRICS_CURVES_BSPLINEBASISCURVE_H
