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
  PKuenSurface<T>::PKuenSurface( T radius ) {

    init();
    _r = radius;
  }


  template <typename T>
  inline
  PKuenSurface<T>::PKuenSurface( const PKuenSurface<T>& copy ) : PSurf<T,3>( copy ) {

    init();
    _r = copy._r;
  }



  template <typename T>
  PKuenSurface<T>::~PKuenSurface() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PKuenSurface<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PKuenSurface<T>::isClosedV() const {
    return false;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PKuenSurface<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );


    this->_p[0][0][0] =	T(2)*_r*(cos(u)+u*sin(u))*sin(v)/(T(1)+u*u*sin(v)*sin(v));
    this->_p[0][0][1] =	T(2)*_r*(sin(u)-u*cos(u))*sin(v)/(T(1)+u*u*sin(v)*sin(v));
    this->_p[0][0][2] =	_r*(log(tan(v/T(2)))+T(2)*cos(v)/(T(1)+u*u*sin(v)*sin(v)));


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) //u
      {
        this->_p[1][0][0] =	T(2)*_r*u*sin(v)*(cos(u)+cos(u)*u*u-cos(u)*u*u*cos(v)*cos(v)
                -T(2)*cos(u)+T(2)*cos(u)*cos(v)*cos(v)-T(2)*u*sin(u)+T(2)*u*sin(u)*cos(v)*cos(v))
                /pow((T)T(1.0)+u*u*sin(v)*sin(v),T(2));
        this->_p[1][0][1] =	-T(T(2))*_r*u*sin(v)*(-sin(u)-sin(u)*u*u+sin(u)*u*u*cos(v)*cos(v)
                +T(2)*sin(u)-T(2)*sin(u)*cos(v)*cos(v)-T(2)*u*cos(u)+T(2)*u*cos(u)*cos(v)*cos(v))
                /pow((T)T(1.0)+u*u*sin(v)*sin(v),T(2));
        this->_p[1][0][2] =	T(2)*_r*T(T(2))*cos(v)*u*(-1.0+cos(v)*cos(v))/pow((T)T(1.0)+u*u*sin(v)*sin(v),T(2));
      }
      if(d1>1)//uu
      {
        this->_p[2][0][0] =	-T(2)*_r*sin(v)*(-cos(u)+T(2)*cos(u)*u*u-T(2)*cos(u)*u*u*cos(v)*cos(v)
                +3.0*cos(u)*u*u*u*u-6.0*cos(u)*u*u*u*u*cos(v)*cos(v)+3.0*cos(u)*u*u*u*u*pow((T)cos(v),T(4))
                +u*sin(u)+T(2)*u*u*u*sin(u)-T(2)*u*u*u*sin(u)*cos(v)*cos(v)
                +u*u*u*u*u*sin(u)-T(2)*u*u*u*u*u*sin(u)*cos(v)*cos(v)+u*u*u*u*u*sin(u)*pow((T)cos(v),T(4))
                -6.0*cos(u)*u*u+T(12)*cos(u)*u*u*cos(v)*cos(v)-6.0*u*u*cos(u)*pow((T)cos(v),T(4))-6.0*u*u*u*sin(u)
                +T(12)*u*u*u*sin(u)*cos(v)*cos(v)-6.0*u*u*u*sin(u)*pow((T)cos(v),T(4))+T(2)*cos(u)
                -T(2)*cos(u)*cos(v)*cos(v)+T(2)*u*sin(u)-T(2)*u*sin(u)*cos(v)*cos(v))
                /pow((T)T(1.0)+u*u*sin(v)*sin(v),T(3));
        this->_p[2][0][1] =	-T(2)*_r*sin(v)*(-sin(u)*pow(T(1.0),T(2))+T(2)*sin(u)*u*u-T(2)*sin(u)*u*u*cos(v)*cos(v)
                +3.0*sin(u)*u*u*u*u-6.0*sin(u)*u*u*u*u*cos(v)*cos(v)+3.0*sin(u)*u*u*u*u*pow((T)cos(v),T(4))
                -u*cos(u)*pow(T(1.0),T(2))-T(2)*u*u*u*cos(u)+T(2)*u*u*u*cos(u)*cos(v)*cos(v)
                -u*u*u*u*u*cos(u)+T(2)*u*u*u*u*u*cos(u)*cos(v)*cos(v)-u*u*u*u*u*cos(u)*pow((T)cos(v),T(4))
                -6.0*sin(u)*u*u+T(12)*sin(u)*u*u*cos(v)*cos(v)-6.0*u*u*sin(u)*pow((T)cos(v),T(4))+6.0*u*u*u*cos(u)
                -T(12)*u*u*u*cos(u)*cos(v)*cos(v)+6.0*u*u*u*cos(u)*pow((T)cos(v),T(4))+T(2)*sin(u)
                -T(2)*sin(u)*cos(v)*cos(v)-T(2)*u*cos(u)+T(2)*u*cos(u)*cos(v)*cos(v))
                /pow((T)T(1.0)+u*u*sin(v)*sin(v),T(3));
        this->_p[2][0][2] =	T(2)*_r*T(2)*cos(v)*(-1.0+cos(v)*cos(v))*(-3.0*u*u+3.0*u*u*cos(v)*cos(v)+T(1.0))
                /pow((T)T(1.0)+u*u*sin(v)*sin(v),T(3));
      }
      if(d2) //v
      {
        this->_p[0][1][0] =	T(T(2))*_r*(cos(u)+u*sin(u))*cos(v)*(T(1.0)-u*u+u*u*cos(v)*cos(v))
                /pow((T)T(1.0)+u*u*sin(v)*sin(v),T(2));
        this->_p[0][1][1] =	T(T(2))*_r*(sin(u)-u*cos(u))*cos(v)*(T(1.0)-u*u+u*u*cos(v)*cos(v))
                /pow((T)T(1.0)+u*u*sin(v)*sin(v),T(2));
        this->_p[0][1][2] =	-_r*(-T(1)-T(2)*u*u+T(2)*u*u*cos(v)*cos(v)
                -u*u*u*u+T(2)*u*u*u*u*cos(v)*cos(v)-u*u*u*u*pow((T)cos(v),T(4))
                +pow(T(T(2)),T(2))*sin(v)*cos(T(.5)*v)*sin(T(.5)*v)
                +pow(T(T(2)),T(2))*sin(v)*cos(T(.5)*v)*sin(T(.5)*v)*u*u
                +pow(T(T(2)),T(2))*cos(v)*cos(v)*u*u*sin(v)*cos(T(.5)*v)*sin(T(.5)*v))
                /cos(T(.5)*v)/T(T(2))/sin(T(.5)*v)/pow((T)T(1.0)+u*u*sin(v)*sin(v),T(2));
      }
      if(d2>1) //vv
      {
        this->_p[0][2][0] =	-T(2.0)*_r*(cos(u)+u*sin(u))*sin(v)*(T(1)-u*u*u*u+u*u*u*u*pow((T)cos(v),T(4))
                +6.0*u*u*cos(v)*cos(v))/pow((T)T(1.0)+u*u*sin(v)*sin(v),T(3));
        this->_p[0][2][1] =	-T(2.0)*_r*(sin(u)-u*cos(u))*sin(v)*(T(1)-u*u*u*u+u*u*u*u*pow((T)cos(v),T(4))
                +6.0*u*u*cos(v)*cos(v))/pow((T)T(1.0)+u*u*sin(v)*sin(v),T(3));

            T s6 =	3.0*u*u*u*u*pow((T)cos(v),T(4))+6.0*u*u*u*u*u*u*cos(T(.5)*v)*cos(T(.5)*v)*cos(v)*cos(v)
                -3.0*u*u*cos(v)*cos(v)-6.0*u*u*u*u*u*u*cos(T(.5)*v)*cos(T(.5)*v)*pow((T)cos(v),T(4))
                +2.0*u*u*u*u*u*u*cos(T(.5)*v)*cos(T(.5)*v)*pow((T)cos(v),T(6))-6.0*u*u*cos(T(.5)*v)*cos(T(.5)*v)
                +u*u*u*u*u*u-6.0*u*u*u*u*cos(T(.5)*v)*cos(T(.5)*v)+3.0*u*u
                +T(1)+4.0*T(8)*cos(v)*cos(T(.5)*v)*cos(T(.5)*v)*u*u
                -6.0*T(8)*pow((T)cos(v),T(3))*cos(T(.5)*v)*cos(T(.5)*v)*u*u-4.0*T(8)*cos(v)
                *pow((T)cos(T(.5)*v),T(4))*u*u+6.0*T(8)*pow((T)cos(v),T(3))*pow((T)cos(T(.5)*v),T(4))
                *u*u-2.0*u*u*u*u*u*u*cos(T(.5)*v)*cos(T(.5)*v)+T(8)
                *cos(v)*pow((T)cos(T(.5)*v),T(4));

            T s5 =	s6-4.0*T(8)*pow((T)cos(v),T(3))*cos(T(.5)*v)*cos(T(.5)*v)*u*u*u*u
                -T(8)*pow((T)cos(v),T(5))*cos(T(.5)*v)*cos(T(.5)*v)*u*u*u*u-5.0*T(8)*cos(v)
                *pow((T)cos(T(.5)*v),T(4))*u*u*u*u+4.0*T(8)*pow((T)cos(v),T(3))*pow((T)cos(T(.5)*v),T(4))
                *u*u*u*u+T(8)*pow((T)cos(v),T(5))*pow((T)cos(T(.5)*v),T(4))*u*u*u*u+6.0
                *u*u*cos(T(.5)*v)*cos(T(.5)*v)*cos(v)*cos(v)+12.0*u*u*u*u*cos(T(.5)*v)*cos(T(.5)*v)
                *cos(v)*cos(v)-6.0*u*u*u*u*cos(T(.5)*v)*cos(T(.5)*v)*pow((T)cos(v),T(4))-T(8)
                *cos(v)*cos(T(.5)*v)*cos(T(.5)*v)+5.0*T(8)*cos(v)
                *cos(T(.5)*v)*cos(T(.5)*v)*u*u*u*u-2.0*cos(T(.5)*v)*cos(T(.5)*v)-3.0*u*u*u*u*u*u
                *cos(v)*cos(v)+3.0*u*u*u*u*u*u*pow((T)cos(v),T(4))-u*u*u*u*u*u*pow((T)cos(v),T(6))
                -6.0*u*u*u*u*cos(v)*cos(v)+3.0*u*u*u*u;

            T s3 =	_r*s5;

            T s2 =	1/cos(T(.5)*v)*cos(T(.5)*v)/T(4)/(-1.0+cos(T(.5)*v)*cos(T(.5)*v))
                /pow((T)T(1.0)+u*u*sin(v)*sin(v),T(3));

        this->_p[0][2][2] =	-s3*s2;
      }
      if(d1 && d2) //uv
      {
        this->_p[1][1][0] =	T(2.0)*_r*u*cos(v)*(cos(u)-cos(u)*u*u*u*u+2.0*cos(u)*u*u*u*u*cos(v)*cos(v)
                -cos(u)*u*u*u*u*pow((T)cos(v),T(4))-6.0*cos(u)+6.0*cos(u)*cos(v)*cos(v)
                +2.0*u*u*cos(u)-4.0*u*u*cos(u)*cos(v)*cos(v)+2.0*u*u*cos(u)*pow((T)cos(v),T(4))
                -6.0*u*sin(u)+6.0*u*sin(u)*cos(v)*cos(v)+2.0*u*u*u*sin(u)
                -4.0*u*u*u*sin(u)*cos(v)*cos(v)+2.0*u*u*u*sin(u)*pow((T)cos(v),T(4)))
                /pow((T)T(1.0)+u*u*sin(v)*sin(v),T(3));
        this->_p[1][1][1] =	-T(2.0)*_r*u*cos(v)*(-sin(u)+sin(u)*u*u*u*u-2.0*sin(u)*u*u*u*u*cos(v)*cos(v)
                +sin(u)*u*u*u*u*pow((T)cos(v),T(4))+6.0*sin(u)-6.0*sin(u)*cos(v)*cos(v)
                -2.0*u*u*sin(u)+4.0*u*u*sin(u)*cos(v)*cos(v)-2.0*u*u*sin(u)*pow((T)cos(v),T(4))
                -6.0*u*cos(u)+6.0*u*cos(u)*cos(v)*cos(v)+2.0*u*u*u*cos(u)
                -4.0*u*u*u*cos(u)*cos(v)*cos(v)+2.0*u*u*u*cos(u)*pow((T)cos(v),T(4)))
                /pow((T)T(1.0)+u*u*sin(v)*sin(v),T(3));
        this->_p[1][1][2] =	-2.0*_r*T(2.0)*sin(v)*u*(-T(1.0)+3.0*cos(v)*cos(v)-u*u+u*u*pow((T)cos(v),T(4)))
                /pow((T)T(1.0)+u*u*sin(v)*sin(v),T(3));

      }
      if(d1>1 && d1)//uuv
      {
        //s1 = T(2.0)*_r;
        //s3 = cos(v);

        T s6 =	cos(u)-6.0*cos(u)*-18.0*cos(u)*u*u*u*
            u*pow((T)cos(v),T(4))-6.0*cos(u)*u*u*u*u+3.0*cos(u)*u*u*u*u*u*u+u*u*u*u*u*u*u*sin(u)
            -6.0*u*u*u*u*u*sin(u)+6.0*u*sin(u)*cos(v)*cos(v)+18.0*cos(u)*u*u*u*u*cos(v)*cos(v)
            -9.0*cos(u)*u*u*u*u*pow((T)cos(v),T(4))+11.0*cos(u)*u*u*cos(v)*cos(v)
            +u*u*u*sin(u)*cos(v)*cos(v)-2.0*u*u*u*u*u*sin(u)*cos(v)*cos(v)+u*u*u*u*u*sin(u)
            *pow((T)cos(v),T(4))-72.0*u*u*cos(u)*cos(v)*cos(v)+36.0*u*u*cos(u)*pow((T)cos(v),T(4))
            -72.0*u*u*u*sin(u)*cos(v)*cos(v)+36.0*u*u*u*sin(u)*pow((T)cos(v),T(4))-u*sin(u);

        T s5 =	s6+18.0*cos(u)*u*u*u*u*cos(v)*cos(v)+6.0*u*u*u*u*cos(u)*pow((T)cos(v),T(6))-3.0*u*u*u*u*u*u*u
            *sin(u)*cos(v)*cos(v)+3.0*u*u*u*u*u*u*u*sin(u)*pow((T)cos(v),T(4))-u*u*u*u*u*u*u*sin(u)
            *pow((T)cos(v),T(6))+6.0*cos(u)*cos(v)*cos(v)-9.0*cos(u)*u*u*u*u*u*u*cos(v)*cos(v)+9.0*cos(u)
            *u*u*u*u*u*u*pow((T)cos(v),T(4))-3.0*cos(u)*u*u*u*u*u*u*pow((T)cos(v),T(6))+18.0*u*u*u*u*u*sin(u)
            *cos(v)*cos(v)-18.0*u*u*u*u*u*sin(u)*pow((T)cos(v),T(4))+6.0*u*u*u*u*u*sin(u)*pow((T)cos(v),T(6))
            +36.0*u*u*cos(u)+36.0*u*u*u*sin(u)-9.0*cos(u)*u*u*u*u-11.0*cos(u)*u*u-u*u*u*sin(u)
            +u*u*u*u*u*sin(u)-6.0*u*sin(u);

        s6 = 1/(pow((T)T(1)+u*u*pow((T)sin(v),T(2)),T(4)));

        //s4 = s5*s6;

        //s2 = cos(v)*s5*s6;

        this->_p[2][1][0] =	T(2.0)*_r*cos(v)*s5*s6;

        //  s1 = T(2.0)*_r;
        //  s3 = cos(v);
        s6 =	-9.0*sin(u)*u*u*u*u*pow((T)cos(v),T(4))-6.0*u*cos(u)*cos(v)*cos(v)
            +11.0*sin(u)*u*u*cos(v)*cos(v)+18.0*sin(u)*u*u*u*u*cos(v)*cos(v)+2.0*u*u*u*u*u
            *cos(u)*cos(v)*cos(v)-u*u*u*u*u*cos(u)*pow((T)cos(v),T(4))+36.0*u*u*sin(u)
            *pow((T)cos(v),T(4))-u*u*u*cos(u)*cos(v)*cos(v)+u*cos(u)-72.0*u*u*sin(u)*cos(v)*cos(v)
            +3.0*sin(u)*u*u*u*u*u*u-36.0*u*u*u*cos(u)*pow((T)cos(v),T(4))+6.0*u*cos(u)
            -u*u*u*u*u*u*u*cos(u)+6.0*u*u*u*u*u*cos(u)+72.0*u*u*u*cos(u)*cos(v)*cos(v)+sin(u)
            -36.0*u*u*u*cos(u)+3.0*u*u*u*u*u*u*u*cos(u)*cos(v)*cos(v);

        s5 = s6-11.0*sin(u)*u*u-6.0*sin(u)-9.0*sin(u)*u*u*u*u-u*u*u*u*u*cos(u)+u*u*u*cos(u)
           +36.0*u*u*sin(u)-3.0*u*u*u*u*u*u*u*cos(u)*pow((T)cos(v),T(4))+u*u*u*u*u*u*u
           *cos(u)*pow((T)cos(v),T(6))-9.0*sin(u)*u*u*u*u*u*u*cos(v)*cos(v)+9.0*sin(u)*u*u*u*u*u*u
                 *pow((T)cos(v),T(4))-3.0*sin(u)*u*u*u*u*u*u*pow((T)cos(v),T(6))+6.0*sin(u)*cos(v)*cos(v)
           -18.0*u*u*u*u*u*cos(u)*cos(v)*cos(v)-6.0*sin(u)*u*u*u*u+18.0*sin(u)*u*u*u*u*cos(v)*cos(v)
           -18.0*sin(u)*u*u*u*u*pow((T)cos(v),T(4))+18.0*u*u*u*u*u*cos(u)*pow((T)cos(v),T(4))
           -6.0*u*u*u*u*u*cos(u)*pow((T)cos(v),T(6))+6.0*u*u*u*u*sin(u)*pow((T)cos(v),T(6));

        s6 = 1/(pow((T)T(1)+u*u*pow((T)sin(v),T(2)),T(4)));

      //	s4 = ;
          //s2 = cos(v)*s4;
         // t0 =
        this->_p[2][1][1] =	T(2.0)*_r*cos(v)*s5*s6;



        this->_p[2][1][2] =	-2.0*_r*T(2.0)*sin(v)*(2.0*u*u-20.0*u*u*cos(v)*cos(v)
                +18.0*u*u*pow((T)cos(v),T(4))+3.0*u*u*u*u-3.0*u*u*u*u*cos(v)*cos(v)
                -3.0*u*u*u*u*pow((T)cos(v),T(4))+3.0*u*u*u*u*pow((T)cos(v),T(6))-T(1)+3.0
                *cos(v)*cos(v))/pow((T)T(1)+u*u*pow((T)sin(v),T(2)),T(4));
      }
      if(d1 && d2>1) //uvv
      {
        this->_p[1][2][0] = -T(2.0)*_r*u*sin(v)*(cos(u)-6.0*cos(u)-2.0*cos(u)*u*u*u*u*pow((T)cos(v),T(4))
                +2.0*cos(u)*u*u*u*u-cos(u)*u*u*u*u*u*u+2.0*u*u*u*u*u*sin(u)+18.0*u*sin(u)*cos(v)*cos(v)
                +6.0*cos(u)*u*u*u*u*cos(v)*cos(v)-5.0*cos(u)*u*u*u*u*pow((T)cos(v),T(4))
                +5.0*cos(u)*u*u*cos(v)*cos(v)-24.0*u*u*cos(u)*cos(v)*cos(v)+28.0*u*u*cos(u)
                *pow((T)cos(v),T(4))-24.0*u*u*u*sin(u)*cos(v)*cos(v)+28.0*u*u*u*sin(u)
                *pow((T)cos(v),T(4))-2.0*cos(u)*u*u*u*u*cos(v)*cos(v)+2.0*u*u*u*u*cos(u)*pow((T)cos(v),T(6))
                +18.0*cos(u)*cos(v)*cos(v)+cos(u)*u*u*u*u*u*u*cos(v)*cos(v)+cos(u)*u*u*u*u*u*u
                *pow((T)cos(v),T(4))-cos(u)*u*u*u*u*u*u*pow((T)cos(v),T(6))-2.0*u*u*u*u*u*sin(u)*cos(v)*cos(v)
                -2.0*u*u*u*u*u*sin(u)*pow((T)cos(v),T(4))+2.0*u*u*u*u*u*sin(u)*pow((T)cos(v),T(6))
                -4.0*u*u*cos(u)-4.0*u*u*u*sin(u)-cos(u)*u*u*u*u+cos(u)*u*u
                -6.0*u*sin(u))/pow((T)T(1)+u*u*pow((T)sin(v),T(2)),T(4));

        this->_p[1][2][1] =	T(2.0)*_r*u*sin(v)*(5.0*sin(u)*u*u*u*u*pow((T)cos(v),T(4))+18.0*u*cos(u)
                *cos(v)*cos(v)-5.0*sin(u)*u*u*cos(v)*cos(v)-6.0*sin(u)*u*u*u*u*cos(v)*cos(v)
                -28.0*u*u*sin(u)*pow((T)cos(v),T(4))+24.0*u*u*sin(u)*cos(v)*cos(v)
                +sin(u)*u*u*u*u*u*u+28.0*u*u*u*cos(u)*pow((T)cos(v),T(4))-6.0*u*cos(u)
                +2.0*u*u*u*u*u*cos(u)-24.0*u*u*u*cos(u)*cos(v)*cos(v)-sin(u)-4.0*u*u*u*cos(u)
                -sin(u)*u*u+6.0*sin(u)+sin(u)*u*u*u*u+4.0*u*u*sin(u)-sin(u)*u*u*u*u*u*u*cos(v)*cos(v)
                -sin(u)*u*u*u*u*u*u*pow((T)cos(v),T(4))+sin(u)*u*u*u*u*u*u*pow((T)cos(v),T(6))-18.0*sin(u)
                *cos(v)*cos(v)-2.0*u*u*u*u*u*cos(u)*cos(v)*cos(v)-2.0*sin(u)*u*u*u*u+2.0*sin(u)*u*u*u*u
                *cos(v)*cos(v)+2.0*sin(u)*u*u*u*u*pow((T)cos(v),T(4))-2.0*u*u*u*u*u*cos(u)*pow((T)cos(v),T(4))
                +2.0*u*u*u*u*u*cos(u)*pow((T)cos(v),T(6))-2.0*u*u*u*u*sin(u)*pow((T)cos(v),T(6)))
                /pow((T)T(1)+u*u*pow((T)sin(v),T(2)),T(4));


        this->_p[1][2][2] =	-2.0*_r*T(2.0)*cos(v)*u*(-7.0+9.0*cos(v)*cos(v)-2.0*u*u
                -12.0*u*u*cos(v)*cos(v)+14.0*u*u*pow((T)cos(v),T(4))+5.0*u*u*u*u-9.0*u*u*u*u
                *cos(v)*cos(v)+3.0*u*u*u*u*pow((T)cos(v),T(4))+u*u*u*u*pow((T)cos(v),T(6)))
                /pow((T)T(1)+u*u*pow((T)sin(v),T(2)),T(4));

      }
      if(d1>1 && d2>1) //uuvv
      {
        T	s6 = 6.0*cos(u)-18.0*cos(v)*cos(v)*u*sin(u)+4.0*u*u*u*sin(u)*cos(v)*cos(v)-cos(u)
             -40.0*cos(u)*cos(v)*cos(v)*u*u+6.0*cos(u)*u*u*u*u*u*u-90.0*cos(u)*u*u*u*u
             *cos(v)*cos(v)+270.0*cos(u)*u*u*u*u*pow((T)cos(v),T(4))+240.0*cos(u)*u*u*cos(v)*cos(v)
             -12.0*cos(u)*u*u*u*u*u*u*cos(v)*cos(v)+12.0*cos(u)*u*u*u*u*u*u*pow((T)cos(v),T(6))
             -30.0*cos(u)*u*u*u*u-30.0*cos(u)*u*u+u*sin(u)-6.0*u*u*u*u*u*u
             *cos(u)*pow((T)cos(v),T(8))-12.0*u*u*u*u*u*u*u*sin(u)*cos(v)*cos(v)+12.0*u*u*u*u*u*u*u*sin(u)
             *pow((T)cos(v),T(6))-6.0*u*u*u*u*u*u*u*sin(u)*pow((T)cos(v),T(8))+10.0*u*u*u*u*u*sin(u)*cos(v)*cos(v)
             +36.0*cos(u)*cos(v)*cos(v)*u*u*u*u*u*u+240.0*u*u*u*sin(u)*cos(v)*cos(v)-210.0*u*u*u*sin(u)
             *pow((T)cos(v),T(4))-210.0*u*u*cos(u)*pow((T)cos(v),T(4))+6.0*u*u*u*u*u*u*u*sin(u)
             -150.0*u*u*u*u*cos(u)*pow((T)cos(v),T(6))+2.0*u*u*u*u*u*u*u*u*u*sin(u)*cos(v)*cos(v);

        T	s5 = s6+6.0*cos(u)*u*u*u*u*u*u-2.0*u*u*u*u*u*u*u*u*u*sin(u)*pow((T)cos(v),T(6))
             +u*u*u*u*u*u*u*u*u*sin(u)*pow((T)cos(v),T(8))-30.0*u*u*u*sin(u)*-90.0*u*u*u*u*u*sin(u)
             *cos(v)*cos(v)-10.0*u*u*u*u*u*u*u*sin(u)*pow((T)cos(v),T(4))+4.0*u*u*u*u*u*u*u*sin(u)
             *pow((T)cos(v),T(6))+8.0*u*u*u*u*u*u*u*sin(u)*cos(v)*cos(v)+10.0*cos(u)*u*u
             -30.0*u*u*u*u*u*sin(u)+6.0*cos(u)*cos(v)*cos(v)*u*u*u*u*u*u*u*u-2.0*u*u*u*u*u*u*u
             *sin(u)-6.0*cos(u)*pow((T)cos(v),T(6))*u*u*u*u*u*u*u*u+3.0*cos(u)*pow((T)cos(v),T(8))
             *u*u*u*u*u*u*u*u+20.0*cos(u)*u*u*u*u+6.0*u*sin(u)*+2.0*u*u*u*sin(u)-10.0*u*u*u*u*u*sin(u)
             *pow((T)cos(v),T(4))-90.0*cos(u)*pow((T)cos(v),T(4))*u*u*u*u*u*u+48.0*cos(u)
             *pow((T)cos(v),T(6))*u*u*u*u*u*u+270.0*u*u*u*u*u*sin(u)*pow((T)cos(v),T(4))
             -150.0*u*u*u*u*u*sin(u)*pow((T)cos(v),T(6))-10.0*cos(u)*u*u*u*u*cos(v)*cos(v)
             -10.0*cos(u)*u*u*u*u*pow((T)cos(v),T(4))-3.0*cos(u)*u*u*u*u*u*u*u*u-u*u*u*u*u*u*u*u*u*sin(u)
             -18.0*cos(v)*cos(v)*cos(u);

          s6 = 1/(pow((T)T(1.0)+u*u*pow((T)sin(v),T(2)),T(5)));

        this->_p[2][2][0] =	T(2.0)*_r*sin(v)*s5*s6;

        // s1 = -T(2.0)*_r;      s3 = sin(v);
          s6 = -270.0*sin(u)*u*u*u*u*pow((T)cos(v),T(4))-240.0*sin(u)*u*u*cos(v)*cos(v)
            +90.0*sin(u)*u*u*u*u*cos(v)*cos(v)-6.0*sin(u)*u*u*u*u*u*u+sin(u)-6.0*sin(u)
            -30.0*u*u*u*u*u*cos(u)-30.0*u*u*u*cos(u)-12.0*u*u*u*u*u*u*u*cos(v)*cos(v)*cos(u)
            -6.0*sin(u)*u*u*u*u*u*u-20.0*sin(u)*u*u*u*u-10.0*sin(u)*u*u-u*u*u*u*u*u*u*u*u*cos(u)
            +8.0*u*u*u*u*u*u*u*cos(u)*cos(v)*cos(v)*+10.0*sin(u)*cos(v)*cos(v)*u*u*u*u-36.0*sin(u)
            *cos(v)*cos(v)*u*u*u*u*u*u-90.0*u*u*u*u*u*cos(v)*cos(v)*cos(u)+240.0*u*u*u*cos(v)
            *cos(v)*cos(u)-150.0*u*u*u*u*u*pow((T)cos(v),T(6))*cos(u)*-10.0*u*u*u*u*u*u*u*cos(u)*pow((T)cos(v),T(4))
            +4.0*u*u*u*u*u*u*u*cos(u)*pow((T)cos(v),T(6))+150.0*u*u*u*u*pow((T)cos(v),T(6))*sin(u)-10.0*u*u*u*u*u*cos(u)
            *pow((T)cos(v),T(4))-210.0*u*u*u*pow((T)cos(v),T(4))*cos(u)+270.0*u*u*u*u*u*pow((T)cos(v),T(4))
            *cos(u)+210.0*u*u*pow((T)cos(v),T(4))*sin(u);

          s5 = 10.0*sin(u)*pow((T)cos(v),T(4))*u*u*u*u+90.0*sin(u)*pow((T)cos(v),T(4))*u*u*u*u*u*u*
            -48.0*sin(u)*pow((T)cos(v),T(6))*u*u*u*u*u*u*+10.0*u*u*u*u*u*cos(u)*cos(v)*cos(v)
            +6.0*u*u*u*u*u*u*u*cos(u)+3.0*sin(u)*u*u*u*u*u*u*u*u-2.0*u*u*u*u*u*u*u*u*u*cos(u)*pow((T)cos(v),T(6))
            +u*u*u*u*u*u*u*u*u*cos(u)*pow((T)cos(v),T(8))+6.0*u*u*u*u*u*u*sin(u)*pow((T)cos(v),T(8))+6.0*u*cos(u)
            +12.0*u*u*u*u*u*u*u*cos(u)*pow((T)cos(v),T(6))-6.0*u*u*u*u*u*u*u*cos(u)*pow((T)cos(v),T(8))
            +6.0*sin(u)*u*u*u*u*u*u*u*u*pow((T)cos(v),T(6))-3.0*sin(u)*u*u*u*u*u*u*u*u*pow((T)cos(v),T(8))
            +2.0*u*u*u*u*u*u*u*u*u*cos(u)*cos(v)*cos(v)-6.0*sin(u)*cos(v)*cos(v)*u*u*u*u*u*u*u*u
            +2.0*u*u*u*cos(u)-2.0*u*u*u*u*u*u*u*cos(u)+s6+30.0*sin(u)*u*u+30.0*sin(u)*u*u*u*u
            -18.0*cos(v)*cos(v)*u*cos(u)+4.0*u*u*u*cos(u)*cos(v)*cos(v)+40.0*sin(u)*cos(v)*cos(v)*u*u
            +12.0*sin(u)*u*u*u*u*u*u*cos(v)*cos(v)-12.0*sin(u)*u*u*u*u*u*u*pow((T)cos(v),T(6))
            +18.0*cos(v)*cos(v)*sin(u)+u*cos(u);

          s6 = 1/(pow((T)T(1.0)+u*u*pow((T)sin(v),T(2)),T(5)));

        this->_p[2][2][1] =	-T(2.0)*_r*sin(v)*s5*s6;

        this->_p[2][2][2] =	-2.0*_r*T(2.0)*cos(v)*(43.0*u*u-148.0*cos(v)*cos(v)*u*u+105.0*u*u*pow((T)cos(v),T(4))
                +35.0*u*u*u*u+5.0*cos(v)*cos(v)*u*u*u*u-115.0*u*u*u*u*pow((T)cos(v),T(4))
                +75.0*u*u*u*u*pow((T)cos(v),T(6))-15.0*u*u*u*u*u*u+42.0*cos(v)*cos(v)*u*u*u*u*u*u
                -36.0*u*u*u*u*u*u*pow((T)cos(v),T(4))+6.0*u*u*u*u*u*u*pow((T)cos(v),T(6))+3.0*u*u*u*u*u*u
                *pow((T)cos(v),T(8))-7.0+9.0*cos(v)*cos(v))/pow((T)T(1.0)+u*u*pow((T)sin(v),T(2)),T(5));

      }
    }
  }



  template <typename T>
  T PKuenSurface<T>::getStartPU() const {
    return -T(4);
  }


  template <typename T>
  T PKuenSurface<T>::getEndPU() const {
    return T(4);
  }


  template <typename T>
  T PKuenSurface<T>::getStartPV() const {
    return T(0.05);
  }


  template <typename T>
  T PKuenSurface<T>::getEndPV() const {
    return T(M_PI-0.05);
  }


  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PKuenSurface<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
