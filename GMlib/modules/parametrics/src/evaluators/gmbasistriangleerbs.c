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




#include "gmerbsevaluator.h"
#include "gmbfbsevaluator.h"


namespace GMlib {

template <typename T>
BasisTriangleERBS<T>::BasisTriangleERBS( BasisTriangleType t)
{
    _type = t;
    if(_type == ERBS_RATIONAL)        _b = new ERBSEvaluator<long double>();
    else if(_type == BFBS_RATIONAL)   _b = new BFBSEvaluator<long double>();
    else                              _b = NULL;
}


template <typename T>
  BasisTriangleERBS<T>::~BasisTriangleERBS()
  {
      if(_b)    delete _b;
  }


  template <typename T>
  void BasisTriangleERBS<T>::reset( BasisTriangleType t)
  {
      if(_b)                     delete _b;

      _type = t;
      if(_type == ERBS_RATIONAL)        _b = new ERBSEvaluator<long double>();
      else if(_type == BFBS_RATIONAL)   _b = new BFBSEvaluator<long double>();
      else                              _b = NULL;
  }



  template <typename T>
  inline
  void BasisTriangleERBS<T>::getB( DVector<T>& B, T t, int d )
  {
    B.setDim(d+1);
    B[0] = (*_b)(t);
    if(d>0) B[1] = _b->getDer1();
    if(d>1) B[2] = _b->getDer2();
  }



  // Klassisk ERBS og BFBS, basert p� rasjonal versjon av 1-dim basis
  //******************************************************************

  template <typename T>
  inline
  void BasisTriangleERBS<T>::computeB1( T u, T v, T w )
  {
      DVector<T> Bu;  getB( Bu, u, 2 );
      DVector<T> Bv;  getB( Bv, v, 2 );
      DVector<T> Bw;  getB( Bw, w, 2 );

      T sum = Bu[0] + Bv[0] + Bw[0];
      T sum2 = sum*sum;
      T sum3 = sum2*sum;

      _A[0][0] = Bu[0]/sum;                     // B1
      _A[0][1] = Bv[0]/sum;                     // B2
      _A[0][2] = Bw[0]/sum;                     // B3

      T aa = Bu[1]/sum2;
      _A[1][0] =  aa * ( sum - Bu[0] );         // du  B1
      _A[1][1] = -aa * Bv[0];                   //     B2
      _A[1][2] = -aa * Bw[0];                   //     B3
      aa = Bv[1]/sum2;
      _A[2][0] = -aa * Bu[0];                   // dv  B1
      _A[2][1] =  aa * ( sum - Bv[0] );         //     B2
      _A[2][2] = -aa * Bw[0];                   //     B3
      aa = Bw[1]/sum2;
      _A[3][0] = -aa * Bu[0];                   // dw  B1
      _A[3][1] = -aa * Bv[0];                   //     B2
      _A[3][2] =  aa * ( sum - Bw[0] );         //     B3

      aa = (2*Bu[1]*Bu[1]/sum - Bu[2])/sum2;
      _A[4][0] = aa * (Bu[0]-sum);              // duu B1
      _A[4][1] = aa * (Bv[0]);                  //     B2
      _A[4][2] = aa * (Bw[0]);                  //     B3
      aa = Bu[1]*Bv[1]/sum3;
      _A[5][0] = aa*(2*Bu[0]-sum);              // duv B1
      _A[5][1] = aa*(2*Bv[0]-sum);              //     B2
      _A[5][2] = aa*(2*Bw[0]);                  //     B3
      aa = Bu[1]*Bw[1]/sum3;
      _A[6][0] = aa*(2*Bu[0]-sum);              // duw B1
      _A[6][1] = aa*(2*Bv[0]);                  //     B2
      _A[6][2] = aa*(2*Bw[0]-sum);              //     B3
      aa = (2*Bv[1]*Bv[1]/sum - Bv[2])/sum2;
      _A[7][0] = aa * (Bu[0]);                  // dvv B1
      _A[7][1] = aa * (Bv[0]-sum);              //     B2
      _A[7][2] = aa * (Bw[0]);                  //     B3
      aa = Bv[1]*Bw[1]/sum3;
      _A[8][0] = aa*(2*Bu[0]);                  // dvw B1
      _A[8][1] = aa*(2*Bv[0]-sum);              //     B2
      _A[8][2] = aa*(2*Bw[0]-sum);              //     B3
      aa = (2*Bw[1]*Bw[1]/sum - Bw[2])/sum2;
      _A[9][0] = aa * (Bu[0]);                  // dww B1
      _A[9][1] = aa * (Bv[0]);                  //     B2
      _A[9][2] = aa * (Bw[0]-sum);              //     B3
  }


  // Rasjonal barycentrisk basert basis
  //************************************

