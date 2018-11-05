/**********************************************************************************
**
** Copyright (C) 1994/2016 University of Tromsø - The Arctic University of Norway
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

template <typename T>               // Described on page 91-92 in "Blend book"
void EvaluatorStatic<T>::evaluateBhp( DMatrix<T>& mat, int d, T t, T scale ) {

    // Initiate result matrix
    mat.setDim( d+1, d+1 );

    // Escape if the degree is 0
    if( d < 1 ) {
        mat[0][0] = T(1);
        return;
    }

    // Compute the Bernstein (polynomials), degree 1 -> d, one for each row.
    // Starts from the second bottom row (degree 1), then goes upwards (degree 2,...,d).
    mat[d-1][0] = 1 - t;
    mat[d-1][1] = t;

    for( int i = d-2; i >= 0; i-- ) {
        mat[i][0] = ( 1 - t) * mat[i+1][0];
        for( int j = 1; j < d - i; j++ )
            mat[i][j] = t * mat[i+1][j-1] + (1 - t) * mat[i+1][j];
        mat[i][d-i] = t * mat[i+1][d-i-1];
    }

    // Compute all the deriatives
    mat[d][0] = -scale;
    mat[d][1] = scale;

    for( int k = 2; k <= d; k++ ) {
        const double s = k * scale;
        for( int i = d; i > d - k; i-- ) {
            mat[i][k] = s * mat[i][k-1];
            for( int j = k - 1; j > 0; j-- )
                mat[i][j] = s * ( mat[i][j-1] - mat[i][j] );
            mat[i][0] = - s * mat[i][0];
        }
    }
}



  template <typename T>
  inline
  int EvaluatorStatic<T>::evaluateBSp( DMatrix<T>& mat, T t, const DVector<T>& tv, int d, bool left, T scale ){

      // Find knot-index i such that: tv[i] <= t <  tv[i+1] if right-evaluation
      // else                         tv[i] <  t <= tv[i+1] if left-evaluation
      int i = knotIndex(tv, t, d, left);
      EvaluatorStatic<T>::evaluateBSp2( mat, t, tv, d, i, scale );
      return i;
  }


  template <typename T>
  void EvaluatorStatic<T>::evaluateBSp2( DMatrix<T>& mat, T t, const DVector<T>& tv, int d, int ii, T scale ){

      // Knot-index ii must be: tv[ii] <= t <  tv[ii+1] if right-evaluation
      // else                   tv[ii] <  t <= tv[ii+1] if left-evaluation

      // For the linar factor - mapping the knot intervalls to [0,1].
      DVector<T> w(d);

      // Compute the B-splines (polynomials), degree 1 -> d, one for each row.
      // Starts from the second bottom row (degree 1), then goes upwards (degree 2,...,d).
      mat.setDim( d+1, d+1 );

      mat[d-1][1] = getW( tv, t, ii, 1 );
      mat[d-1][0] = 1 - mat[d-1][1];

      for( int i = d - 2, k = 2; i >= 0; i--, k++ ) {
          // Generate w, expression 5.45 (page 119) in "Blend book".
          for( int j = 0; j < k; j++ )
              w[j] = getW( tv, t, ii-k+j+1, k );

          // Compute the k b-splines
          mat[i][0] = ( 1 - w[0]) * mat[i+1][0];
          for( int j = 1; j < d - i; j++ )
              mat[i][j] = w[j-1] * mat[i+1][j-1] + (1 - w[j]) * mat[i+1][j];
          mat[i][d-i] = w[k-1] * mat[i+1][d-i-1];
      }

      // Compute all deriatives for the derivatives B-splines (polynomials), 1st, 2nd,...,d-order derivatives.
      mat[d][1] =  delta( tv, ii, 1, scale );
      mat[d][0] = -mat[d][1];

      for( int k = 2; k <= d; k++ ) {
          // Generate w' (delta) for the derivatives, expression 5.47 (page 121) in "Blend book".
          for( int j = 0; j < k; j++ )
              w[j] = k * delta( tv, ii-k+j+1, k, scale);

          for( int i = d; i > d - k; i--) {
              mat[i][k] = w[k-1] * mat[i][k-1];
              for( int j = k - 1; j > 0; j--)
                  mat[i][j] = w[j-1]*mat[i][j-1] - w[j]*mat[i][j] ;
              mat[i][0] = - w[0] * mat[i][0];
          }
      }
  }


  template <typename T>
  void EvaluatorStatic<T>::evaluateH3d( DMatrix<T>& mat, int d, T t) {

      mat.setDim( d+1, 4 );

      T t2 = t*t;
      T t3 = t*t2;

      // Compute the four Hermite polynomials (f_1, f_2, f'_1, f'_2)
      // expression 4.16 (page 78) in "Blend book".
      mat[0][1] = 3*t2 - 2*t3;
      mat[0][0] = 1 - mat[0][1];
      mat[0][2] = t - 2*t2 + t3;
      mat[0][3] = t3 - t2;
      if(d>0) {  // First derivatives
          mat[1][1] =  6*(t - t2);
          mat[1][0] = -mat[1][1];
          mat[1][2] =  1 - 4*t + 3*t2;
          mat[1][3] =  3*t2 - 2*t;
          if(d>1) { // Second derivatives
              mat[2][1] =  6 - 12*t;
              mat[2][0] = -mat[2][1];
              mat[2][2] =  6*t - 4;
              mat[2][3] =  6*t - 2;
              if(d>2) { // Third derivatives
                  mat[3][1] = -12;
                  mat[3][0] =  12;
                  mat[3][2] =  6;
                  mat[3][3] =  6;
              }
          }
      }
  }



  template <typename T>
  void EvaluatorStatic<T>::evaluateH5d( DMatrix<T>& mat, int d, T t) {
       mat.setDim( d+1, 6 );

      T t2 = t*t;
      T t3 = t*t2;
      T t4 = t*t3;
      T t5 = t*t4;

      // Compute the six Hermite polynomials (f_1, f_2, f'_1, f'_2, f''_1, f''_2)
      // expression 4.23 (page 81) in "Blend book".
      mat[0][1] = 10*t3 - 15*t4 + 6*t5;
      mat[0][0] =  1 - mat[0][1];
      mat[0][2] =  t - 6*t3 + 8*t4 - 3*t5;
      mat[0][3] = -4*t3 + 7*t4 - 3*t5;
      mat[0][4] =  0.5*t2 - 1.5*t3 + 1.5*t4 - 0.5*t5;
      mat[0][5] =  0.5*t3 - t4 + 0.5*t5;
      if(d>0) {   // First derivatives
          mat[1][1] = 30*t2 - 60*t3 + 30*t4;
          mat[1][0] = -mat[1][1];
          mat[1][2] =  1 - 18*t2 + 32*t3 - 15*t4;
          mat[1][3] = -12*t2 + 28*t3 - 15*t4;
          mat[1][4] =  t - 4.5*t2 + 6*t3 - 2.5*t4;
          mat[1][5] =  1.5*t2 - 4*t3 + 2.5*t4;
          if(d>1) {  // Second derivatives
              mat[2][1] =  60*t - 180*t2 + 120*t3;
              mat[2][0] = -mat[2][1];
              mat[2][2] = -36*t + 96*t2 - 60*t3;
              mat[2][3] = -24*t + 84*t2 - 60*t3;
              mat[2][4] =  1 - 9*t + 18*t2 - 10*t3;
              mat[2][5] =  3*t - 12*t2 + 10*t3;
              if(d>2) {  // Third derivatives
                  mat[3][1] =  60 - 360*t + 360*t2;
                  mat[3][0] = -mat[3][1];
                  mat[3][2] = -36 + 192*t - 180*t2;
                  mat[3][3] = -24 + 168*t - 180*t2;
                  mat[3][4] = -9 + 36*t - 30*t2;
                  mat[3][5] =  3 - 24*t + 30*t2;
                  if(d>3) {  // Fourth derivatives
                      mat[4][1] = -360 + 720*t;
                      mat[4][0] = -mat[3][1];
                      mat[4][2] =  192 - 360*t;
                      mat[4][3] =  168 - 360*t;
                      mat[4][4] =  36 - 60*t;
                      mat[4][5] =  24 + 60*t;
                      if(d>4) {  // Fifth derivatives
                          mat[5][1] =  720;
                          mat[5][0] = -720;
                          mat[5][2] = -360;
                          mat[5][3] = -360;
                          mat[5][4] = -60;
                          mat[5][5] =  60;
                      }
                  }
              }
          }
      }
  }




  template <typename T>
  inline
  int EvaluatorStatic<T>::knotIndex(const DVector<T>& tv, T t, int d, bool left) {
      // Return knot-index i such that: tv[i] <= t <  tv[i+1] if right-evaluation
      // else                           tv[i] <  t <= tv[i+1] if left-evaluation
      int i = d;
      int j = tv.getDim()-d-1;  // = n
      if(left){
          do{
              int k=(i+j)/2;
              if(t > tv(k)) i=k;
              else          j=k;
          }while(j-i>1);
      }
      else {
          do{
              int k=(i+j)/2;
              if(t < tv(k)) j=k;
              else          i=k;
          }while(j-i>1);
      }
      return i;
  }





  template <typename T>
  inline
  T EvaluatorStatic<T>::knotInterval( const DVector<T>& t, int d, int n ) {
      // Return average knot-interval, that is average for intervalls different from zero
      int i = 0;
      for(int j=d; j<n; j++)
          if(t[j+1] > t[j]) i++;
      return (t[n]-t[d])/T(i);
  }






  template <typename T>
  inline                    // expression 5.45 (page 119) in "Blend book".
  T EvaluatorStatic<T>::getW( const DVector<T>& tv, T t, int i, int d ) {
      return (t-tv(i))/(tv(i+d)-tv(i));
  }


  template <typename T>
  inline                    // expression 5.47 (page 121) in "Blend book".
  T EvaluatorStatic<T>::delta( const DVector<T>& tv, int i, int d, T s ) {
      return s/(tv(i+d)-tv(i));
  }

} // END namespace GMlib
