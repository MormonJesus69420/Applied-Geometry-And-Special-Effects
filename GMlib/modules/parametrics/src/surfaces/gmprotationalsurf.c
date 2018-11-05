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
  PRotationalSurf<T>::PRotationalSurf( PCurve<T,3>* cu, T start_par, T end_par ) {

    init();

    _cu = cu;
    _start_par = start_par;
    _end_par = end_par;
  }


  template <typename T>
  inline
  PRotationalSurf<T>::PRotationalSurf( const PRotationalSurf<T>& copy ) : PSurf<T,3>( copy ) {

  }


  template <typename T>
  PRotationalSurf<T>::~PRotationalSurf() {}



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************


  template <typename T>
  bool PRotationalSurf<T>::isClosedU() const {
    return false;
  }


  template <typename T>
  bool PRotationalSurf<T>::isClosedV() const {
    return abs(_end_par - _start_par - M_2PI) < 1e-6;
  }


  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PRotationalSurf<T>::eval(T u, T v, int d1,int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim(d1+1,d2+1);
    DVector<Vector<T,3> > uu =  _cu->evaluate( u, d1 );
    T sv = sin(v);
    T cv = cos(v);

    // S
    this->_p[0][0][0] =    uu[0][0]*cv;
    this->_p[0][0][1] =    uu[0][0]*sv;
    this->_p[0][0][2] =    uu[0][1];

    if( this->_dm == GM_DERIVATION_EXPLICIT )
    {
      if(d1) {                            // Su
        this->_p[1][0][0] =   uu[1][0]*cv;
        this->_p[1][0][1] =   uu[1][0]*sv;
        this->_p[1][0][2] =   uu[1][1];   
        if(d2)  {                         // Suv
          this->_p[1][1][0] =  -uu[1][0]*sv;
          this->_p[1][1][1] =   uu[1][0]*cv;
          this->_p[1][1][2] =   T(0);
          if(d2>1) {                      // Suvv
            this->_p[1][2][0] =  -uu[1][0]*cv;
            this->_p[1][2][1] =  -uu[1][0]*sv;
            this->_p[1][2][2] =   T(0);
            if(d2>2) {                    // Suvvv
              this->_p[1][3][0] =   uu[1][0]*sv;
              this->_p[1][3][1] =  -uu[1][0]*cv;
              this->_p[1][3][2] =   T(0);
            }
          }
        }
        if(d1>1) {                        // Suu
          this->_p[2][0][0] =   uu[2][0]*cv;
          this->_p[2][0][1] =   uu[2][0]*sv;
          this->_p[2][0][2] =   uu[2][1];
          if(d2) {                        // Suuv
            this->_p[2][1][0] =  -uu[2][0]*sv;
            this->_p[2][1][1] =   uu[2][0]*cv;
            this->_p[2][1][2] =   T(0);
            if(d2>1) {                    // Suuvv
              this->_p[2][2][0] =  -uu[2][0]*cv;
              this->_p[2][2][1] =  -uu[2][0]*sv;
              this->_p[2][2][2] =   T(0);
              if(d2>2) {                  // Suuvvv
                this->_p[2][3][0] =   uu[2][0]*sv;
                this->_p[2][3][1] =  -uu[2][0]*cv;
                this->_p[2][3][2] =   T(0);
              }
            }
          }

          if(d1>2) {                      // Suuu
            this->_p[3][0][0] =   uu[3][0]*cv;
            this->_p[3][0][1] =   uu[3][0]*sv;
            this->_p[3][0][2] =   uu[3][1];
            if(d2) {                      // Suuuv
              this->_p[3][1][0] =  -uu[3][0]*sv;
              this->_p[3][1][1] =   uu[3][0]*cv;
              this->_p[3][1][2] =   T(0);
              if(d2>1)  {                 // Suuuvv
                this->_p[3][2][0] =  -uu[3][0]*cv;
                this->_p[3][2][1] =  -uu[3][0]*sv;
                this->_p[3][2][2] =   T(0);
                if(d2>2) {                // Suuuvvv
                  this->_p[3][3][0] =   uu[3][0]*sv;
                  this->_p[3][3][1] =  -uu[3][0]*cv;
                  this->_p[3][3][2] =   T(0);
                }
              }
            }
          }
        }
      }

      if(d2) {                            // Sv
        this->_p[0][1][0] =  -uu[0][0]*sv;
        this->_p[0][1][1] =   uu[0][0]*cv;
        this->_p[0][1][2] =   T(0);
        if(d2>1) {                        // Svv
          this->_p[0][2][0] =  -uu[0][0]*cv;
          this->_p[0][2][1] =  -uu[0][0]*sv;
          this->_p[0][2][2] =   T(0);
          if(d2>2) {                      // Svvv
            this->_p[0][3][0] =   uu[0][0]*sv;
            this->_p[0][3][1] =  -uu[0][0]*cv;
            this->_p[0][3][2] =   T(0);
          }
        }
      }
    }
  }



  template <typename T>
  T PRotationalSurf<T>::getStartPU() const {
    return _cu->getParStart();
  }


  template <typename T>
  T PRotationalSurf<T>::getEndPU() const {
    return _cu->getParEnd();
  }


  template <typename T>
  T PRotationalSurf<T>::getStartPV() const {
    return _start_par;
  }


  template <typename T>
  T PRotationalSurf<T>::getEndPV() const {
    return _end_par;
  }


  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PRotationalSurf<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;
  }


}

