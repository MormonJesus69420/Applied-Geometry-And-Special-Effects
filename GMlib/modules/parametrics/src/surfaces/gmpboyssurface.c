
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
  PBoysSurface<T>::PBoysSurface() {
    init();
  }


  template <typename T>
  inline
  PBoysSurface<T>::PBoysSurface( const PBoysSurface<T>& copy ) : PSurf<T,3>( copy ) {
    init();
  }


  template <typename T>
  PBoysSurface<T>::~PBoysSurface() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************

  template <typename T>
  bool PBoysSurface<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PBoysSurface<T>::isClosedV() const {
    return false;
  }



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************

  template <typename T>
  void PBoysSurface<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

      T sq2 = T(M_SQRT2);
      T cu  = cos(u);
      T su  = sin(u);
      T cv  = cos(v);
      T sv  = sin(v);
      T c2u = sin(2*u);
      T s2v = sin(2*v);
      T c2v = cos(2*v);
      T s3v = sin(3*v);
      T c3v = cos(3*v);
      T sqv = sq2*su*cu*s3v;
      T suv = su*sv;
      T cuv = cu*cv;
      T cuu = cu*cu;
      T t23 = T(2)/T(3);
      T pc2 = pow(cu,T(2));
      T pc3 = pow(cu,T(3));
      T pc4 = pow(cu,T(4));
      T pc5 = pow(cu,T(5));
      T pc6 = pow(cu,T(6));
      T pc7 = pow(cu,T(7));
      T ps2 = pow(su,T(2));
      T pc20 = pow(cu,T(20));
      T sq3 = pow(sq2,T(3));
      T sq5 = pow(sq2,T(5));
      T pq2 = pow(T(1.0)-sqv,T(2));
      T pq3 = pow(T(1.0)-sqv,T(3));



    this->_p[0][0][0] =	cu*(sq2/T(3)*cu*c2v + t23*su*cv) / (T(1) - sqv);
    this->_p[0][0][1] =	cu*(sq2/T(3)*cu*s2v - t23*suv) / (T(1) - sqv);
    this->_p[0][0][2] =	cu/(T(1) - sqv)*cu - T(1);


    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(d1) {              //u
          T hq = -c2u*sq2*s3v;
        this->_p[1][0][0] =	-su*(T(1)/T(3)*sq2*cu*c2v + t23*su*cv) / (T(1)-sqv) + cu*(-T(1)/T(3)*sq2*su*c2v + t23*cuv) / (T(1)-sqv)
                            - cu*(T(1)/T(3)*sq2*cu*c2v + t23*su*cv)*(-hq) / pow((T(1)-sqv),T(2));
        this->_p[1][0][1] =	-su*(T(1)/T(3)*cu*s2v*sq2 - t23*suv) / (T(1)-sqv) + cu*(-T(1)/T(3)*su*s2v*sq2 + t23*cu*sv) / (T(1)-sqv)
                            - cu*(T(1)/T(3)*cu*s2v*sq2 - t23*suv)*(-hq) / pow((T(1)-sqv),T(2));
        this->_p[1][0][2] =	-T(2)*cu*su/(T(1)-sqv) - (cuu*(-hq)) / pow((T(1)-sqv),T(2));
      }

      if(d1>1)//uu
      {
        this->_p[2][0][0] =	-cu*(sq2*cu*c2v/T(3)+t23*su*cv)
                /(T(1.0)-sqv)-T(2)*su*(-sq2*su*c2v
                /T(3)+t23*cuv)/(T(1.0)-sqv)
                +T(2)*su*(sq2*cu*c2v/T(3)+t23*su*cv)
                /pq2*(-sq2*pc2
                *s3v+sq2*ps2*s3v)+cu*(-sq2*cu
                *c2v/T(3)-t23*su*cv)/(T(1.0)-sqv)
                -T(2)*cu*(-sq2*su*c2v/T(3)+t23*cuv)
                /pq2*(-sq2*pc2
                *s3v+sq2*ps2*s3v)+T(2)*cu*(sq2*cu
                *c2v/T(3)+t23*su*cv)/pow(T(1.0)-sqrt(T(2))*su*cu
                *s3v,T(3))*pow(-sq2*pc2*s3v+sqrt(T(2))
                *ps2*s3v,T(2))-T(4.0)*pc2*(sq2*cu
                *c2v/T(3)+t23*su*cv)
                /pow(T(1.0)-sqv,	T(2))*sq2*su*s3v;

        this->_p[2][0][1] =	-cu*(sq2*cu*s2v/T(3)-t23*suv)
                /(T(1.0)-sqv)-T(2)*su*(-sq2*su*s2v
                /T(3)-t23*cu*sv)/(T(1.0)-sqv)
                +T(2)*su*(sq2*cu*s2v/T(3)-t23*suv)
                /pq2*(-sq2*pc2
                *s3v+sq2*ps2*s3v)+cu*(-sq2*cu
                *s2v/T(3)+t23*suv)/(T(1.0)-sq2*su*cu
                *s3v)-T(2)*cu*(-sq2*su*s2v/T(3)-T(2)
                /T(3)*cu*sv)/pq2*(-sq2
                *pc2*s3v+sq2*ps2*s3v)+T(2)
                *cu*(sq2*cu*s2v/T(3)-t23*suv)/pow(T(1.0)
                -sqrt(T(2))*su*cu*s3v,T(3))*pow(-sq2*pc2
                *s3v+sqrt(T(2))*ps2*s3v,T(2))-T(4.0)*pc2
                *(sq2*cu*s2v/T(3)-t23*suv)
                /pq2*sq2*su*s3v;

        this->_p[2][0][2] =	T(2)*ps2/(T(1.0)-sqv)+T(4.0)*cu
                /pq2*su*(-sq2*pc2
                *s3v+sq2*ps2*s3v)-T(2)*pc2
                /(T(1.0)-sqv)+T(2)*pc2
                /pq3*pow(-sq2*pc2
                *s3v+sqrt(T(2))*ps2*s3v,T(2))-T(4.0)*pc3
                /pq2*sq2*su*s3v;
      }

      if(d2) //v
      {
        this->_p[0][1][0] =	cu*(-t23*cu*s2v*sq2 - t23*suv) / (T(1)-sqv) + T(3)*cuu*(T(1)/T(3)*sq2*cu*c2v + t23*su*cv)*sq2*su*c3v / pow((T(1)-sqv),T(2));

        this->_p[0][1][1] =	cu*(t23*sq2*cu*c2v - t23*su*cv) / (T(1)-sq2*su*cu*sin(3*v)) + T(3)*cuu*(T(1)/T(3)*cu*s2v*sq2 - t23*suv)*sq2*su*c3v / pow((T(1)-sqv),T(2));

        this->_p[0][1][2] =	T(3)*cuu*cu*sq2*su*c3v / pow((T(1)-sqv),T(2));
      }

      if(d2>1) //vv
      {
        this->_p[0][2][0] =	cu*(-T(4.0)/T(3)*sq2*cu*c2v-t23*su*cv)
                /(T(1.0)-sqv)+T(6.0)*pc2
                *(-t23*sqrt(T(2))*cu*s2v-t23*suv)
                /pq2*sq2*su*c3v
                +T(36.0)*pc3*(sq2*cu*c2v/T(3)+t23*su*cv)
                /pq3*ps2
                *pow(c3v,T(2))-T(9.0)*pc2*(sq2*cu*c2v/T(3)
                +t23*su*cv)/pq2
                *sq2*su*s3v;

        this->_p[0][2][1] =	cu*(-T(4.0)/T(3)*sq2*cu*s2v+t23*suv)
                /(T(1.0)-sqv)+T(6.0)*pc2
                *(t23*sq2*cu*c2v-t23*su*cv)
                /pq2*sq2*su*c3v
                +T(36.0)*pc3*(sq2*cu*s2v/T(3)-t23*suv)
                /pq3*ps2
                *pow(c3v,T(2))-T(9.0)*pc2*(sq2*cu*s2v
                /T(3)-t23*suv)/pq2
                *sq2*su*s3v;

        this->_p[0][2][2] =	T(36.0)*pow(cu,T(4.0))/pq3
                *ps2*pow(c3v,T(2))-T(9.0)*pc3
                /pq2*sq2*su*s3v;
      }
      if(d1 && d2) //uv
      {
        this->_p[1][1][0] =	-su*(-t23*sq2*cu*s2v-t23*suv)
                /(T(1.0)-sqv)-T(3)*ps2*(sq2*cu
                *c2v/T(3)+t23*su*cv)
                /pq2*sq2*cu*c3v
                +cu*(t23*sq2*su*s2v-t23*cu*sv)
                /(T(1.0)-sqv)+T(3)*pc2*(-sq2*su
                *c2v/T(3)+t23*cuv)/pq2
                *sq2*su*c3v-cu*(-t23*sq2*cu*s2v
                -t23*suv)/pq2
                *(-sq2*pc2*s3v+sq2*ps2*s3v)
                -T(6.0)*pc2*(sq2*cu*c2v/T(3)+t23*su*cv)
                /pq3*(-sq2*pc2
                *s3v+sq2*ps2*s3v)*sq2*su*c3v
                -cu*(sq2*cu*c2v/T(3)+t23*su*cv)
                /pq2*(-T(3)*sq2
                *pc2*c3v+T(3)*sq2*ps2*c3v);

        this->_p[1][1][1] =	-su*(t23*sq2*cu*c2v-t23*su*cv)
                /(T(1.0)-sqv)-T(3)*ps2*(sq2*cu
                *s2v/T(3)-t23*suv)
                /pq2*sq2*cu*c3v
                +cu*(-t23*sq2*su*c2v-t23*cuv)
                /(T(1.0)-sqv)+T(3)*pc2
                *(-sq2*su*s2v/T(3)-t23*cu*sv)
                /pq2*sq2*su*c3v
                -cu*(t23*sq2*cu*c2v-t23*su*cv)
                /pq2*(-sq2*pc2
                *s3v+sq2*ps2*s3v)-T(6.0)*pc2
                *(sq2*cu*s2v/T(3)-t23*suv)
                /pq3*(-sq2*pc2
                *s3v+sq2*ps2*s3v)*sq2*su*c3v
                -cu*(sq2*cu*s2v/T(3)-t23*suv)
                /pq2*(-T(3)*sq2
                *pc2*c3v+T(3)*sq2*ps2*c3v);

        this->_p[1][1][2] =	-T(6.0)*pc2/pq2
                *ps2*sq2*c3v-T(6.0)*pc3
                /pq3*(-sq2*pc2
                *s3v+sq2*ps2*s3v)*sq2*su*c3v
                -pc2/pq2
                *(-T(3)*sq2*pc2*c3v+T(3)*sq2
                *ps2*c3v);
      }
      if(d1>1 && d2)//uuv
      {
        T s3 =	16.0*pc4*sq2*c3v*T(6)*cv+8.0*pc5
            *T(2)*s3v*su*c3v*T(6)*cv+4.0*cuu
            *T(2)*sq2*s2v+4.0*cu*T(2)*suv
            -16.0*pc3*T(2)*su*c3v*T(3)*c2v
            +8.0*pc6*sq3*s3v*c3v*T(3)*c2v
            -T(4)*sq2*s2v;

        T s2 =	s3+2.0*sq2*c3v*T(6)*cv-16.0*cuu*sq2
            *c3v*T(6)*cv-4.0*pc4*sq3
            *c3v*T(3)*c2v*s3v-2.0*T(2)*sq2*s3v*sv
            -6.0*pc4*T(2)*sq3*s2v+6.0
            *pc4*T(2)*sq3*s2v*T(9)
            +4.0*pc6*T(2)*sq3*s2v;

          s3 =	s2 - T(36)*pc6*sq3*s2v + T(8)*pc5*s2v*su*s3v - T(72)*pc5*s2v*su*s3v
            + pc5*T(8)*T(2)*sv*su - T(36)*sv*su*pc20 + T(48)*su*s3v*cu*c3v*cv - T(96)*su*s3v*pc3*c3v*cv;

            T s1 =	s3+2.0*su*T(2)*T(2)*s2v*cu*s3v+2.0*pc5
            *T(4)*su*c3v*T(3)*c2v-2.0*pc5
            *T(4)*su*pow((T)c3v,T(3))*T(3)*c2v+6.0*su
            *T(2)*cu*c3v*T(3)*c2v+2.0*su*T(2)
            *T(2)*sv*cu-2.0*su*T(2)*T(2)*sv*cu
            *T(9)-4.0*su*T(2)*T(2)*sv*pc3
            +4.0*su*T(2)*T(2)*sv*pc3*T(9);

              s2 =	1/T(3)/pow((T)-T(1.0)+sqv,T(4));

        this->_p[2][1][0] =	s1*s2;

          s3 =	2.0*T(2)*sq2*c2v+4.0*cu*T(2)*su*cv
            -16.0*pc4*sq2*c3v*T(6)*sv
            +8.0*pc6*sq3*s3v*c3v*T(3)*s2v
            -8.0*pc5*T(2)*s3v*su*c3v*T(6)
            *sv-16.0*pc3*T(2)*su*c3v*T(3)*s2v
            +4.0*su*T(2)*T(2)*cv*pc3
            *T(9);

          s2 =	s3+2.0*su*T(2)*T(2)*cv*cu-2.0*su*T(2)*T(2)*c2v
            *cu*s3v-4.0*su*T(2)*T(2)*cv*pc3
            +6.0*su*T(2)*cu*c3v*T(3)*s2v+4.0*pc5
            *T(2)*T(2)*cv*su-4.0*pc5*T(2)*T(2)
            *cv*su*T(9)+8.0*su*T(2)*s3v
            *pc3*c3v*T(6)*sv;

          s3 =	s2-4.0*su*T(2)*s3v*cu*c3v*T(6)*sv
            -4.0*pc6*T(2)*sq3*c2v+4.0*pc6*T(2)
            *sq3*c2v*T(9)-4.0*cuu*T(2)*sq2
            *c2v+16.0*cuu*sq2*c3v*T(6)*sv
            +6.0*pc4*T(2)*sq3*c2v
            -6.0*pc4*T(2)*sq3*c2v*T(9);

          s1 =	s3-2.0*pc20*su*pow((T)c3v,T(3))*T(3)*s2v
            +2.0*pc20*su*c3v*T(3)*s2v
            -2.0*pc5*T(2)*T(4)*c2v*su*s3v
            +2.0*pc5*T(2)*T(4)*c2v*su*s3v
            *T(9)-2.0*su*T(2)*T(2)*cv*cu
            *T(9)-2.0*T(2)*sq2*s3v*cv-2.0*sq2
            *c3v*T(6)*sv-4.0*pc4*sq3
            *c3v*T(3)*s2v*s3v;

          s2 =	1/T(3)/pow((T)-T(1.0)+sqv,T(4));

        this->_p[2][1][1] =	s1*s2;
        this->_p[2][1][2] =	-2.0*sq2*cu*c3v*T(3)*(-4.0*pc5*sq2*s3v
                +8.0*su*cuu-pc4*su*T(2)+pc4
                *su*T(2)*T(9)+2.0*pc3*sq2*s3v
                -3.0*su)/pow((T)-T(1.0)+sqv,T(4));
      }
      if(d1 && d2>1) //uvv
      {
        T s4 =	T(2)*cv*T(2)*cuu-T(2)*cv*T(2)
            *cuu*T(9)-3.0*T(2)*cv*T(2)*pc4
            +3.0*T(2)*cv*T(2)*pc4*T(9)+2.0*T(2)*cv
            *T(2)*pc6-2.0*T(2)*cv*T(2)
            *pc6*T(9);
        T s3 =	s4+T(4)*pc4*s3v*T(9)*c2v+T(4)
            *pc4*s3v*T(9)*c2v*T(9)-T(4)
            *pc6*s3v*T(9)*c2v-T(4)*pc6
            *s3v*T(9)*c2v*T(9)+pc6*T(4)
            *T(4)*s3v*c2v-pc6*T(4)*T(4)
            *s3v*c2v*T(9)+2.0*T(2)*T(4)*pc4
            *c3v*T(3)*s2v;

          s4 =	s3-2.0*T(2)*T(4)*pc4*pow((T)c3v,T(3))*T(3)*s2v
            -2.0*T(2)*T(4)*pc6*c3v*T(3)*s2v+2.0*T(2)
            *T(4)*pc6*pow((T)c3v,T(3))*T(3)*s2v+4.0*su
            *T(2)*sq2*cu*c3v*T(3)*sv-4.0*su*sq3
            *pc3*T(9)*c2v+4.0*su*sq3*pc5
            *T(9)*c2v;
        T s2 =	s4-4.0*su*sq3*pc3*T(9)*c2v*T(9)
            -2.0*su*T(2)*cv*sq2*cu*s3v-5.0*T(4)
            *T(2)*cuu*c2v*s3v+2.0*su*sq2
            *cu*s3v*T(9)*T(2)*cv-2.0*T(2)
            *cuu*T(9)*T(2)*cv-4.0*T(2)*cuu
            *T(9)*T(2)*cv*T(9)+6.0*T(2)*pc4
            *T(9)*T(2)*cv;

          s4 =	12.0*T(2)*pc4*T(9)*T(2)*cv*T(9)
            -4.0*T(2)*pc6*T(9)*T(2)*cv-8.0*T(2)
            *pc6*T(9)*T(2)*cv*T(9)+6.0*T(2)
            *T(2)*cuu*c3v*T(3)*s2v-4.0*T(2)
            *T(2)*cuu*c3v*T(3)*sv*s3v+T(24)
            *T(2)*pc4*c3v*T(3)*sv*s3v;

          s3 =	s4-8.0*T(2)*T(2)*pc6*c3v*T(3)*sv*s3v
            +3.0*T(2)*cuu*s3v*T(9)*c2v
            -8.0*su*T(2)*sq3*pc3*c3v*T(3)*s2v
            *s3v+4.0*su*T(4)*sq3*pc3*c2v
            -2.0*su*T(4)*sq3*pc5*c2v
            -4.0*su*T(4)*sq3*pc3*c2v
            *T(9)+2.0*su*T(4)*sq3*pc5
            *c2v*T(9);

          s4 =	s3-T(4)*T(4)*pc4*c2v*s3v+T(4)
            *T(4)*pc4*c2v*s3v*T(9)+8.0
            *pc5*T(2)*sq3*s3v*su*c3v*T(3)*s2v
            +T(2)*cv+s2+2.0*su*T(4)*sq2*cu*c2v
            ;
        T s1 =	s4-2.0*cuu*T(2)*cv-8.0*pc4*T(2)*T(2)
            *c3v*T(3)*s2v+8.0*pc5*sq3
            *T(9)*T(9)*su*c2v-4.0*pc4
            *T(2)*s3v*T(9)*c2v+4.0*pc4
            *T(4)*T(2)*s3v*c2v-8.0*pc3
            *T(2)*sq2*su*c3v*T(3)*sv+4.0*pc3*T(2)
            *cv*sq2*su*s3v-4.0*pc3*sq2*su
            *s3v*T(9)*T(2)*cv;

        s2 =	1/T(3)/pow((T)T(1.0)-sqv,T(4));

        this->_p[1][2][0] =	s1*s2;

          s4 =	-T(2)*sv-2.0*su*sq2*cu*s3v*T(9)*T(2)
            *sv-4.0*T(2)*T(2)*cuu*c3v*T(3)*cv
            *s3v-5.0*T(4)*T(2)*cuu*s2v
            *s3v+4.0*pc5*sq3*T(9)*su*s2v
            -8.0*pc5*T(2)*sq3*s3v*su*c3v*T(3)*c2v
            ;
          s3 =	s4+4.0*su*T(4)*sq3*pc3*s2v-4.0*su
            *T(4)*sq3*pc3*s2v*T(9)
            -4.0*su*sq3*pc3*T(9)*s2v-pc6
            *T(4)*s3v*T(9)*s2v+T(4)*pc4
            *s3v*T(9)*s2v+8.0*su*T(2)*sq3*pc3
            *c3v*T(3)*c2v*s3v-6.0*T(2)*T(2)*cuu
            *c3v*T(3)*c2v;

          s4 =	s3+4.0*su*T(2)*sq2*cu*c3v*T(3)*cv-2.0*T(2)
            *T(4)*pc4*c3v*T(3)*c2v+2.0*T(2)*T(4)
            *pc4*pow((T)c3v,T(3))*T(3)*c2v+2.0*T(2)*T(4)
            *pc6*c3v*T(3)*c2v-2.0*T(2)*T(4)*pc6
            *pow((T)c3v,T(3))*T(3)*c2v-T(4)*pc6*T(9)
            *T(9)*s2v*s3v;

          s2 =	s4+2.0*T(2)*cuu*T(9)*T(2)*sv-6.0*T(2)
            *pc4*T(9)*T(2)*sv+4.0*T(2)*pc6
            *T(9)*T(2)*sv+3.0*T(2)*cuu*s3v*T(9)
            *s2v+12.0*pc4*T(2)*T(2)*c3v*T(3)*cv
            *s3v-8.0*pc6*T(2)*T(2)*c3v*T(3)*cv
            *s3v-2.0*pc20*sq3*s2v*su;

          s4 =	2.0*pc20*sq3*s2v*su*T(9)
            -T(4)*T(4)*pc4*s2v*s3v+T(4)
            *T(4)*pc4*s2v*s3v*T(9)+T(4)
            *T(4)*pc6*s2v*s3v-T(4)*T(4)
            *pc6*s2v*s3v*T(9)+4.0*T(2)*cuu
            *T(9)*T(9)*T(2)*sv;

          s3 =	s4-12.0*T(2)*pc4*T(9)*T(9)*T(2)*sv+8.0
            *T(2)*pc6*T(9)*T(9)*T(2)*sv-4.0*su
            *sq3*pc3*T(9)*T(9)*s2v-3.0*T(2)
            *sv*T(2)*pc4*T(9)-2.0*T(2)*sv*T(2)
            *pc6+2.0*T(2)*sv*T(2)*pc6*T(9)+2.0*su
            *T(2)*sv*sq2*cu*s3v;

          s4 =	s3-T(2)*sv*T(2)*cuu+T(2)*sv*T(2)
            *cuu*T(9)+3.0*T(2)*sv*T(2)*pc4
            +T(4)*pc4*T(9)*T(9)*s2v*s3v+4.0
            *pc4*T(4)*T(2)*s3v*s2v+8.0
            *pc5*sq3*T(9)*T(9)*su*s2v;

          s1 =	s4+8.0*pc4*T(2)*T(2)*c3v*T(3)*c2v-4.0*pc4
            *T(2)*s3v*T(9)*s2v-8.0*pc3*T(2)
            *sq2*su*c3v*T(3)*cv-4.0*pc3*T(2)*sv*sq2
            *su*s3v+4.0*pc3*sq2*su*s3v*T(9)*T(2)*sv
            +2.0*su*T(4)*sq2*cu*s2v+2.0*cuu*T(2)*sv+s2;

          s2 =	1/T(3)/pow((T)-T(1.0)+sqv,T(4));

        this->_p[1][2][1] =	s1*s2;
        this->_p[1][2][2] =	-cuu*sq2*T(9)*(4.0*s3v*cuu
                +pc4*T(2)*T(9)*s3v-3.0*s3v
                -s3v*T(2)*cuu+s3v*T(2)
                *pc4-4.0*su*sq2*pc3+4.0*su*sq2
                *cu-cuu*T(2)*T(9)*s3v+4.0*cu
                *su*sq2*T(9)-8.0*pc3*sq2
                *T(9)*su)/pow((T)T(1.0)-sqv,T(4));

      }
      if(d1>1 && d2>1) //uuvv
      {
        T c8 = pow((T)cu,T(8));

        T s5 =	-32.0*su*T(2)*T(2)*c3v*T(3)*pc3*s2v
            -16.0*pc5*T(2)*T(2)*s3v*c3v*T(3)*suv
            -4.0*T(4)*cuu*sq2*c2v-4.0*T(2)*su*cuv
            -16.0*pc3*T(2)*su*s3v*T(9)*c2v
            +32.0*pc4*T(2)*sq2*c3v*T(3)*sv;

        T s4 =	s5+16.0*pc4*sq2*s3v*T(9)*T(2)*cv-4.0
            *pc4*T(2)*sq2*s3v*cv-64.0*pc6*sq3
            *T(9)*T(9)*c2v+2.0*T(4)*sq2*c2v
            +2.0*c8*sq5*T(9)*c2v-2.0*c8*T(4)
            *sq5*c2v;

        s5 =	s4-2.0*pc6*sq5*T(9)*c2v+2.0*pc6*T(4)
            *sq5*c2v+4.0*pc5*T(2)*T(9)*su*T(2)
            *cv+4.0*pc3*T(4)*T(2)*su*s3v*c2v
            +8.0*pc20*T(4)*su*s3v*c2v
            *T(9);

        T  s3 =	s5-4.0*pc7*T(4)*T(4)*su*s3v*c2v*T(9)
            -6.0*cuu*sq3*T(9)*c2v-18.0*cuu
            *sq3*T(9)*c2v*T(9)+18.0*pc4
            *sq3*T(9)*c2v+74.0*pc4*sq3*T(9)
            *c2v*T(9)-8.0*pc6*sq3*T(9)
            *c2v-2.0*T(4)*sq3*c2v*cuu
            *T(9);

          s5 =	4.0*T(4)*sq3*c2v*pc4-4.0*T(4)*sq3*c2v
            *pc4*T(9)-4.0*T(4)*sq3*c2v
            *pc6+4.0*T(4)*sq3*c2v*pc6
            *T(9)-4.0*T(4)*su*T(2)*c2v*cu*s3v
            +T(8)*cuv*sq2*s3v;

          s4 =	s5-8.0*pc20*T(4)*su*s3v*c2v-12.0
            *pc5*T(2)*T(4)*su*pow((T)c3v,T(3))*T(3)*s2v-8.0*cuu
            *T(2)*sq3*c3v*T(3)*sv+24.0*pc4*T(2)*sq3
            *c3v*T(3)*sv+8.0*cuu*T(2)*sq3*pow((T)c3v,T(3))
            *T(3)*sv-24.0*pc4*T(2)*sq3*pow((T)c3v,T(3))*T(3)*sv
            +36.0*pc4*T(2)*sq3*s3v*c3v*T(3)*s2v;

          s5 =	s4+4.0*T(2)*su*pc3*cv*T(2)-4.0*T(2)*su*pc5*cv
            *T(2)-4.0*T(2)*su*pc3*cv*T(2)
            *T(9)-12.0*cuu*T(2)*sq3*s3v*c3v*T(3)
            *s2v+4.0*cu*T(2)*T(2)*s3v*c3v*T(3)*su
            *sv+16.0*pc3*T(2)*T(2)*s3v*c3v*T(3)
            *suv;

        T s2 =	s5-8.0*cuu*sq3*T(9)*T(9)*T(2)*cv
            *s3v+24.0*pc4*sq3*T(9)*T(9)*T(2)*cv
            *s3v-32.0*pc6*sq3*T(9)*T(9)*T(2)*cv
            *s3v+16.0*c8*sq3*T(9)*T(9)*T(2)
            *cv*s3v+64.0*pc3*T(2)*su*T(9)*T(9)
            *T(2)*cv+2.0*pc20*su*T(9)*T(9)
            *c2v*s3v+s3;

          s5 =	-16.0*pc7*T(2)*T(4)*su*c3v*T(3)*s2v+16.0
            *pc7*T(2)*T(4)*su*pow((T)c3v,T(3))*T(3)*s2v-32.0
            *pc6*T(2)*sq3*c3v*T(3)*sv+16.0*c8*T(2)
            *sq3*c3v*T(3)*sv+32.0*pc6*T(2)*sq3
            *pow((T)c3v,T(3))*T(3)*sv-16.0*c8*T(2)*sq3*pow((T)c3v,T(3))
            *T(3)*sv;

          s4 =	s5+12.0*pc5*T(2)*T(4)*su*c3v*T(3)*s2v+2.0*
            T(4)*sq3*c2v*cuu+4.0*pc7
            *T(4)*T(4)*su*s3v*c2v-2.0*c8
            *sq5*pow((T)c3v,T(4))*T(9)*c2v+2.0*T(2)*sq2*s3v
            *cv-32.0*T(2)*sq2*cuu*c3v*T(3)*sv;

          s5 =	s4+12.0*su*T(2)*T(2)*cu*c3v*T(3)*s2v-14.0*cu
            *T(2)*T(9)*T(9)*T(2)*su*cv-16.0
            *cuu*sq2*s3v*T(9)*T(2)*cv+2.0*cu
            *T(2)*T(9)*T(2)*su*cv+8.0*pc3*T(2)
            *T(9)*T(2)*su*cv-8.0*pc5*T(2)*T(9)
            *T(2)*su*cv;

          s3 =	s5-16.0*pc6*sq3*s3v*T(9)*T(2)*cv+8.0
            *c8*sq3*s3v*T(9)*T(2)*cv-4.0*pc6
            *T(2)*sq5*s3v*c3v*T(3)*s2v+4.0*pc6*T(2)
            *sq5*s3v*pow((T)c3v,T(3))*T(3)*s2v+4.0*c8*T(2)
            *sq5*s3v*c3v*T(3)*s2v-4.0*c8*T(2)
            *sq5*s3v*pow((T)c3v,T(3))*T(3)*s2v-4.0*cuu
            *sq3*s3v*T(9)*T(2)*cv;

          s5 =	12.0*pc4*sq3*s3v*T(9)*T(2)*cv+2.0*pc6
            *sq5*T(9)*c2v*pow((T)c3v,T(4))-4.0*pc6*T(4)
            *sq5*c2v*T(9)+4.0*c8*T(4)*sq5
            *c2v*T(9)+2.0*pc6*T(4)*sq5*c2v
            *pow((T)c3v,T(4))-2.0*c8*T(4)*sq5*c2v*pow((T)c3v,T(4));

          s4 =	s5-8.0*pc7*T(4)*s3v*su*T(9)*c2v+6.0
            *pc20*s3v*su*T(9)*c2v+4.0*T(2)
            *sq2*c3v*T(3)*sv+2.0*sq2*s3v*T(9)*T(2)
            *cv+6.0*su*T(2)*cu*s3v*T(9)*c2v
            +8.0*pc6*T(2)*sq3*s3v*cv-4.0*c8
            *T(2)*sq3*s3v*cv;

          s5 =	s4-8.0*pc6*T(2)*sq3*s3v*cv*T(9)+4.0
            *c8*T(2)*sq3*s3v*cv*T(9)+2.0*cuu
            *T(2)*sq3*s3v*cv-6.0*pc4*T(2)*sq3
            *s3v*cv-2.0*cuu*T(2)*sq3*s3v*cv*T(9)
            +6.0*pc4*T(2)*sq3*s3v*cv*T(9);

          T s1 = s5-4.0*cu*T(2)*T(2)*su*cv+4.0*cu*T(2)*T(2)
            *su*cv*T(9)+s2-16.0*pc7*T(4)
            *T(9)*T(9)*c2v*su*s3v+s3-16.0*pc6
            *T(2)*sq3*s3v*c3v*T(3)*s2v-64.0
            *pc5*T(2)*T(9)*T(9)*su*T(2)*cv;

          s2 =	1/T(3)/pow((T)-T(1.0)+sqv,T(5));

        this->_p[2][2][0] =	s1*s2;

          s5 =	16.0*pc6*T(2)*sq3*s3v*c3v*T(3)*c2v
            +32.0*pc3*T(2)*T(2)*c3v*T(3)*su*c2v
            +4.0*pc4*T(2)*sq2*s3v*sv+4.0*cu*T(2)*suv
            -64.0*pc6*sq3*T(9)*T(9)*s2v
            -4.0*cuu*T(4)*sq2*s2v;

          s4 =	s5-16.0*pc5*T(2)*T(2)*s3v*su*c3v*T(3)*cv
            -16.0*pc7*T(4)*s3v*su*T(9)*T(9)*s2v
            +64.0*pc5*T(2)*T(9)*T(9)*su*T(2)*sv+32.0
            *pc4*T(2)*sq2*c3v*T(3)*cv-16.0*pc4*sq2
            *s3v*T(9)*T(2)*sv-16.0*su*T(2)*s3v
            *T(9)*pc3*s2v;

          s5 =	s4+6.0*su*T(2)*s3v*T(9)*cu*s2v-6.0*cuu
            *sq3*T(9)*s2v+18.0*pc4*sq3*T(9)
            *s2v+4.0*cu*T(2)*T(2)*s3v*c3v*T(3)*su*cv+16.0
            *pc6*sq3*s3v*T(9)*T(2)*sv;

          s3 =	s5+8.0*pc5*T(2)*su*T(9)*T(2)*sv-8.0*c8
            *sq3*s3v*T(9)*T(2)*sv+4.0*T(2)*sq2*c3v*T(3)*cv
            +16.0*pc3*T(2)*T(2)*s3v*c3v*T(3)*su*cv+8.0*cuu
            *sq3*s3v*T(9)*T(9)*T(2)*sv+32.0*pc6
            *sq3*T(9)*T(9)*T(2)*sv*s3v-36.0*pc4*T(2)
            *sq3*s3v*c3v*T(3)*c2v;

          s5 =	s3+12.0*cuu*T(2)*sq3*s3v*c3v*T(3)*c2v-32.0*cuu
            *T(2)*sq2*c3v*T(3)*cv-4.0*pc5*T(2)*T(9)
            *su*T(2)*sv-2.0*T(2)*sq2*s3v*sv+16.0*sq2*s3v
            *T(9)*T(2)*sv*cuu;

          s4 =	s5-8.0*cuu*T(2)*sq3*c3v*T(3)*cv-4.0*cuu*T(2)*sq2
            *s3v*sv-2.0*sq2*s3v*T(9)*T(2)*sv+4.0*pc3
            *T(4)*T(2)*s3v*su*s2v+2.0*T(4)*sq2*s2v
            -2.0*c8*T(4)*sq5*s2v+2.0*c8*sq5
            *T(9)*s2v;

          s5 =	s4+2.0*pc6*T(4)*sq5*s2v-2.0*pc6*sq5*T(9)
            *s2v+74.0*pc4*sq3*T(9)*T(9)*s2v-18.0*cuu
            *sq3*T(9)*T(9)*s2v-2.0*c8*sq5
            *pow((T)c3v,T(4))*T(9)*s2v-8.0*pc20*T(4)*s3v*su
            *s2v;

          s2 =	s5+4.0*pc7*T(4)*T(4)*s3v*su*s2v+8.0*pc5
            *T(4)*T(4)*s3v*su*s2v*T(9)-4.0*pc7
            *T(4)*T(4)*s3v*su*s2v*T(9)+6.0*pc4
            *T(2)*sq3*s3v*sv-8.0*pc6*T(2)*sq3*s3v*sv
            +4.0*c8*T(2)*sq3*s3v*sv+2.0*cuu*T(2)*sq3
            *s3v*sv*T(9);

          s5 =	s2-6.0*pc4*T(2)*sq3*s3v*sv*T(9)+8.0*pc6*T(2)
            *sq3*s3v*sv*T(9)-4.0*c8*T(2)*sq3*s3v
            *sv*T(9)-4.0*pc3*T(2)*T(2)*suv+4.0
            *pc5*T(2)*T(2)*suv;

          s4 =	s5-4.0*cu*T(2)*T(2)*suv*T(9)+4.0*pc3*T(4)*su
            *sv*T(9)-2.0*cuu*T(2)*sq3*s3v*sv+4.0*cu*T(2)
            *T(2)*suv-64.0*pc3*T(2)*T(9)*T(9)*su
            *T(2)*sv-24.0*pc4*sq3*s3v*T(9)*T(9)
            *T(2)*sv;

          s5 =	s4-16.0*c8*sq3*s3v*T(9)*T(9)*T(2)*sv+4.0*pc6
            *T(2)*sq5*s3v*c3v*T(3)*c2v-4.0*c8*T(2)*sq5
            *s3v*c3v*T(3)*c2v-4.0*pc6*T(2)*sq5*s3v*pow((T)c3v,T(3))
            *T(3)*c2v+4.0*c8*T(2)*sq5*s3v*pow((T)c3v,T(3))*T(3)*c2v
            -4.0*pc6*T(4)*sq5*s2v*T(9);

          s3 =	s5+4.0*c8*T(4)*sq5*s2v*T(9)+2.0*pc6*T(4)
            *sq5*s2v*pow((T)c3v,T(4))-2.0*c8*T(4)*sq5*s2v
            *pow((T)c3v,T(4))+14.0*su*T(2)*T(9)*T(9)*cu*T(2)*sv
            +2.0*cuu*T(4)*sq3*s2v+4.0*pc4*T(4)
            *sq3*s2v-4.0*pc6*T(4)*sq3*s2v;

          s5 =	s3-2.0*cuu*T(4)*sq3*s2v*T(9)
            -4.0*pc4*T(4)*sq3*s2v*T(9)
            +4.0*pc6*T(4)*sq3*s2v*T(9)
            +6.0*pc20*s3v*su*T(9)*s2v-8.0*pc7
            *T(4)*s3v*su*T(9)*s2v;

          s4 =	s5+2.0*pc20*s3v*su*T(9)*s2v*T(9)
            -4.0*cu*T(4)*T(2)*s3v*su*s2v-32.0*pc6
            *T(2)*sq3*c3v*T(3)*cv+16.0*c8*T(2)*sq3
            *c3v*T(3)*cv+8.0*cuu*T(2)*sq3*pow((T)c3v,T(3))*T(3)
            *cv-24.0*pc4*T(2)*sq3*pow((T)c3v,T(3))*T(3)*cv+32.0
            *pc6*T(2)*sq3*pow((T)c3v,T(3))*T(3)*cv;

          s5 =	s4-16.0*c8*T(2)*sq3*pow((T)c3v,T(3))*T(3)*cv+24.0*pc4
            *T(2)*sq3*c3v*T(3)*cv+2.0*pc6*sq5*T(9)
            *s2v*pow((T)c3v,T(4))-2.0*cu*T(2)*T(9)*T(2)*suv
            -12.0*cu*T(2)*T(2)*c3v*T(3)*su*c2v-12.0*pc5
            *T(2)*T(4)*su*c3v*T(3)*c2v;

          s1 =	s5+16.0*pc7*T(2)*T(4)*su*c3v*T(3)*c2v+12.0*pc5
            *T(2)*T(4)*su*pow((T)c3v,T(3))*T(3)*c2v-16.0*pc7*T(2)
            *T(4)*su*pow((T)c3v,T(3))*T(3)*c2v-12.0*pc4*sq3
            *s3v*T(9)*T(2)*sv-8.0*pc3*T(2)*su*T(9)
            *T(2)*sv+4.0*cuu*sq3*s3v*T(9)*T(2)*sv
            -8.0*pc6*sq3*T(9)*s2v;

          s2 = 1/T(3)/pow((T)-T(1.0)+sqv,T(5));

        this->_p[2][2][1] =	s1*s2;

        T s6 =	8.0*su*s3v*cuu+32.0*pc5*sq2*T(9)
            +8.0*pc6*T(2)*s3v*su*T(9)
            -pc7*sq3-3.0*su*s3v+3.0*sq2
            *cu+4.0*pc6*su*T(2)*s3v-3.0*pc4*su
            *T(2)*s3v;

          s5 =	s6-pc4*su*T(2)*s3v*T(9)+pc5
            *sq3+9.0*sq2*cu*T(9)-9.0*pc3*sq2
            -37.0*pc3*sq2*T(9)+4.0*pc5
            *sq2+pc7*sq3*pow((T)c3v,T(4))-pc5
            *sq3*pow((T)c3v,T(4));

          s6 = 1/(pow((T)-T(1.0)+sqv,T(5)));

          s4 = T(9)*s5*s6;

        this->_p[2][2][2] =	-2.0*sq2*cu*s4;
      }
    }
  }


  template <typename T>
  T PBoysSurface<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PBoysSurface<T>::getEndPU() const {
    return T(M_PI);
  }


  template <typename T>
  T PBoysSurface<T>::getStartPV() const {
    return T(0);
  }


  template <typename T>
  T PBoysSurface<T>::getEndPV() const {
    return T(M_PI);
  }



  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PBoysSurface<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


} // END namespace GMlib
