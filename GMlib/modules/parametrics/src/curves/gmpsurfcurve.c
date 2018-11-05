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



//#include "../evaluators/gmevaluatorstatic.h"

namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************

  template <typename T>
  inline
  PSurfCurve<T>::PSurfCurve(  PSurf<T,3>* s,  const Point<T,2>& p1,   const Point<T,2>& p2 )
  {
    _der_curve = false;
    _plot = false;
    _s  = s;
    _p1 = p1;
    _p2 = p2;
    _dv = p2-p1;
  }


  template <typename T>
  inline
  PSurfCurve<T>::PSurfCurve(  PSurf<T,3>* s,  const Point<T,2>& p1,   const Point<T,2>& p2,
                                            const Vector<T,2>& v1,  const Vector<T,2>& v2 )
  {
    _der_curve = true;
    _plot = false;

    _s  = s;
    _p1 = p1;
    _p2 = p2;
    _dv = p2-p1;
    _v1 = v1;
    _v2 = v2;
  }


  template <typename T>
  inline
  PSurfCurve<T>::PSurfCurve( const PSurfCurve<T>& copy ) : PCurve<T,3>( copy ) {
      _der_curve = copy._der_curve;
      _plot = false;

      _s  = copy._s;
      _p1 = copy._p1;
      _p2 = copy._p2;
      _dv = copy._dv;
      _v1 = copy._v1;
      _v2 = copy._v2;
  }


  template <typename T>
  PSurfCurve<T>::~PSurfCurve() {}




  //**************************************
  //        Public local functons       **
  //**************************************


  template <typename T>
  inline
  void PSurfCurve<T>::togglePlot() {
     _plot = !_plot;
  }


  template <typename T>
  void PSurfCurve<T>::resample( DVector< DVector< Vector<T, 3> > >& p, int m, int d, T start, T end )
  {
      if (_der_curve && _plot)
      {
        T du = (end-start)/(m-1);
        p.setDim(m);

        for( int i = 0; i < m - 1; i++ )
        {
          p[i].setDim(d+1);
          eval1(start + i * du, 1);
          p[i][0] = this->_p[0];
          eval2(start + i * du, d-1);
          for(int j=1; j<d;j++)
             p[i][j] = this->_p[j-1];
        }
        p[m-1].setDim(d+1);
        eval1(end, 1);
        p[m-1][0] = this->_p[0];
        eval2(end, d-1);
        for(int j=1; j<d;j++)
           p[m-1][j] = this->_p[j-1];

        switch( this->_dm )
        {
          case GM_DERIVATION_EXPLICIT:
            // Do nothing, evaluator algorithms for explicite calculation of derivatives
            // should be defined in the eval( ... ) function enclosed by
            // if( this->_derivation_method == this->EXPLICIT ) { ... eval algorithms for derivatives ... }
            break;

          case GM_DERIVATION_DD:
          default:
          //  _evalDerDD( p, d, du );
            break;
        };
      }
      else
        PCurve<T,3>::resample(  p, m, d, start, end );
  }




  //***************************************************
  // Overrided (public) virtual functons from PCurve **
  //***************************************************

  template <typename T>
  bool PSurfCurve<T>::isClosed() const {
    return false;
  }


  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************

  template <typename T>
  inline
  void PSurfCurve<T>::eval( T t, int d, bool /*l*/ ) const {
      if (_der_curve)
          eval2(t,d);
      else
          eval1(t,d);
  }


  template <typename T>
  T PSurfCurve<T>::getStartP() const {
    return T(0);
  }


  template <typename T>
  T PSurfCurve<T>::getEndP() const {
    return T(1);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************


  template <typename T>
  inline
  void PSurfCurve<T>::eval1( T t, int d ) const {
    this->_p.setDim( d + 1 );

    DMatrix< Vector<T,3> > m = _s->evaluateParent(_p1 + t*_dv , d);

    this->_p[0] = m[0][0];
    if(d>0)
    {
        Matrix<T,3,2> d1;
        d1.setCol(m[1][0], 0);
        d1.setCol(m[0][1], 1);
        this->_p[1] = d1*_dv;
    }
    if(d>1)
    {
        Matrix<T,3,2> d2;
        d2.setCol(m[2][0], 0);
        d2.setCol(m[1][1], 1);
        Vector<T,3> v1 = d2*_dv;
        Matrix<T,3,2> d3;
        d3.setCol(m[1][1], 0);
        d3.setCol(m[0][2], 1);
        Vector<T,3> v2 = d3*_dv;
        d2.setCol(v1, 0);
        d2.setCol(v2, 1);
        this->_p[2]= d2*_dv;
    }
    if(d>2)
    {
        Matrix<T,3,2> d4;
        d4.setCol(m[3][0], 0);
        d4.setCol(m[2][1], 1);
        Vector<T,3> v1 = d4*_dv;
        Matrix<T,3,2> d5;
        d5.setCol(m[2][1], 0);
        d5.setCol(m[1][2], 1);
        Vector<T,3> v2 = d5*_dv;
        d4.setCol(v1, 0);
        d4.setCol(v2, 1);
        v1 = d4*_dv;

        d4.setCol(m[1][2], 0);
        d4.setCol(m[0][3], 1);
        Vector<T,3> v3 = d4*_dv;
        d5.setCol(v2, 0);
        d5.setCol(v3, 1);
        v2 = d5*_dv;
        d4.setCol(v1, 0);
        d4.setCol(v2, 1);
        this->_p[3] = d4*_dv;
    }
  }



  template <typename T>
  inline
  void PSurfCurve<T>::eval2( T t, int d ) const
  {
    this->_p.setDim( d + 1 );

    Vector<T,2> h  = _p1 + t*(_p2-_p1);
    Vector<T,2> dh = _p2-_p1;

    DMatrix<Vector<T,3> > m = _s->evaluateParent(h, d+1);

    Vector<T,2> v  = (1-t)*_v1 + t*_v2;
//    Vector<T,2> dv = _v2 - _v1;

//    Matrix<T,3,2> d1, d2, d3;
//    d1.setCol(m[1][0], 0);
//    d1.setCol(m[0][1], 1);
    this->_p[0] = m[1][0]*v[0]+m[0][1]*v[1];
    if(d>0)
    {
        Vector<T,3> v1 = m[2][0]*dh[0] + m[1][1]*dh[1];
        Vector<T,3> v2 = m[1][1]*dh[0] + m[0][2]*dh[1];
        Vector<T,3> bb = v[0]*v1 + v[1]*v2 + m[1][0]*dh[0] + m[0][1]*dh[1];
        this->_p[1] = bb;
//        d2.setCol(m[2][0], 0);
//        d2.setCol(m[1][1], 1);
//        Vector<T,3> v1 = d2*v;
//        d2.setCol(m[1][1], 0);
//        d2.setCol(m[0][2], 1);
//        Vector<T,3> v2 = d2*v;
//        d2.setCol(v1, 0);
//        d2.setCol(v2, 1);
//        this->_p[1] = d2*_dv;
    }
    if(d>1)
    {
        Matrix<T,3,2> d3;
        d3.setCol(m[3][0], 0);
        d3.setCol(m[2][1], 1);
        Vector<T,3> v1 = d3*v;
        d3.setCol(m[2][1], 0);
        d3.setCol(m[1][2], 1);
        Vector<T,3> v2 = d3*v;
        d3.setCol(m[1][2], 0);
        d3.setCol(m[0][3], 1);
        Vector<T,3> v3 = d3*v;
        d3.setCol(v1, 0);
        d3.setCol(v2, 1);
        Vector<T,3> v4 = d3*_dv;
        d3.setCol(v2, 0);
        d3.setCol(v3, 1);
        Vector<T,3> v5 = d3*_dv;
        d3.setCol(v4, 0);
        d3.setCol(v5, 1);
        Vector<T,3> v6 = d3*_dv;

        this->_p[2] = v6;
    }
  }




} // END namespace GMlib

