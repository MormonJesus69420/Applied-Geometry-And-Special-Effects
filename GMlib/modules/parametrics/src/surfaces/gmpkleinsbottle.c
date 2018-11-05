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
  PKleinsBottle<T>::PKleinsBottle( T radius, T scale_x, T scale_y ) {

    init();

    _r = radius;
    _sx = scale_x;
    _sy = scale_y;

  }


  template <typename T>
  inline
  PKleinsBottle<T>::PKleinsBottle( const PKleinsBottle<T>& copy ) : PSurf<T,3>( copy ) {

    init();

    _r = copy._r;
    _sx = copy._sx;
    _sy = copy._sy;
  }



  template <typename T>
  PKleinsBottle<T>::~PKleinsBottle() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PKleinsBottle<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PKleinsBottle<T>::isClosedV() const {
    return false;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PKleinsBottle<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );


    if(u>=0.0 && u<=M_PI) this->_p[0][0][0] = _sx*cos(u)*(T(1)+sin(u))+_r*(T(1)-cos(u)*T(.5))*cos(u)*cos(v);
    else				  this->_p[0][0][0] = _sx*cos(u)*(T(1)+sin(u))+_r*(T(1)-cos(u)*T(.5))*cos(v+M_PI);
    if(u>=0.0 && u<=M_PI) this->_p[0][0][1] = _sy*sin(u)+_r*(T(1)-cos(u)*T(.5))*sin(u)*cos(v);
    else				  this->_p[0][0][1] = _sy*sin(u);
                this->_p[0][0][2] = _r*(T(1)-cos(u)*T(.5))*sin(v);


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1)	//u
      {
        if(u>=0.0 && u<=M_PI) this->_p[1][0][0] = -_sx*sin(u)*T(1)-_sx+T(2)*_sx*cos(u)*cos(u)
                           +T(2)*_r*sin(u)*T(.5)*cos(u)*cos(v)-_r*sin(u)*cos(v)*T(1);
        else				  this->_p[1][0][0] = -_sx*sin(u)*T(1)-_sx+T(2)*_sx*cos(u)*cos(u)
                           +_r*sin(u)*T(.5)*cos(v+M_PI);
        if(u>=0.0 && u<=M_PI) this->_p[1][0][1] = _sy*cos(u)+_r*T(.5)*cos(v)-T(2)*_r*cos(u)*cos(u)*cos(v)*T(.5)
                           +_r*cos(u)*cos(v)*T(1);
        else				  this->_p[1][0][1] = _sy*cos(u);
                    this->_p[1][0][2] = _r*sin(u)*T(.5)*sin(v);
      }

      if(d1>1)//uu
      {
        if(u>=0.0 && u<=M_PI) this->_p[2][0][0] = -_sx*cos(u)*T(1)-T(4)*_sx*sin(u)*cos(u)+T(4)*_r*cos(u)*cos(u)*T(.5)*cos(v)
                           -T(2)*_r*T(.5)*cos(v)-_r*cos(u)*cos(v)*T(1);
        else				  this->_p[2][0][0] = -cos(u)*(_sx*T(1)+T(4)*_sx*sin(u)-_r*T(.5)*cos(v+M_PI));
        if(u>=0.0 && u<=M_PI) this->_p[2][0][1] = -sin(u)*(_sy-T(4)*_r*cos(v)*cos(u)*T(.5)+_r*cos(v)*T(1));
        else				  this->_p[2][0][1] = -_sy*sin(u);
                    this->_p[2][0][2] = _r*cos(u)*T(.5)*sin(v);
      }

      if(d2)	//v
      {
        if(u>=0.0 && u<=M_PI) this->_p[0][1][0] = -_r*(T(1)-cos(u)*T(.5))*cos(u)*sin(v);
        else				  this->_p[0][1][0] = -_r*(T(1)-cos(u)*T(.5))*sin(v+M_PI);
        if(u>=0.0 && u<=M_PI) this->_p[0][1][1] = -_r*(T(1)-cos(u)*T(.5))*sin(u)*sin(v);
        else				  this->_p[0][1][1] = T(0);
                    this->_p[0][1][2] = _r*(T(1)-cos(u)*T(.5))*cos(v);

      }

      if(d2>1) //vv
      {
        if(u>=0.0 && u<=M_PI) this->_p[0][2][0] = -_r*(T(1)-cos(u)*T(.5))*cos(u)*cos(v);
        else				  this->_p[0][2][0] = _r*(-T(1)+cos(u)*T(.5))*cos(v+M_PI);
        if(u>=0.0 && u<=M_PI) this->_p[0][2][1] = -_r*(T(1)-cos(u)*T(.5))*sin(u)*cos(v);
        else				  this->_p[0][2][1] = T(0);
                    this->_p[0][2][2] = _r*(-T(1)+cos(u)*T(.5))*sin(v);
      }

      if(d1 && d2) //uv
      {
        if(u>=0.0 && u<=M_PI) this->_p[1][1][0] = -_r*sin(u)*sin(v)*(T(2)*cos(u)*T(.5)-T(1));
        else				  this->_p[1][1][0] = -_r*sin(u)*T(.5)*sin(v+M_PI);
        if(u>=0.0 && u<=M_PI) this->_p[1][1][1] = _r*sin(v)*(-T(.5)+cos(u)*cos(u)-cos(u));
        else				  this->_p[1][1][1] = T(0);
                    this->_p[1][1][2] = _r*sin(u)*T(.5)*cos(v);
      }

      if(d1>1 && d2)//uuv
      {
        if(u>=0.0 && u<=M_PI) this->_p[2][1][0] = -_r*sin(v)*(T(2)*cos(u)*cos(u)-2.0*T(.5)-cos(u));
        else				  this->_p[2][1][0] = -_r*cos(u)*T(.5)*sin(v+M_PI);
        if(u>=0.0 && u<=M_PI) this->_p[2][1][1] = -_r*sin(u)*sin(v)*(T(2)*cos(u)-T(1));
        else				  this->_p[2][1][1] = T(0);
                    this->_p[2][1][2] =  _r*cos(u)*T(.5)*cos(v);
      }

      if(d1 && d2>1) //uvv
      {
        if(u>=0.0 && u<=M_PI) this->_p[1][2][0] = -_r*sin(u)*cos(v)*(cos(u)-T(1));
        else				  this->_p[1][2][0] = -_r*sin(u)*T(.5)*cos(v+M_PI);
        if(u>=0.0 && u<=M_PI) this->_p[1][2][1] = _r*cos(v)*(-T(.5)+cos(u)*cos(u)-cos(u));
        else				  this->_p[1][2][1] = T(0);
                    this->_p[1][2][2] = -_r*sin(u)*T(.5)*sin(v);
      }

      if(d1>1 && d2>1) //uuvv
      {
        if(u>=0.0 && u<=M_PI) this->_p[2][2][0] = -_r*cos(v)*(T(2)*cos(u)*cos(u)-T(1)-cos(u));
        else				  this->_p[2][2][0] = -_r*cos(u)*T(.5)*cos(v+M_PI);
        if(u>=0.0 && u<=M_PI) this->_p[2][2][1] = -_r*sin(u)*cos(v)*(T(2)*cos(u)-T(1));
        else				  this->_p[2][2][1] = T(0);
                    this->_p[2][2][2] = -_r*cos(u)*T(.5)*sin(v);

      }
    }
  }



  template <typename T>
  T PKleinsBottle<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PKleinsBottle<T>::getEndPU() const {
    return T(2*M_PI);
  }


  template <typename T>
  T PKleinsBottle<T>::getStartPV() const {
    return T(0);
  }


  template <typename T>
  T PKleinsBottle<T>::getEndPV() const {
    return T(2*M_PI);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PKleinsBottle<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
