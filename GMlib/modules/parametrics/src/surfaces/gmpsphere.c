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
  PSphere<T>::PSphere( T radius ) {
    init();
    _radius = radius;
  }


  template <typename T>
  inline
  PSphere<T>::PSphere( T radius, int /*s1*/, int /*s2*/ ) {
    init();
    _radius = radius;
  }


  template <typename T>
  inline
  PSphere<T>::PSphere( const PSphere<T>& copy ) : PSurf<T,3>( copy ) {}


  template <typename T>
  PSphere<T>::~PSphere() {}


  //*****************************************
  //            Local functons             **
  //*****************************************


  template <typename T>
  T PSphere<T>::getRadius() const {
    return _radius;
  }


  template <typename T>
  inline
  void PSphere<T>::setRadius( T radius ) {
    _radius = radius;
  }



  //**************************************************
  // Overrided (public) virtual functons from PSurf **
  //**************************************************



  template <typename T>
  void PSphere<T>::replot( int m1, int m2, int d1, int d2 ) {

      if(d1==0 && m1>3 && m1<31) {

          DVector< DVector< Vector<T,3> > >   p;
          DMatrix< Vector<T,3> >              normals;
          Sphere<T,3>                         s(Point<T,3>(T(0)), _radius);

          // Sample Positions
          resample(p, m1);

          // Make normal map
          makeNmap( normals, m1, 64, 64);

          // Replot Visaulizers
          for( int i = 0; i < this->_psurf_visualizers.getSize(); i++ )
            this->_psurf_visualizers[i]->replot( p, normals, m1, isClosedU(), isClosedV() );

          Parametrics<T,2,3>::setSurroundingSphere(s);
      } else
          PSurf<T,3>::replot( m1, m2, d1, d2 );
  }



  template <typename T>
  bool PSphere<T>::isClosedU() const {
    return true;
  }


  template <typename T>
  bool PSphere<T>::isClosedV() const {
    return false;
  }



  //*****************************************************
  // Overrided (protected) virtual functons from PSurf **
  //*****************************************************

  template <typename T>
  void PSphere<T>::eval(T u, T v, int d1, int d2, bool /*lu*/, bool /*lv*/ ) const {

    this->_p.setDim( d1+1, d2+1 );

    T cos_u =  cos(u);
    T sin_u =  sin(u);
    T cos_v = _radius * cos(v);
    T sin_v = _radius * sin(v);


    this->_p[0][0][0] =  cos_u * cos_v;	// S
    this->_p[0][0][1] =  sin_u * cos_v;
    this->_p[0][0][2] =  sin_v;

    if( this->_dm == GM_DERIVATION_EXPLICIT ) {

      if(GMutils::compValueF(cos_v,T(0))) cos_v = T(1e-4);

      T cu_cv = cos_u * cos_v;
      T su_cv = sin_u * cos_v;
      T cu_sv = cos_u * sin_v;
      T su_sv = sin_u * sin_v;

      if(d1) {
        this->_p[1][0][0] = -su_cv;	// S_u
        this->_p[1][0][1] =  cu_cv;
        this->_p[1][0][2] =  T(0);
      }
      if(d2) {
        this->_p[0][1][0] = -cu_sv;	// S_v
        this->_p[0][1][1] = -su_sv;
        this->_p[0][1][2] =  cos_v;
      }
      if(d1 && d2) {
        this->_p[1][1][0] =  su_sv;	// S_uv
        this->_p[1][1][1] = -cu_sv;
        this->_p[1][1][2] =  T(0);
      }
      if(d1>1) {
        this->_p[2][0][0] = -cu_cv;	// S_uu
        this->_p[2][0][1] = -su_cv;
        this->_p[2][0][2] =  T(0);
      }
      if(d2>1) {
        this->_p[0][2][0] = -cu_cv;	// S_vv
        this->_p[0][2][1] = -su_cv;
        this->_p[0][2][2] = -sin_v;
      }
      if(d1>1 && d2) {
        this->_p[2][1][0] =  cu_sv;	// S_uuv
        this->_p[2][1][1] =  su_sv;
        this->_p[2][1][2] =  T(0);
      }
      if(d1 && d2>1) {
        this->_p[1][2][0] =  su_cv;	// S_uvv
        this->_p[1][2][1] = -cu_cv;
        this->_p[1][2][2] =  T(0);
      }
      if(d1>1 && d2>1) {
        this->_p[2][2][0] =  cu_cv;	// S_uuvv
        this->_p[2][2][1] =  su_cv;
        this->_p[2][2][2] =  T(0);
      }
      if(d1>2) {
        this->_p[3][0][0] =  su_cv;	// S_uuu
        this->_p[3][0][1] = -cu_cv;
        this->_p[3][0][2] =  T(0);
      }
      if(d2>2) {
        this->_p[0][3][0] =  cu_sv;	// S_vvv
        this->_p[0][3][1] =  su_sv;
        this->_p[0][3][2] = -cos_v;
      }
      if(d1>2 && d2) {
        this->_p[3][1][0] = -su_sv;	// S_uuuv
        this->_p[3][1][1] =  cu_sv;
        this->_p[3][1][2] =  T(0);
      }
      if(d1>2 && d2>1) {
        this->_p[3][2][0] = -su_cv;	// S_uuuvv
        this->_p[3][2][1] =  cu_cv;
        this->_p[3][2][2] =  T(0);
      }
      if(d1 && d2>2) {
        this->_p[1][3][0] = -su_sv;	// S_uvvv
        this->_p[1][3][1] =  cu_sv;
        this->_p[1][3][2] =  T(0);
      }
      if(d1>1 && d2>2) {
        this->_p[2][3][0] = -cu_sv;	// S_uuvvv
        this->_p[2][3][1] = -su_sv;
        this->_p[2][3][2] =  T(0);
      }
      if(d1>2 && d2>2) {
        this->_p[3][3][0] =  su_sv;	// S_uuuvvv
        this->_p[3][3][1] = -cu_sv;
        this->_p[3][3][2] =  T(0);
      }
    }
  }


  template <typename T>
  T PSphere<T>::getStartPU() const {
    return T(0);
  }


  template <typename T>
  T PSphere<T>::getEndPU() const {
    return T(M_2PI);
  }


  template <typename T>
  T PSphere<T>::getStartPV() const {
    return -T(M_PI_2);
  }


  template <typename T>
  T PSphere<T>::getEndPV() const {
    return T(M_PI_2);
  }


  template <typename T>
  void PSphere<T>::resampleNormals(const DMatrix<DMatrix<Vector<T,3> > >&, DMatrix<Vector<T,3> >& n) const {
    if(_nmap.getDim1() == 0) makeNmap(32,32);//(66,66);
    n = _nmap;
  }




  //*****************************************
  //     Local (protected) functons        **
  //*****************************************

  template <typename T>
  void PSphere<T>::init() {

    this->_dm = GM_DERIVATION_EXPLICIT;

    if(_mat1.getDim()<1) init_mat1();
    if(_nmap.getDim1() == 0) makeNmap(130,130);
  }



  //*******************************************
  //  Private functon and static normal map  **
  //*******************************************


  template <typename T>
  DMatrix<Vector<T,3> > PSphere<T>::_nmap;

  template <typename T>
  inline
  void PSphere<T>::makeNmap(int s, int t) const {

      _nmap.setDim(s,t);
      double su = getStartPU();
      double sv = getStartPV();
      double du = getEndPU()-su;
      double dv = getEndPV()-sv;

      DVector<Vector<T,2> > c(t);
      for(int j=0; j<t; j++){
          double u = su + j/double(t-1)*du;
          c[j] = {(T)cos(u), (T)sin(u)};
      }

      for(int i=0; i<s; i++) {
          double v = sv + i/double(s-1)*dv;
          T cos_v = cos(v);
          T sin_v = sin(v);
          for(int j=0; j<t; j++) {
              _nmap[i][j][0] =  c[j][0] * cos_v;
              _nmap[i][j][1] =  c[j][1] * cos_v;
              _nmap[i][j][2] =  sin_v;
          }
      }
  }




  template <typename T>
  inline
  void PSphere<T>::makeNmap(DMatrix<Vector<T,3>>& nm, int m, int s, int t) const {

      nm.setDim(s,t);
      double su = getStartPU();
      double sv = getStartPV();
      double du = getEndPU()-su;
      double dv = getEndPV()-sv;

      double sm = (s-1)/double(m-1);
      int    si = sm;
      DVector<double> sa(si+1);
      sa[0] = T(1);
      for(int i=1; i<=si; i++)
          sa[i]= i/sm;

      std::cout << "sm=" << sm << std::endl;
      std::cout << "si=" << si << std::endl;
      std::cout << "sa=" << sa << std::endl;

      DVector<Vector<T,2> > c(t);
      for(int j=0; j<t; j++){
          double u = su + j/double(t-1)*du;
          c[j] = {(T)cos(u), (T)sin(u)};
      }

      for(int i=0; i<=si; i++) {
          double v     = sv + i/double(s-1)*dv;
          double dt    = du/((t-1)*sa[i]);
          double cos_v = cos(v);
          double sin_v = sin(v);
std::cout << "u=";
          for(int j=0; j<t; j++) {
              double u = su + j*dt;
std::cout << "  " << u;
              nm[i][j][0] =  cos(u) * cos_v;
              nm[i][j][1] =  sin(u) * cos_v;
              nm[i][j][2] =  sin_v;
          }
std::cout << std::endl;
      }
      for(int i=si+1; i<s-si-1; i++) {
          double v     = sv + i/double(s-1)*dv;
          double cos_v = cos(v);
          double sin_v = sin(v);
          for(int j=0; j<t; j++) {
              nm[i][j][0] =  c[j][0] * cos_v;
              nm[i][j][1] =  c[j][1] * cos_v;
              nm[i][j][2] =  sin_v;
          }
      }
      for(int i=s-si-1; i<s; i++) {
          double v     = sv + i/double(s-1)*dv;
          double dt    = du/((t-1)*sa[s-1-i]);
          double cos_v = cos(v);
          double sin_v = sin(v);
          for(int j=0; j<t; j++) {
              double u = su + j*dt;
              nm[i][j][0] =  cos(u) * cos_v;
              nm[i][j][1] =  sin(u) * cos_v;
              nm[i][j][2] =  sin_v;
          }
      }
  }



  template <typename T>
  inline
  void PSphere<T>::resample( DVector< DVector< Vector<T,3> > >& p, int m ) const {

      p.setDim(m);

      double su = getStartPU();
      double sv = getStartPV();
      double du = getEndPU()-su;
      double dv = getEndPV()-sv;

      for(int i=0; i<m; i++) {

          double v =  sv + i/double(m-1)*dv;
          T  cos_v = _radius * cos(v);
          T  sin_v = _radius * sin(v);

          int n = _mat1[m][i]+1;
          p[i].setDim(n);

          for(int j=0; j<n; j++) {

              double u = su + j/double(n-1)*du;
              T  cos_u =  cos(u);
              T  sin_u =  sin(u);

              p[i][j][0] =  cos_u * cos_v;
              p[i][j][1] =  sin_u * cos_v;
              p[i][j][2] =  sin_v;
          }
      }
  }


  template <typename T>
  DVector<DVector<int> > PSphere<T>::_mat1;

  template <typename T>
  DVector<Vector<int,2> > PSphere<T>::_mat1_size;

  template <typename T>
  inline
  void PSphere<T>::init_mat1() {

     _mat1.setDim(31);
     _mat1_size.setDim(31);
     for(int i=3; i<31; i++) _mat1[i].setDim(i);

     int m3[]  = {1,4,1}; _mat1[3] = m3; _mat1_size[3]= {6, 16};
     int m4[]  = {1,6,6,1}; _mat1[4] = m4; _mat1_size[3]= {14, 34};
     int m5[]  = {1,6,7,6,1}; _mat1[5] = m5; _mat1_size[3]= {21, 50};
     int m6[]  = {1,6,12,12,6,1}; _mat1[6] = m6; _mat1_size[3]= {38, 90};
     int m7[]  = {1,6,12,13,12,6,1}; _mat1[7] = m7; _mat1_size[3]= {51, 118};
     int m8[]  = {1,6,12,13,13,12,6,1}; _mat1[8] = m8; _mat1_size[3]= {64, 146};
     int m9[]  = {1,6,12,13,14,13,12,6,1}; _mat1[9] = m9; _mat1_size[3]= {78, 176};
     int m10[] = {1,6,12,18,19,19,18,12,6,1}; _mat1[10] = m10; _mat1_size[3]= {112, 250};
     int m11[] = {1,6,12,18,19,20,19,18,12,6,1}; _mat1[11] = m11; _mat1_size[3]= {132, 292};
     int m12[] = {1,6,12,18,19,20,20,19,18,12,6,1}; _mat1[12] = m12; _mat1_size[3]= {152, 334};
     int m13[] = {1,6,12,18,19,20,21,20,19,18,12,6,1}; _mat1[13] = m13; _mat1_size[3]= {173, 378};
     int m14[] = {1,6,12,18,24,25,26,26,25,24,18,12,6,1}; _mat1[14] = m14; _mat1_size[3]= {224, 486};
     int m15[] = {1,6,12,18,24,25,26,27,26,25,24,18,12,6,1}; _mat1[15] = m15; _mat1_size[3]= {251, 542};
     int m16[] = {1,6,12,18,24,25,26,27,27,26,25,24,18,12,6,1}; _mat1[16] = m16; _mat1_size[3]= {278, 598};
     int m17[] = {1,6,12,18,24,25,26,27,28,27,26,25,24,18,12,6,1}; _mat1[17] = m17; _mat1_size[3]= {306, 656};
     int m18[] = {1,6,12,18,24,30,31,32,33,33,32,31,30,24,18,12,6,1}; _mat1[18] = m18; _mat1_size[3]= {374, 798};
     int m19[] = {1,6,12,18,24,30,31,32,33,34,33,32,31,30,24,18,12,6,1}; _mat1[19] = m19; _mat1_size[3]= {408, 868};
     int m20[] = {1,6,12,18,24,30,31,32,33,34,34,33,32,31,30,24,18,12,6,1}; _mat1[20] = m20; _mat1_size[3]= {442, 938};
     int m21[] = {1,6,12,18,24,30,31,32,33,34,35,34,33,32,31,30,24,18,12,6,1}; _mat1[21] = m21; _mat1_size[3]= {477, 1010};
     int m22[] = {1,6,12,18,24,30,36,37,38,39,40,40,39,38,37,36,30,24,18,12,6,1}; _mat1[22] = m22; _mat1_size[3]= {562, 1186};
     int m23[] = {1,6,12,18,24,30,36,37,38,39,40,41,40,39,38,37,36,30,24,18,12,6,1}; _mat1[23] = m23; _mat1_size[3]= {603, 1270};
     int m24[] = {1,6,12,18,24,30,36,37,38,39,40,41,41,40,39,38,37,36,30,24,18,12,6,1}; _mat1[24] = m24; _mat1_size[3]= {644, 1354};
     int m25[] = {1,6,12,18,24,30,36,37,38,39,40,41,42,41,40,39,38,37,36,30,24,18,12,6,1}; _mat1[25] = m25; _mat1_size[3]= {686, 1440};
     int m26[] = {1,6,12,18,24,30,36,42,43,44,45,49,50,50,49,45,44,43,42,36,30,24,18,12,6,1}; _mat1[26] = m26; _mat1_size[3]= {800, 1674};
     int m27[] = {1,6,12,18,24,30,36,42,43,44,45,46,47,48,47,46,45,44,43,42,36,30,24,18,12,6,1}; _mat1[27] = m27; _mat1_size[3]= {836, 1748};
     int m28[] = {1,6,12,18,24,30,36,42,43,44,45,46,47,48,48,47,46,45,44,43,42,36,30,24,18,12,6,1}; _mat1[28] = m28; _mat1_size[3]= {884, 1846};
     int m29[] = {1,6,12,18,24,30,36,42,43,44,45,46,47,48,49,48,47,46,45,44,43,42,36,30,24,18,12,6,1}; _mat1[29] = m29; _mat1_size[3]= {933, 1946};
     int m30[] = {1,6,12,18,24,30,36,42,48,49,50,51,52,53,54,54,53,52,51,50,49,48,42,36,30,24,18,12,6,1}; _mat1[30] = m30; _mat1_size[3]= {1052, 2190};
  }


} // END namespace GMlib