  template <typename T>
  inline
  void BasisTriangleERBS<T>::computeB2( T u, T v, T w )
  {
      int k=2;

      T uu = pow(u,k);
      T vv = pow(v,k);
      T ww = pow(w,k);
      T sum = uu + vv + ww;
      T sum2 = pow(sum,k);

      _A[0][0] = uu/sum;
      _A[0][1] = vv/sum;
      _A[0][2] = ww/sum;

      _A[1][0] = k*pow(u,k-1)*(sum - uu)/sum2;
      _A[1][1] = k*pow(v,k-1)*(-uu)/sum2;
      _A[1][2] = k*pow(w,k-1)*(-uu)/sum2;

      _A[2][0] = k*pow(u,k-1)*(-vv)/sum2;
      _A[2][1] = k*pow(v,k-1)*(sum - vv)/sum2;
      _A[2][2] = k*pow(w,k-1)*(-vv)/sum2;

      _A[3][0] = k*pow(u,k-1)*(-ww)/sum2;
      _A[3][1] = k*pow(v,k-1)*(-ww)/sum2;
      _A[3][2] = k*pow(w,k-1)*(sum - ww)/sum2;

      _A[4][0] = k*(k-1)*pow(u,k-2)*(sum - uu)/sum2 ;
      _A[4][1] = k*pow(v,k-1)*(-uu)/sum2;
      _A[4][2] = k*pow(w,k-1)*(-uu)/sum2;
  }


  // Polynomial barycentrisk basert basis, orden 1
  //***********************************************

  template <typename T>
  inline
  void BasisTriangleERBS<T>::computeB3( T u, T v, T w )
  {
      T uu = u*u;
      T vv = v*v;
      T ww = w*w;
      T uv = u*v;
      T uw = u*w;
      T vw = v*w;

      _A[0][0] = uu*(6*vw-2*u+3);    // B1 u�(6vw-2u+3)
      _A[0][1] = vv*(6*uw-2*v+3);    // B2 v�(6uw-2v+3)
      _A[0][2] = ww*(6*uv-2*w+3);    // B3 w�(6uv-2w+3)

      _A[1][0] = 6*u*(2*vw-u+1); // du  B1 6u(2vw-u+1)  (12vw -6u)u + 6u
      _A[1][1] = 6*vv*w;         //     B2 6v�w
      _A[1][2] = 6*ww*v;         //     B3 6w�v

      _A[2][0] = 6*uu*w;         // dv  B1 6u�w
      _A[2][1] = 6*v*(2*uw-v+1); //     B2 6v(2uw-v+1)
      _A[2][2] = 6*ww*u;         //     B3 6w�u

      _A[3][0] = 6*uu*v;         // dw  B1 6u�v
      _A[3][1] = 6*vv*u;         //     B2 6v�u
      _A[3][2] = 6*w*(2*uv-w+1); //     B3 6w(2uv-w+1)

      _A[4][0] = 6*(1-2*(u-vw)); // duu B1 6(1-2(u-vw))  6-12u+12vw
      _A[4][1] = 0;              //     B2 0
      _A[4][2] = 0;              //     B3 0

      _A[5][0] = 12*uw;          // duv B1 12uw
      _A[5][1] = 12*vw;          //     B2 12vw
      _A[5][2] = 6*ww;           //     B3 6w�

      _A[6][0] = 12*uv;          // duw B1 12uv
      _A[6][1] = 6*vv;           //     B2 6v�
      _A[6][2] = 12*vw;          //     B3 12vw

      _A[7][0] = 0;              // dvv B1 0
      _A[7][1] = 6*(1-2*(v-uw)); //     B2 6(1-2(v-uw))
      _A[7][2] = 0;              //     B3 0

      _A[8][0] = 6*uu;           // dvw B1 6u�
      _A[8][1] = 12*uv;          //     B2 12uv
      _A[8][2] = 12*uw;          //     B3 12uw

      _A[9][0] = 0;              // dww B1 0
      _A[9][1] = 0;              //     B2 0
      _A[9][2] = 6*(1-2*(w-uv)); //     B3 6(1-2(w-uv))

      _A[10][0] = -12;           // duuu B1 -12
      _A[10][1] = 0;             //      B2 0
      _A[10][2] = 0;             //      B3 0

      _A[11][0] = 12*w;          // duuv B1 12w
      _A[11][1] = 0;             //      B2 0
      _A[11][2] = 0;             //      B3 0

      _A[12][0] = 12*v;          // duuw B1 12v
      _A[12][1] = 0;             //      B2 0
      _A[12][2] = 0;             //      B3 0

      _A[13][0] = 0;             // duvv B1 0
      _A[13][1] = 12*w;          //      B2 12w
      _A[13][2] = 0;             //      B3 0

      _A[14][0] = 12*u;          // duvw B1 12u
      _A[14][1] = 12*v;          //      B2 12v
      _A[14][2] = 12*w;          //      B3 12w

      _A[15][0] = 0;             // duww B1 0
      _A[15][1] = 0;             //      B2 0
      _A[15][2] = 12*v;          //      B3 12v

      _A[16][0] = 0;             // dvvv B1 0
      _A[16][1] = -12;           //      B2 -12
      _A[16][2] = 0;             //      B3 0

      _A[17][0] = 0;             // dvvw B1 0
      _A[17][1] = 12*u;          //      B2 12u
      _A[17][2] = 0;             //      B3 0

      _A[18][0] = 0;             // dvww B1 0
      _A[18][1] = 0;             //      B2 0
      _A[18][2] = 12*u;          //      B3 12u

      _A[19][0] = 0;             // dwww B1 0
      _A[19][1] = 0;             //      B2 0
      _A[19][2] = -12;           //      B3 -12
  }


