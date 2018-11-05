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
  PSlippersSurface<T>::PSlippersSurface() {

    init();
  }


  template <typename T>
  inline
  PSlippersSurface<T>::PSlippersSurface( const PSlippersSurface<T>& copy ) : PSurf<T,3>( copy ) {

    init();
  }



  template <typename T>
  PSlippersSurface<T>::~PSlippersSurface() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PSlippersSurface<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PSlippersSurface<T>::isClosedV() const {
    return false;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PSlippersSurface<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );


    this->_p[0][0][0] =  (T(2) + cos(u))*pow((T)cos(v),T(3))* sin(v);
    this->_p[0][0][1] =	(T(2) + cos(u+T(2)*M_PI/T(3)))*pow((T)cos(T(2)*M_PI/T(3)+v),T(2))*pow((T)sin(T(2)*M_PI/T(3)+v),T(2));
    this->_p[0][0][2] =	-(T(2) + cos(u-T(2)*M_PI/T(3)))*pow((T)cos(T(2)*M_PI/T(3)-v),T(2))*pow((T)sin(T(2)*M_PI/T(3)-v),T(3));


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) //u
      {
        this->_p[1][0][0] =	-sin(u)*pow((T)cos(v),T(3))*sin(v);
        this->_p[1][0][1] =	-sin(u+T(2)*M_PI/T(3))*pow((T)cos(v+T(2)*M_PI/T(3)),T(2))*pow((T)sin(v+T(2)*M_PI/T(3)),T(2));
        this->_p[1][0][2] =	-sin(-u+T(2)*M_PI/T(3))*pow((T)cos(T(2)*M_PI/T(3)-v),T(2))*pow((T)sin(T(2)*M_PI/T(3)-v),T(3));
      }
      if(d1>1)//uu
      {
        this->_p[2][0][0] =	-cos(u)*pow(cos(v),T(3.0))*sin(v);
        this->_p[2][0][1] =	cos(u+T(2.0)/T(3.0)*T(M_PI))*pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(2.0))
                *(-T(1.0)+pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(2.0)));
        this->_p[2][0][2] =	cos(u-T(2.0)/T(3.0)*T(M_PI))*pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(2.0))
                *(-T(1.0)+pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(2.0)))*sin(-T(2.0)/T(3.0)*T(M_PI)+v);
      }

      if(d2) //v
      {
        this->_p[0][1][0] =	-T(3)*(T(2)+cos(u))*cos(v)*cos(v)*sin(v)*sin(v)+(T(2)+cos(u))*pow((T)cos(v),T(4));
        this->_p[0][1][1] =	-T(2)*(T(2)+cos(u+T(2)/T(3)*M_PI))*cos(v+T(2)/T(3)*M_PI)*pow((T)sin(v+T(2)/T(3)*M_PI),T(3))
                +T(2)*(T(2)+cos(u+T(2)/T(3)*M_PI))*pow((T)cos(v+T(2)/T(3)*M_PI),T(3))*sin(v+T(2)/T(3)*M_PI);
        this->_p[0][1][2] =	-T(2)*(T(2)+cos(-u+T(2)/T(3)*M_PI))*cos(T(2)/T(3)*M_PI-v)*pow((T)sin(T(2)/T(3)*M_PI-v),T(4))
                +T(3)*(T(2)+cos(-u+T(2)/T(3)*M_PI))*pow((T)cos(T(2)/T(3)*M_PI-v),T(3))*pow((T)sin(T(2)
                /T(3)*M_PI-v),T(2));
      }
      if(d2>1) //vv
      {
        this->_p[0][2][0] =	-T(2.0)*cos(v)*sin(v)*(-6.0+16.0*pow(cos(v),T(2.0))-T(3.0)*cos(u)+8.0*pow(cos(v),T(2.0))*cos(u));
        this->_p[0][2][1] =	T(4.0)-32.0*pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(2.0))+32.0*pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(4.0))
                +T(2.0)*cos(u+T(2.0)/T(3.0)*T(M_PI))-16.0*cos(u+T(2.0)/T(3.0)*T(M_PI))
                *pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(2.0))+16.0*pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(4.0))*cos(u+T(2.0)/T(3.0)*T(M_PI));
        this->_p[0][2][2] =	sin(-T(2.0)/T(3.0)*T(M_PI)+v)*(T(4.0)-42.0*pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(2.0))+50.0
                *pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(4.0))+T(2.0)*cos(u-T(2.0)/T(3.0)*T(M_PI))-21.0*cos(u-T(2.0)/T(3.0)*T(M_PI))
                *pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(2.0))+25.0*pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(4.0))*cos(u-T(2.0)/T(3.0)*T(M_PI)));
      }

      if(d1 && d2) //uv
      {
        this->_p[1][1][0] =	-sin(u)*pow(cos(v),T(2.0))*(-T(3.0)+T(4.0)*pow(cos(v),T(2.0)));
        this->_p[1][1][1] =	-T(2.0)*sin(u+T(2.0)/T(3.0)*T(M_PI))*cos(T(2.0)/T(3.0)*T(M_PI)+v)*sin(T(2.0)/T(3.0)*T(M_PI)+v)*(-T(1.0)+T(2.0)
                *pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(2.0)));
        this->_p[1][1][2] =	sin(u-T(2.0)/T(3.0)*T(M_PI))*cos(-T(2.0)/T(3.0)*T(M_PI)+v)*(-T(1.0)+pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(2.0)))
                *(-T(2.0)+T(5.0)*pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(2.0)));
      }
      if(d1>1 && d2)//uuv
      {
        this->_p[2][1][0] =	-cos(u)*pow(cos(v),T(2.0))*(-T(3.0)+T(4.0)*pow(cos(v),T(2.0)));
        this->_p[2][1][1] =	-T(2.0)*cos(u+T(2.0)/T(3.0)*T(M_PI))*cos(T(2.0)/T(3.0)*T(M_PI)+v)*sin(T(2.0)/T(3.0)*T(M_PI)+v)
                *(-T(1.0)+T(2.0)*pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(2.0)));
        this->_p[2][1][2] =	cos(-T(2.0)/T(3.0)*T(M_PI)+v)*(-T(1.0)+pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(2.0)))*cos(u-T(2.0)/T(3.0)*T(M_PI))
                *(-T(2.0)+T(5.0)*pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(2.0)));
      }
      if(d1 && d2>1) //uvv
      {
        this->_p[1][2][0] =	T(2.0)*sin(u)*cos(v)*sin(v)*(-T(3.0)+8.0*pow(cos(v),T(2.0)));
        this->_p[1][2][1] =	-T(2.0)*sin(u+T(2.0)/T(3.0)*T(M_PI))*(T(1.0)-8.0
                *pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(2.0))+8.0*pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(4.0)));
        this->_p[1][2][2] =	-sin(u-T(2.0)/T(3.0)*T(M_PI))*sin(-T(2.0)/T(3.0)*T(M_PI)+v)*(T(2.0)-21.0
                *pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(2.0))+25.0*pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(4.0)));
      }
      if(d1>1 && d2>1) //uuvv
      {
        this->_p[2][2][0] =	T(2.0)*cos(u)*cos(v)*sin(v)*(-T(3.0)+8.0*pow(cos(v),T(2.0)));
        this->_p[2][2][1] =	-T(2.0)*cos(u+T(2.0)/T(3.0)*T(M_PI))*(T(1.0)-8.0*pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(2.0))
                +8.0*pow(cos(T(2.0)/T(3.0)*T(M_PI)+v),T(4.0)));
        this->_p[2][2][2] =	-cos(u-T(2.0)/T(3.0)*T(M_PI))*sin(-T(2.0)/T(3.0)*T(M_PI)+v)*(T(2.0)-21.0*pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(2.0))
                +25.0*pow(cos(-T(2.0)/T(3.0)*T(M_PI)+v),T(4.0)));
      }
    }
  }



  template <typename T>
  T PSlippersSurface<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PSlippersSurface<T>::getEndPU() const {
    return T(2*M_PI);
  }


  template <typename T>
  T PSlippersSurface<T>::getStartPV() const {
    return T(0);
  }


  template <typename T>
  T PSlippersSurface<T>::getEndPV() const {
    return T(2*M_PI);
  }




  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PSlippersSurface<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
