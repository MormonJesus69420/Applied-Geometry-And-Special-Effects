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
  PERBSTriangle<T>::PERBSTriangle( PTriangle<T,3>* c0, PTriangle<T,3>* c1, PTriangle<T,3>* c2, BasisTriangleType t)
  {
    _B = new BasisTriangleERBS<T>(t);

    _c.setDim( 3 );
    _c[0] = c0; insertPatch( c0 );
    _c[1] = c1; insertPatch( c1 );
    _c[2] = c2; insertPatch( c2 );
  }


  template <typename T>
  PERBSTriangle<T>::~PERBSTriangle() {

    for( int i = 0; i < _c.getDim(); i++ )
      SceneObject::remove( _c[i] );

    if( _B )  delete _B;
  }

  template <typename T>
  inline
  void PERBSTriangle<T>::edit( SceneObject *obj ) {

    int i;
    for( i = 0; i < _c.getDim(); i++ )
      if( _c[i] == obj )
        goto edit_loop_break;

    edit_loop_break:

    // If Bezier Patch
    PBezierTriangle<T> *bezier = dynamic_cast<PBezierTriangle<T>*>(_c[i]);
    if( bezier )
      bezier->updateCoeffs( _c[i]->getPos() - _c[i]->evaluateParent( 0.5, 0.5, 0 )(0) );

    PTriangle<T,3>::replot();
  }

  template <typename T>
  inline
  void PERBSTriangle<T>::eval( T u, T v, T w, int d ) const // d can be: 0,1,2,3
  {
     if(d==0)        this->_p.setDim(1);
     else if(d==1)   this->_p.setDim(4);
     else if(d==2)   this->_p.setDim(10);
     else            this->_p.setDim(20); // d==3

     DVector< Vector<T,3> > c0 = _c[0]->evaluateParent( u, v, d); // (u,v,w)
     DVector< Vector<T,3> > c1 = _c[1]->evaluateParent( v, w, d); // (v,w,u)
     DVector< Vector<T,3> > c2 = _c[2]->evaluateParent( w, u, d); // (w,u,v)

     Vector<Vector<T,3>,20> B = _B->eval(u,v,w);

     this->_p[0] = c0[0] * B[0][0] + c1[0] * B[0][1] + c2[0] * B[0][2];
     if(d>0)
     {              //        D_u B1     C_u      B1
        this->_p[1] = c0[0] * B[1][0] + c0[1] * B[0][0] // S_u
                    //        D_u B2     C_w      B2
                    + c1[0] * B[1][1] + c1[3] * B[0][1]
                    //        D_u B3     C_v      B3
                    + c2[0] * B[1][2] + c2[2] * B[0][2];
                    //        D_v B1     C_v      B1
        this->_p[2] = c0[0] * B[2][0] + c0[2] * B[0][0] // S_v
                    //        D_v B2     C_u      B2
                    + c1[0] * B[2][1] + c1[1] * B[0][1]
                    //        D_v B3     C_w      B3
                    + c2[0] * B[2][2] + c2[3] * B[0][2];
                    //        D_w B1     C_w     B1
        this->_p[3] = c0[0] * B[3][0] + c0[3] * B[0][0] // S_w
                    //        D_w B2     C_v     B2
                    + c1[0] * B[3][1] + c1[2] * B[0][1]
                    //        D_w B3     C_u     B3
                    + c2[0] * B[3][2] + c2[1] * B[0][2];
        if(d>1)
        {              //    C * B_uu    + 2 C_u * B_u         +  C_uu * B
           this->_p[4] = c0[0] * B[4][0] + c0[1] * (2*B[1][0]) + c0[4] * B[0][0]                  // S_uu
                       + c1[0] * B[4][1] + c1[3] * (2*B[1][1]) + c1[9] * B[0][1]
                       + c2[0] * B[4][2] + c2[2] * (2*B[1][2]) + c2[7] * B[0][2];
                       //     c*B_uv     +    C_v*B_u      +    C_u*B_v      +   C_uv*B
           this->_p[5] = c0[0] * B[5][0] + c0[2] * B[1][0] + c0[1] * B[2][0] + c0[5] * B[0][0]    // S_uv
                       + c1[0] * B[5][1] + c1[1] * B[1][1] + c1[3] * B[2][1] + c1[6] * B[0][1]
                       + c2[0] * B[5][2] + c2[3] * B[1][2] + c2[2] * B[2][2] + c2[8] * B[0][2];
                       //     c*B_uw     +    C_w*B_u      +    C_u*B_w      +   C_uw*B
           this->_p[6] = c0[0] * B[6][0] + c0[3] * B[1][0] + c0[1] * B[3][0] + c0[6] * B[0][0]    // S_uw
                       + c1[0] * B[6][1] + c1[2] * B[1][1] + c1[3] * B[3][1] + c1[8] * B[0][1]
                       + c2[0] * B[6][2] + c2[1] * B[1][2] + c2[2] * B[3][2] + c2[5] * B[0][2];
                       //     C*B_vv     +    C_v*B_v          +   C_vv*B
           this->_p[7] = c0[0] * B[7][0] + c0[2] * (2*B[2][0]) + c0[7] * B[0][0]                  // S_vv
                       + c1[0] * B[7][1] + c1[1] * (2*B[2][1]) + c1[4] * B[0][1]
                       + c2[0] * B[7][2] + c2[3] * (2*B[2][2]) + c2[9] * B[0][2];
                       //     c*B_vw     +    C_w*B_v      +    C_v*B_w      +   C_vw*B
           this->_p[8] = c0[0] * B[8][0] + c0[3] * B[2][0] + c0[2] * B[3][0] + c0[8] * B[0][0]    // S_vw
                       + c1[0] * B[8][1] + c1[2] * B[2][1] + c1[1] * B[3][1] + c1[5] * B[0][1]
                       + c2[0] * B[8][2] + c2[1] * B[2][2] + c2[3] * B[3][2] + c2[6] * B[0][2];
                       //     C*B_ww     +    C_w*B_w          +   C_ww*B
           this->_p[9] = c0[0] * B[9][0] + c0[3] * (2*B[3][0]) + c0[9] * B[0][0]                  // S_ww
                       + c1[0] * B[9][1] + c1[2] * (2*B[3][1]) + c1[7] * B[0][1]
                       + c2[0] * B[9][2] + c2[1] * (2*B[3][2]) + c2[4] * B[0][2];
        }
        if(d>2)
        {                //   C * B_uuu    + 3 C_u *    B_uu     + 3 C_uu    *B_u      + C_uuu  * B
           this->_p[10] = c0[0] * B[10][0] + c0[1] * (3*B[4][0]) + c0[4] * (3*B[1][0]) + c0[10] * B[0][0]  // S_uuu
                        + c1[0] * B[10][1] + c1[3] * (3*B[4][1]) + c1[9] * (3*B[1][1]) + c1[19] * B[0][1]
                        + c2[0] * B[10][2] + c2[2] * (3*B[4][2]) + c2[7] * (3*B[1][2]) + c2[16] * B[0][2];
                        //     c*B_uuv     +    C_v*B_uu     +  2 C_u*B_uv         + 2 C_uv*B_u          +   C_uu*B_v      +  C_uuv*B
           this->_p[11] = c0[0] * B[11][0] + c0[2] * B[4][0] + c0[1] * (2*B[5][0]) + c0[5] * (2*B[1][0]) + c0[4] * B[2][0] + c0[11] * B[0][0]  // S_uuv
                        + c1[0] * B[11][1] + c1[1] * B[4][1] + c1[3] * (2*B[5][1]) + c1[6] * (2*B[1][1]) + c1[9] * B[2][1] + c1[15] * B[0][1]
                        + c2[0] * B[11][2] + c2[3] * B[4][2] + c2[2] * (2*B[5][2]) + c2[8] * (2*B[1][2]) + c2[7] * B[2][2] + c2[17] * B[0][2];
                        //     c*B_uuw     +    C_w*B_uu     +  2 C_u*B_uw         + 2 C_uw*B_u          +   C_uu*B_w      +  C_uuw*B
           this->_p[12] = c0[0] * B[12][0] + c0[3] * B[4][0] + c0[1] * (2*B[6][0]) + c0[6] * (2*B[1][0]) + c0[4] * B[3][0] + c0[12] * B[0][0]  // S_uuw
                        + c1[0] * B[12][1] + c1[2] * B[4][1] + c1[3] * (2*B[6][1]) + c1[8] * (2*B[1][1]) + c1[9] * B[3][1] + c1[18] * B[0][1]
                        + c2[0] * B[12][2] + c2[1] * B[4][2] + c2[2] * (2*B[6][2]) + c2[5] * (2*B[1][2]) + c2[7] * B[3][2] + c2[13] * B[0][2];
//            cout << "p_12=" << B << endl;
                        //     C*B_uvv     +    C_u*B_vv     +  2 C_v*B_uv         + 2 C_uv*B_v          +   C_vv*B_u      +  C_uvv*B
           this->_p[13] = c0[0] * B[13][0] + c0[1] * B[7][0] + c0[2] * (2*B[5][0]) + c0[5] * (2*B[2][0]) + c0[7] * B[1][0] + c0[13] * B[0][0]  // S_uvv
                        + c1[0] * B[13][1] + c1[3] * B[7][1] + c1[1] * (2*B[5][1]) + c1[6] * (2*B[2][1]) + c1[4] * B[1][1] + c1[12] * B[0][1]
                        + c2[0] * B[13][2] + c2[2] * B[7][2] + c2[3] * (2*B[5][2]) + c2[8] * (2*B[2][2]) + c2[9] * B[1][2] + c2[18] * B[0][2];
                        //     c*B_uvw     +    C_w*B_uv     +    C_v*B_uw     +    C_u*B_vw     +   C_vw*B_u      +   C_uw*B_v      +   C_uv*B_w      +  C_uvw*B
           this->_p[14] = c0[0] * B[14][0] + c0[3] * B[5][0] + c0[2] * B[6][0] + c0[1] * B[8][0] + c0[8] * B[1][0] + c0[6] * B[2][0] + c0[5] * B[3][0] + c0[14] * B[0][0]  // S_uvw
                        + c1[0] * B[14][1] + c1[2] * B[5][1] + c1[1] * B[6][1] + c1[3] * B[8][1] + c1[5] * B[1][1] + c1[8] * B[2][1] + c1[6] * B[3][1] + c1[14] * B[0][1]
                        + c2[0] * B[14][2] + c2[1] * B[5][2] + c2[3] * B[6][2] + c2[2] * B[8][2] + c2[6] * B[1][2] + c2[5] * B[2][2] + c2[8] * B[3][2] + c2[14] * B[0][2];
                        //     c*B_uww     +    C_u*B_ww     +  2 C_w*B_uw         + 2 C_uw*B_w          +   C_ww*B_u      +  C_uww*B
           this->_p[15] = c0[0] * B[15][0] + c0[1] * B[9][0] + c0[3] * (2*B[6][0]) + c0[6] * (2*B[3][0]) + c0[9] * B[1][0] + c0[15] * B[0][0]  // S_uww
                        + c1[0] * B[15][1] + c1[3] * B[9][1] + c1[2] * (2*B[6][1]) + c1[8] * (2*B[3][1]) + c1[7] * B[1][1] + c1[17] * B[0][1]
                        + c2[0] * B[15][2] + c2[2] * B[9][2] + c2[1] * (2*B[6][2]) + c2[5] * (2*B[3][2]) + c2[4] * B[1][2] + c2[11] * B[0][2];
                        //     C*B_vvv     +  3 C_v*B_vv         + 3 C_vv*B_v          +   C_vvv*B
           this->_p[16] = c0[0] * B[16][0] + c0[2] * (3*B[7][0]) + c0[7] * (3*B[2][0]) + c0[16] * B[0][0]  // S_vvv
                        + c1[0] * B[16][1] + c1[1] * (3*B[7][1]) + c1[4] * (3*B[2][1]) + c1[10] * B[0][1]
                        + c2[0] * B[16][2] + c2[3] * (3*B[7][2]) + c2[9] * (3*B[2][2]) + c2[19] * B[0][2];
                        //     c*B_vvw     +    C_w*B_vv     +  2 C_v*B_vw         + 2 C_vw*B_v          +   C_vv*B_w      +  C_vvw*B
           this->_p[17] = c0[0] * B[17][0] + c0[3] * B[7][0] + c0[2] * (2*B[8][0]) + c0[8] * (2*B[2][0]) + c0[7] * B[3][0] + c0[17] * B[0][0]  // S_vvw
                        + c1[0] * B[17][1] + c1[2] * B[7][1] + c1[1] * (2*B[8][1]) + c1[5] * (2*B[2][1]) + c1[4] * B[3][1] + c1[11] * B[0][1]
                        + c2[0] * B[17][2] + c2[1] * B[7][2] + c2[3] * (2*B[8][2]) + c2[6] * (2*B[2][2]) + c2[9] * B[3][2] + c2[15] * B[0][2];
                        //     c*B_vww     +    C_v*B_ww     +   2 C_w*B_vw        + 2 C_vw*B_w          +   C_ww*B_v      +   C_vww*B
           this->_p[18] = c0[0] * B[18][0] + c0[2] * B[9][0] + c0[3] * (2*B[8][0]) + c0[8] * (2*B[3][0]) + c0[9] * B[2][0] + c0[18] * B[0][0]  // S_vww
                        + c1[0] * B[18][1] + c1[1] * B[9][1] + c1[2] * (2*B[8][1]) + c1[5] * (2*B[3][1]) + c1[7] * B[2][1] + c1[13] * B[0][1]
                        + c2[0] * B[18][2] + c2[3] * B[9][2] + c2[1] * (2*B[8][2]) + c2[6] * (2*B[3][2]) + c2[4] * B[2][2] + c2[12] * B[0][2];
                        //     C*B_www     +  3 C_w*B_ww         + 3 C_ww*B_w          +   C_www*B
           this->_p[19] = c0[0] * B[19][0] + c0[3] * (3*B[9][0]) + c0[9] * (3*B[3][0]) + c0[19] * B[0][0]  // S_www
                        + c1[0] * B[19][1] + c1[2] * (3*B[9][1]) + c1[7] * (3*B[3][1]) + c1[16] * B[0][1]
                        + c2[0] * B[19][2] + c2[1] * (3*B[9][2]) + c2[4] * (3*B[3][2]) + c2[10] * B[0][2];
        }
     }
  }

  template <typename T>
  inline
  DVector< PTriangle<T,3>* >& PERBSTriangle<T>::getLocalPatches() {

    return _c;
  }

  template <typename T>
  Vector<Point<T,3>,3> PERBSTriangle<T>::getPoints() const {

    Vector<Point<T,3>,3>  a;
    DVector<Vector<T,3> > m = this->evaluateGlobal(Point<T,3>(1,0,0) , 0);
    a[0] = m[0];
    m = this->evaluateGlobal(Point<T,3>(0,1,0) , 0);
    a[1] = m[0];
    m = this->evaluateGlobal(Point<T,3>(0,0,1) , 0);
    a[2] = m[0];

    return a;
  }

  template <typename T>
  inline
  void PERBSTriangle<T>::hideLocalPatches() {

    for( int i = 0; i < _c.getDim(); i++ )
      _c[i]->setVisible( false, -1 );
  }

  template <typename T>
  inline
  void  PERBSTriangle<T>::insertPatch( PTriangle<T,3> *patch ) {

    patch->replot( 10 );
    patch->setVisible( false );
    patch->setCollapsed( true );
    this->insert( patch );
  }

  template <typename T>
  inline
  bool PERBSTriangle<T>::isLocalPatchesVisible() const {

    for( int i = 0; i < _c.getDim(); i++ )
      if( _c(i)->isVisible() )
        return true;

    return false;
  }

  template <typename T>
  inline
  void PERBSTriangle<T>::resetBasis( BasisTriangleType t) {

    if( _B )    delete _B;
    _B = new BasisTriangleERBS<T>(t);
  }

  template <typename T>
  inline
  void PERBSTriangle<T>::showLocalPatches() {

    for( int i = 0; i < _c.getDim(); i++ )
      _c[i]->setVisible( true, -1 );
  }






} // END namespace GMlib