  // Polynomial barycentrisk basert basis, orden 2
  //***********************************************

  template <typename T>
  inline
  void BasisTriangleERBS<T>::computeB4( T u, T v, T w )
  {

      T uu = u*u;
      T vv = v*v;
      T ww = w*w;
      T uv = u*v;
      T uw = u*w;
      T vw = v*w;
      T u3 = uu*u;
      T v3 = vv*v;
      T w3 = ww*w;

      // B1, B2, B3
      _A[0][0] = u3*(30*vw*(3*vw-u+1)+6*uu-15*u+10); // u�(30vw(3vw-u+1)+6uu-15u+10)
      _A[0][1] = v3*(30*uw*(3*uw-v+1)+6*vv-15*v+10); // v�(30uw(3uw-v+1)+6vv-15v+10)
      _A[0][2] = w3*(30*uv*(3*uv-w+1)+6*ww-15*w+10); // w�(30uv(3uv-w+1)+6ww-15w+10)

      // du: B1, B2, B3
      _A[1][0] = 30*uu*(vw*(9*vw-4*u+3)+u*(u-2)+1);  // 30u�(vw(9vw-4u+3)+u(u-2)+1)
      _A[1][1] = 30*v3*w*(6*uw-v+1);                 // 30v�w(6uw-v+1)
      _A[1][2] = 30*w3*v*(6*uv-w+1);                 // 30vw�(6uv-w+1)

      // dv: B1, B2, B3
      _A[2][0] = 30*u3*w*(6*vw-u+1);                // 30u�w(6vw-u+1)
      _A[2][1] = 30*vv*(uw*(9*uw-4*v+3)+v*(v-2)+1); // 30v�(uw(9uw-4v+3)+v(v-2)+1)
      _A[2][2] = 30*w3*u*(6*uv-w+1);                // 30uw�(6uv-w+1)

      // dw: B1, B2, B3
      _A[3][0] = 30*u3*v*(6*vw-u+1);                // 30u�v(6vw-u+1)
      _A[3][1] = 30*v3*u*(6*uw-v+1);                // 30v�u(6uw-v+1)
      _A[3][2] = 30*ww*(uv*(9*uv-4*w+3)+w*(w-2)+1); // 30w�(uv(9uv-4w+3)+w(w-2)+1)

      // duu: B1, B2, B3
      _A[4][0] = B4_a(u,2*u,3*vw);               // 60u((2u-1)(u-1)-3vw(2u-3vw-1))
      _A[4][1] = 180*v3*ww;                      // 180v�w�
      _A[4][2] = 180*w3*vv;                      // 180w�v�

      // duv: B1, B2, B3
      _A[5][0] = B4_b(uu*w,vw,u);                // 30u�w(18vw-4u+3)
      _A[5][1] = B4_b(vv*w,uw,v);                // 30v�w(18uw-4v+3)
      _A[5][2] = B4_c(w3,uv,w);                  // 30w�(12uv-w+1)

      // duw: B1, B2, B3
      _A[6][0] = B4_b(uu*v,vw,u);                // 30u�v(18vw-4u+3)
      _A[6][1] = B4_c(v3,uw,v);                  // 30v�(12uw-v+1)
      _A[6][2] = B4_b(ww*v,uv,w);                // 30w�(12uv-w+1)

      // dvv: B1, B2, B3
      _A[7][0] = 180*u3*ww;                      // 180u�w�
      _A[7][1] = B4_a(v,2*v,3*uw);               // 60v((2v-1)(v-1)-3uw(2v-3uw-1))
      _A[7][2] = 180*w3*uu;                      // 180w�u�

      // dvw: B1, B2, B3
      _A[8][0] = B4_c(u3,vw,u);                  // 30u�(12vw-u+1)
      _A[8][1] = B4_b(vv*u,uw,v);                // 30uv�(18uw-4v+3)
      _A[8][2] = B4_b(ww*u,uv,w);                // 30uw�(18uv-4w+3)

      // dww: B1, B2, B3
      _A[9][0] = 180*u3*vv;                      // 180u�v�
      _A[9][1] = 180*v3*uu;                      // 180v�u�
      _A[9][2] = B4_a(w,2*w,3*uv);               // 60w((2w-1)(w-1)-3uv(2w-3uv-1))

      // duuu: B1, B2, B3
      _A[10][0] = B4_d(u,vw);                    // 60(6u(u-1)-3vw(4u-3vw-1)+1)
      _A[10][1] = 0;                             // 0
      _A[10][2] = 0;                             // 0

      // duuv: B1, B2, B3
      _A[11][0] = 180*uw*(6*vw-2*u+1);           // 180uw(6vw-2u+1)
      _A[11][1] = 540*vv*ww;                     // 540v�w�
      _A[11][2] = 360*w3*v;                      // 360w�v

      // duuw: B1, B2, B3
      _A[12][0] = 180*uv*(6*vw-2*u+1);           // 180uv(6vw-2u+1)
      _A[12][1] = 360*v3*w;                      // 360v�w
      _A[12][2] = 540*vv*ww;                     // 540v�w�

      // duvv: B1, B2, B3
      _A[13][0] = 540*uu*ww;                     // 540u�w�
      _A[13][1] = 180*vw*(6*uw-2*v+1);           // 180vw(6uw-2v+1)
      _A[13][2] = 360*w3*u;                      // 360w�u

      // duvw: B1, B2, B3
      _A[14][0] = 30*uu*(36*vw-4*u+3);           // 30u�(36vw-4u+3)
      _A[14][1] = 30*vv*(36*uw-4*v+3);           // 30v�(36uw-4v+3)
      _A[14][2] = 30*ww*(36*uv-4*w+3);           // 30w�(36uv-4w+3)

      // duww: B1, B2, B3
      _A[15][0] = 540*uu*vv;                     // 540u�v�
      _A[15][1] = 360*v3*u;                      // 360v�u
      _A[15][2] = 180*vw*(6*uv-2*w+1);           // 180vw(6uv-2w+1)

      // dvvv: B1, B2, B3
      _A[16][0] = 0;                             // 0
      _A[16][1] = B4_d(v,uw);                    // 60(6v(v-1)-3uw(4v-3uw-1)+1)
      _A[16][2] = 0;                             // 0

      // dvvw: B1, B2, B3
      _A[17][0] = 360*u3*w;                      // 360u�w
      _A[17][1] = 180*uv*(6*uw-2*v+1);           // 180uv(6uw-2v+1)
      _A[17][2] = 540*uu*ww;                     // 540u�w�

      // dvww: B1, B2, B3
      _A[18][0] = 360*u3*v;                      // 360u�v
      _A[18][1] = 540*uu*vv;                     // 540u�v�
      _A[18][2] = 180*uw*(6*uv-2*w+1);           // 180uw(6uv-2w+1)

      // dwww: B1, B2, B3
      _A[19][0] = 0;                             // 0
      _A[19][1] = 0;                             // 0
      _A[19][2] = B4_d(w,uv);                    // 60(6w(w-1)-3uv(4w-3uv-1)+1)
  }

