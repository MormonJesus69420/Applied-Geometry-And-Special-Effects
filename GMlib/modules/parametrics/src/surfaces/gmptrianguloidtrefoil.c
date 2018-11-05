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


//*****************************************
// Constructors and destructor           **
//*****************************************

  template <typename T>
  inline
  PTrianguloidTrefoil<T>::PTrianguloidTrefoil() {
    init();
  }


  template <typename T>
  inline
  PTrianguloidTrefoil<T>::PTrianguloidTrefoil( const PTrianguloidTrefoil<T>& copy ) : PSurf<T,3>( copy ) {
    init();
  }


  template <typename T>
  PTrianguloidTrefoil<T>::~PTrianguloidTrefoil() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PTrianguloidTrefoil<T>::isClosedU() const {
    return true;
  }


  template <typename T>
  bool PTrianguloidTrefoil<T>::isClosedV() const {
    return true;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PTrianguloidTrefoil<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );


    T su = sin(u);
    T cu = cos(u);
    T s2u = sin(2*u);
    T s3u = sin(3*u);
    T c2u = cos(2*u);
    T c3u = cos(3*u);

    T sv = sin(v);
    T cv = cos(v);
    T cvp2 = cv+2;
    T sv23 = sin(v+M_2PI_3);
    T cv23 = cos(v+M_2PI_3);
    T cv23p2 = cv23+2;

    DVector< Point<T,3> > fu( 1 );
    DVector< Point<T,3> > fv( 1 );

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      fu.setDim( d1+1 );
      fv.setDim( d2+1 );

      switch(d1)
      {
      case 3:  fu[3] = Point<T,3>(-54*c3u,-2*(cu+16*c2u), su-16*s2u);
      case 2:  fu[2] = Point<T,3>(-18*s3u,-2*(su+8*s2u), -cu+8*c2u);
      case 1:  fu[1] = Point<T,3>( 6*c3u,  2*(cu+4*c2u), -su+4*s2u);
      default: fu[0] = Point<T,3>( 2*s3u,  2*(su+2*s2u),  cu-2*c2u);
      }

      switch(d2)
      {
      case 3:  fv[3] = Point<T,3>(6*(sv*cv/pow(cvp2,3)+pow(sv,3)/pow(cvp2,4))+sv/(cvp2*cvp2),
                            6*(sv23*cv23/pow(cv23p2,3)+pow(sv23,3)/pow(cv23p2,4))+sv23/(cv23p2*cv23p2),
                            (sv*cv23p2+3*cv*sv23+3*sv*cv23+sv23*cvp2)/4);
      case 2:  fv[2] = Point<T,3>(2*sv*sv/pow(cvp2,3)+cv/(cvp2*cvp2),
                            2*(sv23*sv23/pow(cv23p2,3))+cv23/(cv23p2*cv23p2),
                            -(cv*cv23p2-2*sv*sv23+cvp2*cv23)/4);
      case 1:  fv[1] = Point<T,3>( sv/(cvp2*cvp2),  sv23/(cv23p2*cv23p2), -(sv*cv23p2+cvp2*sv23)/4);
      default: fv[0] = Point<T,3>( 1/cvp2,  1/cv23p2,  cvp2*cv23p2/4);
      }
    }
    else {

      fu[0] = Point<T,3>( 2*s3u,  2*(su+2*s2u),  cu-2*c2u);
      fv[0] = Point<T,3>( 1/cvp2,  1/cv23p2,  cvp2*cv23p2/4);
    }

    for(int i=0; i<fu.getDim();i++)
      for(int j=0; j<fv.getDim(); j++)
        this->_p[i][j] = fu[i]%fv[j];
  }



  template <typename T>
  T PTrianguloidTrefoil<T>::getStartPU() const {
    return -T(M_PI);
  }


  template <typename T>
  T PTrianguloidTrefoil<T>::getEndPU() const {
    return T(M_PI);
  }


  template <typename T>
  T PTrianguloidTrefoil<T>::getStartPV() const {
    return -T(M_PI);
  }


  template <typename T>
  T PTrianguloidTrefoil<T>::getEndPV() const {
    return T(M_PI);
  }




  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PTrianguloidTrefoil<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
