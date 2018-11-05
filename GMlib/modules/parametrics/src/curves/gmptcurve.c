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





namespace GMlib {


  template <typename T>
  inline
  PTCurve<T>::PTCurve( PCurve<T,3>* pcA, PCurve<T,3>* pcB ) {

    _angle      = T(0);
    _pcA        = pcA;
    _pcB        = pcB;
    _resampleA  = true;
    _resampleB  = true;
  }


  template <typename T>
  inline
  PTCurve<T>::PTCurve( const PTCurve<T>& ptc ) {

    _angle      = ptc._angle;

    _pcA        = ptc._pcA;
    _pcB        = ptc._pcB;

    _pA         = ptc._pA;
    _pB         = ptc._pB;

    _resampleA  = ptc._resampleA;
    _resampleB  = ptc._resampleB;
  }


  template <typename T>
  inline
  void PTCurve<T>::_compute( DVector<DVector<Vector<T,3> > >& p ) {

    for(int i = 0; i < p.getDim(); i++) {
      _merge_PA_and_PB( i );
      p[i][0] = _rotate_and_project();
    }
  }


  template <typename T>
  inline
  void PTCurve<T>::_merge_PA_and_PB( int i ) {

    _tPoints[0] = _pA[i][0][0];
    _tPoints[1] = _pA[i][0][1];
    _tPoints[2] = _pA[i][0][2];
    _tPoints[3] = _pB[i][0][0];
    _tPoints[4] = _pB[i][0][1];
    _tPoints[5] = _pB[i][0][2];
  }


  template <typename T>
  inline
  void PTCurve<T>::_resample( PCurve<T,3>* obj, DVector<DVector<Vector<T,3> > >& p, int m, int d ) {

    p.setDim( m );

    T st = obj->getParStart();
    T en = obj->getParEnd();
    T dt = ( en - st ) / ( m - 1 );

    for(int i = 0; i < m - 1; i++)
      p[i] = obj->evaluateLocal( st + i * dt, d );

    p[m-1] = obj->evaluateLocal( en, d );
  }


  template <typename T>
  inline
  Point<T, 3> PTCurve<T>::_rotate_and_project() {

    Point<T, 3> p;
    Point<T, 6> tA, tB;
    double sphi = sin(_angle);
    double cphi = cos(_angle);
    int dim     = 6;

    for(int i = 0; i < dim; i++) {
      tB[i] = _tPoints(i);
      tA[i] = tB[i];
    }

    for(int i = 0; i < dim - 1; i++) {
      for(int j = i + 1; j < dim; j++) {
        tB[i] = cphi * tA[i] - sphi * tA[j];
        tB[j] = sphi * tA[i] + cphi * tA[j];

        for(int k = 0; k < dim; k++)
          tA[k] = tB[k];
      }
    }

    for(int i = 0; i < 3; i++)
      p[i] = tA[i];

    return p;
  }


  template <typename T>
  bool PTCurve<T>::isClosed() const {

    return false;
  }


  template <typename T>
  inline
  void PTCurve<T>::localSimulate( double dt ) {

    setAngle( dt );
    this->replot();
  }


  template <typename T>
  inline
  void PTCurve<T>::resample( DVector<DVector<Vector<T,3> > >& p, int m, int d, T start, T end ) {

    if( _resampleA )
      _resample( _pcA, _pA, m ,d );

    if( _resampleB )
      _resample( _pcB, _pB, m, d );

    p.setDim(m);
    _compute( p );
  }


  template <typename T>
  inline
  void PTCurve<T>::setAngle( T a ) {

    _angle += a;
    if(_angle >= M_2PI)
      _angle -= M_2PI;
  }


  template <typename T>
  inline
  void PTCurve<T>::setPA( PCurve<T,3>* pcA ) {

    _pcA        = pcA;
    _resampleA  = true;
  }


  template <typename T>
  inline
  void PTCurve<T>::setPB( PCurve<T,3>* pcB ) {

    _pcB        = pcB;
    _resampleB  = true;
  }
}