  template <typename T>
  inline
  T BasisTriangleERBS<T>::B4_a( T u, T u2, T vw3) {
    return 60*u*(vw3*(vw3-u2+1)+u*(u2-3)+1);
  }
  template <typename T>
  inline
  T BasisTriangleERBS<T>::B4_b( T uuw, T vw, T u) {
    return 30*uuw*(18*vw-4*u+3);
  }
  template <typename T>
  inline
  T BasisTriangleERBS<T>::B4_c( T u3, T vw, T u) {
    return 30*u3*(12*vw-u+1);
  }
  template <typename T>
  inline
  T BasisTriangleERBS<T>::B4_d( T u, T vw) {
    return 60*(6*u*(u-1)-3*vw*(4*u-3*vw-1)+1);
  }


  template <typename T>
  inline
  Vector< Vector<T,3>, 20>&  BasisTriangleERBS<T>::eval( T u, T v, T w)
  {
      if(_type == ERBS_RATIONAL)        computeB1( u, v, w );
      else if (_type == BFBS_RATIONAL)  computeB1( u, v, w );
      else if (_type == POLY_RATIONAL)  computeB2( u, v, w );
      else if (_type == ERBS)           computeB4( u, v, w );
      else if (_type == BFBS_2)         computeB3( u, v, w );
      else if (_type == BFBS_3)         computeB4( u, v, w );

      return _A;
  }

} // END namespace GMlib


