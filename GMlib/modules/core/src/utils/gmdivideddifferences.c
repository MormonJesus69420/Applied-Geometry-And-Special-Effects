
// include for syntaxhighlighting in editors
#include "gmdivideddifferences.h"

// gmlib
#include "gmutils.h"
#include "../containers/gmdvector.h"
#include "../containers/gmdmatrix.h"

// stl
#include <cassert>

namespace GMlib {
  namespace DD {
    namespace Private {




      // ln : loop
      // en : end/execute
      // n  : dim






      template <int en, int n>
      class Static_Compute_ {
      public:

        template <typename G>
        static void compute( G& assign, G& prev, G& next, const Vector<double,n>& dt, int di ) {

          assign = (next - prev) / dt(di-1);

        }

      };





      template <int en, int n>
      class Static_Compute_Boundary_Open_2nd_ {
      public:
        template <typename G>
        static void compute( G& assign, G& curr , G& next, G& next2, const Vector<double,n>& dt, int di, bool l ) {

          const int s = l ? 1 : -1;
          assign = (s*4*next - s*3*curr - s*next2) / dt(di-1);

        }

      };






      template <int ln, int en, int n>
      class Static_For_Der_ {
      public:

        template <typename G>
        static void compute( G& assign, G& prev , G& next, const Vector<double,n>& dt, const Vector<int,n>& d, const Vector<int,n>& ed, int di) {

          if( ln > en )
            Static_For_Der_<ln+1,en,n>::compute(   assign[0],  prev[0],    next[0],    dt,d,ed,di );
          else if( ln == en )
            Static_For_Der_<ln+1,en,n>::compute(   assign[di], prev[di-1], next[di-1], dt,d,ed,di );
          else
            for( int i = ed(ln-1); i <= ed(ln-1)+d(ln-1); ++i )
              Static_For_Der_<ln+1,en,n>::compute( assign[i],  prev[i],    next[i],    dt,d,ed,di );
        }
      };

      template <int en, int n>
      class Static_For_Der_<n,en,n> {
      public:

        template <typename G>
        static void compute( G& assign, G& prev , G& next, const Vector<double,n>& dt, const Vector<int,n>& d, const Vector<int,n>& ed, int di ) {

          if( n > en )
            Static_Compute_<en,n>::compute(   assign[0],  prev[0],    next[0],    dt,di );
          else if ( n == en )
            Static_Compute_<en,n>::compute(   assign[di], prev[di-1], next[di-1], dt,di );
          else
            for( int i = ed(n-1); i <= ed(n-1)+d(n-1); ++i )
              Static_Compute_<en,n>::compute( assign[i],  prev[i],    next[i],    dt,di );
        }
      };








      template <int ln, int en, int n>
      class Static_For_Data_ {
      public:

        template <typename G>
        static void compute( G& assign, G& prev , G& next, const Vector<int,n>& k, const Vector<double,n>& dt, const Vector<int,n>& d, const Vector<int,n>& ed, int di ) {

          const int s = ( ln==en ? 1       : 0 );
          const int e = ( ln==en ? k(ln-1) : k(ln-1)+1 );

          const int o = ( ln==en ? 1 : 0 );
          for( int i = s; i < e; ++i )
            Static_For_Data_<ln+1,en,n>::compute( assign[i], prev[i-o],   next[i+o],   k, dt, d, ed, di );

        }
      };

      template <int en, int n>
      class Static_For_Data_<n,en,n> {
      public:

        template <typename G>
        static void compute( G& assign, G& prev , G& next, const Vector<int,n>& k, const Vector<double,n>& dt, const Vector<int,n>& d, const Vector<int,n>& ed, int di ) {

          const int s = ( n==en ? 1      : 0 );
          const int e = ( n==en ? k(n-1) : k(n-1)+1 );

          const int o = ( n==en ? 1 : 0 );
          for( int i = s; i < e; ++i )
            Static_For_Der_<1,en,n>::compute( assign[i], prev[i-o],   next[i+o], dt, d, ed, di );

        }
      };



      template <int ln, int en, int n>
      class Static_For_Data_Boundary_Closed_ {
      public:

        template <typename G>
        static void compute( G& assign, G& prev , G& next, const Vector<int,n>& k, const Vector<double,n>& dt, const Vector<int,n>& d, const Vector<int,n>& ed, int di ) {

          if( ln == en ) {
            Static_For_Data_Boundary_Closed_<ln+1,en,n>::compute( assign[0],       prev[k(ln-1)-1], next[1], k,dt,d,ed,di );
            Static_For_Data_Boundary_Closed_<ln+1,en,n>::compute( assign[k(ln-1)], prev[k(ln-1)-1], next[1], k,dt,d,ed,di );
          }
          else
            for( int i = 0; i < k(ln-1)+1; ++i )
              Static_For_Data_Boundary_Closed_<ln+1,en,n>::compute( assign[i], prev[i],   next[i], k,dt,d,ed,di);

        }
      };

      template <int en, int n>
      class Static_For_Data_Boundary_Closed_<n,en,n> {
      public:

        template <typename G>
        static void compute( G& assign, G& prev , G& next, const Vector<int,n>& k, const Vector<double,n>& dt, const Vector<int,n>& d, const Vector<int,n>& ed, int di ) {

          if( n == en ) {
            Static_For_Der_<1,en,n>::compute( assign[0],      prev[k(n-1)-1], next[1], dt,d,ed,di );
            Static_For_Der_<1,en,n>::compute( assign[k(n-1)], prev[k(n-1)-1], next[1], dt,d,ed,di );
          }
          else
            for( int i = 0; i < k(n-1)+1; ++i )
              Static_For_Der_<1,en,n>::compute( assign[i], prev[i],   next[i], dt,d,ed,di);

        }
      };

















      template <int ln, int en, int n>
      class Static_For_Der_Boundary_Open_2nd_ {
      public:

        template <typename G>
        static void compute( G& assign, G& curr , G& next, G& next2, const Vector<double,n>& dt, const Vector<int,n>& d, const Vector<int,n>& ed, int di, bool l ) {

          if( ln > en )
            Static_For_Der_Boundary_Open_2nd_<ln+1,en,n>::compute(   assign[0],  curr[0],    next[0],    next2[0],    dt,d,ed,di,l );
          else if( ln == en )
            Static_For_Der_Boundary_Open_2nd_<ln+1,en,n>::compute(   assign[di], curr[di-1], next[di-1], next2[di-1], dt,d,ed,di,l );
          else
            for( int i = ed(ln-1); i < ed(ln-1)+d(ln-1); ++i )
              Static_For_Der_Boundary_Open_2nd_<ln+1,en,n>::compute( assign[i],  curr[i],    next[i],    next2[i],    dt,d,ed,di,l );

        }
      };

      template <int en, int n>
      class Static_For_Der_Boundary_Open_2nd_<n,en,n> {
      public:

        template <typename G>
        static void compute( G& assign, G& curr , G& next, G& next2, const Vector<double,n>& dt, const Vector<int,n>& d, const Vector<int,n>& ed, int di, bool l ) {

          if( n > en )
            Static_Compute_Boundary_Open_2nd_<en,n>::compute(   assign[0],  curr[0],    next[0],    next2[0],    dt,di,l );
          else if ( n == en )
            Static_Compute_Boundary_Open_2nd_<en,n>::compute(   assign[di], curr[di-1], next[di-1], next2[di-1], dt,di,l );
          else
            for( int i = ed(n-1); i < ed(n-1)+d(n-1); ++i )
              Static_Compute_Boundary_Open_2nd_<en,n>::compute( assign[i],  curr[i],    next[i],    next2[i],    dt,di,l );

        }
      };









      template <int ln, int en, int n>
      class Static_For_Data_Boundary_Open_ {
      public:

        template <typename G>
        static void compute( G& assign, G& curr , G& next, G& next2, const Vector<int,n>& k, const Vector<double,n>& dt, const Vector<int,n>& d, const Vector<int,n>& ed, int di, bool l = true ) {

          if( ln == en ) {
            Static_For_Data_Boundary_Open_<ln+1,en,n>::compute(   assign[0],       curr[0],       next[1],         next2[2],         k,dt,d,ed,di, true  );  // left
            Static_For_Data_Boundary_Open_<ln+1,en,n>::compute(   assign[k(ln-1)], curr[k(ln-1)], next[k(ln-1)-1], next2[k(ln-1)-2], k,dt,d,ed,di, false ); // right
          }
          else
            for( int i = 0; i < k(ln-1)+1; ++i )
              Static_For_Data_Boundary_Open_<ln+1,en,n>::compute( assign[i],       curr[i],       next[i],         next2[i],         k,dt,d,ed,l );

        }
      };

      template <int en, int n>
      class Static_For_Data_Boundary_Open_<n,en,n> {
      public:

        template <typename G>
        static void compute( G& assign, G& curr , G& next, G& next2, const Vector<int,n>& k, const Vector<double,n>& dt, const Vector<int,n>& d, const Vector<int,n>& ed, int di, bool l = true ) {

          if( n == en ) {
            Static_For_Der_Boundary_Open_2nd_<1,en,n>::compute(   assign[0],      curr[0],      next[1],        next2[2],        dt,d,ed,di, true );  // left
            Static_For_Der_Boundary_Open_2nd_<1,en,n>::compute(   assign[k(n-1)], curr[k(n-1)], next[k(n-1)-1], next2[k(n-1)-2], dt,d,ed,di, false ); // right
          }
          else
            for( int i = 0; i < k(n-1)+1; ++i )
              Static_For_Der_Boundary_Open_2nd_<1,en,n>::compute( assign[i],      curr[i],      next[i],        next2[i],        dt,d,ed,di,l );
        }
      };


















      template <int ln, int n>
      class Static_For_Dim_ {
      public:

        template <typename T>
        static void compute( T& p, const Vector<int,n>& k, const Vector<double,n>& dt, const Vector<bool,n>& closed, const Vector<int,n>& d, const Vector<int,n>& ed ) {

          for( int i = 1+ed(ln-1); i <= ed(ln-1)+d(ln-1); ++i ) {
            Static_For_Data_<1,ln,n>::compute( p, p, p, k, dt, d, ed, i );

            if( closed(ln-1) )
              Static_For_Data_Boundary_Closed_<1,ln,n>::compute( p, p, p, k, dt, d, ed, i );
            else
              Static_For_Data_Boundary_Open_<1,ln,n>::compute( p, p, p, p, k, dt, d, ed, i )
              ;
          }

          Static_For_Dim_<ln+1,n>::compute( p, k, dt, closed, d, ed );

        }
      };





      template <int n>
      class Static_For_Dim_<n,n> {
      public:

        template <typename T>
        static void compute( T& p, const Vector<int,n>& k, const Vector<double,n>& dt, const Vector<bool,n>& closed, const Vector<int,n>& d, const Vector<int,n>& ed ) {


          for( int i = 1+ed(n-1); i <= ed(n-1)+d(n-1); ++i ) {
            Static_For_Data_<1,n,n>::compute( p, p, p, k, dt, d, ed, i );

            if( closed(n-1) )
              Static_For_Data_Boundary_Closed_<1,n,n>::compute( p, p, p, k, dt, d, ed, i );
            else
              Static_For_Data_Boundary_Open_<1,n,n>::compute( p, p, p, p, k, dt, d, ed, i )
              ;
          }

        }
      };





      template< typename G>
      inline
      void computeHelp2( std::vector<double[3]>& b, const G& t, bool closed, int k ) {

        int s = k-1;

        std::vector<double[2]> a(k);
        for(int i=0; i<k; i++) {
            a[i][0] = t[i+1]-t[i];
            a[i][1] = a[i][0]*a[i][0];
        }

        if(closed) {
            double d = a[s][1]*a[0][0] + a[s][0]*a[0][1];
            b[0][0]  = b[k][0] = -a[0][1]/d;
            b[0][1]  = b[k][1] = (a[0][1]-a[s][1])/d;
            b[0][2]  = b[k][2] = a[s][1]/d;
        }else {
            double d = a[0][1]*a[1][0] + a[0][0]*a[1][1];
            b[0][0]  = -a[1][0]*(2*a[0][0]+a[1][0])/d;
            b[0][1]  = (a[0][0]+a[1][0])*(a[0][0]+a[1][0])/d;
            b[0][2]  = -a[0][1]/d;
            d = a[s-1][1]*a[s][0] + a[s-1][0]*a[s][1];
            b[k][0]  =  a[s][1]/d;
            b[k][1]  = -(a[s-1][0]+a[s][0])*(a[s-1][0]+a[s][0])/d;
            b[k][2]  =  a[s-1][0]*(2*a[s][0]+a[s-1][0])/d;
        }
        for(int i=1; i<k; i++) {
            double d = a[i-1][1]*a[i][0] + a[i-1][0]*a[i][1];
            b[i][0]  = -a[i][1]/d;
            b[i][1]  = (a[i][1]-a[i-1][1])/d;
            b[i][2]  = a[i-1][1]/d;
        }
      }

    }


    /***********************************************************************************************/





    template <typename T>
    inline
    double relationCK(const T& p1, const T& p2, const T& p3) {
      T v0 = p3-p1;
      T v1 = p2-p1;
      T v2 = p3-p2;

      double a1 = v1*v1;
      double a2 = v2*v2;
      double a3 = v1*v2;
      double a4 = a3/a1;
      double a5 = a2-a3*a4;

      if(a5 < 1e-5) return 1;

      double a6 = v0*v2;
      return acos(a3/sqrt(a1*a2))*sqrt((a1+a6*a6/a5)/(v0*v0));
    }





    template< typename T>
    inline
    void compute1D( T& p, double dt, bool closed, int d, int ed ) {

      assert( ed >= 0 );

      double dt2 = 2*dt;
      int k = p.size()-1;

      for(int i = 1+ed; i <= ed+d; i++) {
        int i1 = i-1;

        for(int l = 1; l < k; l++) // ordinary diviedd differences
          p[l][i] = (p[l+1][i1] - p[l-1][i1])/dt2;

        if(closed)  // biting its own tail
          p[k][i] = p[0][i] = (p[1][i1] - p[k-1][i1])/dt2;
        else {      // second degree endpoints diviedd differences
          p[0][i] = ( 4*p[1][i1]   - 3*p[0][i1] - p[2][i1]  )/dt2;
          p[k][i] = (-4*p[k-1][i1] + 3*p[k][i1] + p[k-2][i1])/dt2;
        }
      }
    }






    template< typename T, typename G>
    inline
    void compute1D( T& p, const G& t, bool closed, int d, int ed ) {

      assert( ed >= 0 );

      int k = t.size()-1;
      std::vector<double[3]> b(k+1);
      Private::computeHelp2<G>( b, t, closed, k );

      for(int i = 1+ed; i <= d; i++) {
        int i1 = i-1;

        for(int l = 1; l < k; l++)// ordinary diviedd differences
          p[l][i] = b[l][0]*p[l-1][i1] + b[l][1]*p[l][i1]  + b[l][2]*p[l+1][i1];

        if(closed)                // biting its own tail
          p[0][i] = p[k][i] = b[0][0]*p[k-1][i1] + b[0][1]*p[0][i1]  + b[0][2]*p[1][i1];
        else {                    // second degree endpoints divided differences
          p[0][i] = b[0][0]*p[0][i1]   + b[0][1]*p[1][i1]   + b[0][2]*p[2][i1];
          p[k][i] = b[k][0]*p[k-2][i1] + b[k][1]*p[k-1][i1] + b[k][2]*p[k][i1];
        }
      }
    }





    template <typename T>
    inline
    void compute2D( T& p, double du, double dv, bool closed_u, bool closed_v,
                    int d1, int d2, int ed1, int ed2 ) {

      assert( ed1 >= 0 );
      assert( ed2 >= 0 );


      double du2 = 2*du;
      double dv2 = 2*dv;
      int ku = p.getDim1()-1;
      int kv = p.getDim2()-1;


      // Compute U derivatives

      for(int i = 1+ed1; i <= ed1+d1; ++i) { // edr in u
        // j = 0

        int i1 = i-1;

        // ordinary divided differences
        for(int k = 1; k < ku; ++k)       // data points u
          for(int l = 0; l < kv+1; ++l) { // data points v
            double scale = relationCK(p(k-1)(l)(0)(0), p(k)(l)(0)(0), p(k+1)(l)(0)(0));
            p[k][l][i][0] = scale * (p[k+1][l][i1][0] - p[k-1][l][i1][0]) / ( du2);
          }

        if(closed_u) { // biting its own tail
          for(int l = 0; l < kv+1; ++l) { // data points u
            double scale = relationCK(p(ku-1)(l)(0)(0), p(0)(l)(0)(0), p(1)(l)(0)(0));
            p[0 ][l][i][0] = scale * (p[1][l][i1][0] - p[ku-1][l][i1][0]) / du2;
            p[ku][l][i][0] = p[0][l][i][0];
          }
        }
        else { // second degree endpoints divided differences
          for(int l = 0; l < kv+1; ++l) { // data points u
            double scale = relationCK(p(0)(l)(0)(0), p(1)(l)(0)(0), p(2)(l)(0)(0));
            p[0 ][l][i][0] = scale * ( 4*p[1   ][l][i1][0] - 3*p[0 ][l][i1][0] - p[2   ][l][i1][0] ) / du2;
            scale = relationCK(p(ku-2)(l)(0)(0), p(ku-1)(l)(0)(0), p(ku)(l)(0)(0));
            p[ku][l][i][0] = scale * (-4*p[ku-1][l][i1][0] + 3*p[ku][l][i1][0] + p[ku-2][l][i1][0] ) / du2;
          }
        }
      }


      // Compute ALL V derivatives

      for( int i = 0; i <= ed1+d1; ++i ) {
        for(int j = 1+ed2; j <= ed2+d2; ++j) { // edr in u

          int j1 = j-1;

          // ordinary divided differences
          for(int k = 0; k < ku+1; ++k)   // data points u
            for(int l = 1; l < kv; ++l) {  // data points v
              double scale = relationCK(p(k)(l-1)(0)(0), p(k)(l)(0)(0), p(k)(l+1)(0)(0) );
              p[k][l][i][j] = scale * (p[k][l+1][i][j1] - p[k][l-1][i][j1]) / (  dv2 );
            }

          if(closed_v) { // biting its own tail

            for(int k = 0; k < ku+1; ++k) { // data points v
              double scale = relationCK(p(k)(kv-1)(0)(0), p(k)(0)(0)(0), p(k)(1)(0)(0) );
              p[k][0 ][i][j] = scale * (p[k][1][i][j1] - p[k][kv-1][i][j1]) / dv2;
              p[k][kv][i][j] = p[k][0][i][j];
            }
          }
          else { // second degree endpoints divided differences

            for(int k = 0; k < ku+1; ++k) { // data points v
              double scale = relationCK(p(k)(0)(0)(0), p(k)(1)(0)(0), p(k)(2)(0)(0) );
              p[k][0 ][i][j] = scale * ( 4*p[k][1   ][i][j1] - 3*p[k][0 ][i][j1] - p[k][2   ][i][j1] ) / dv2;
              scale = relationCK(p(k)(kv-2)(0)(0), p(k)(kv-1)(0)(0), p(k)(kv)(0)(0) );
              p[k][kv][i][j] = scale * (-4*p[k][kv-1][i][j1] + 3*p[k][kv][i][j1] + p[k][kv-2][i][j1] ) / dv2;
            }
          }
        }
      }
    }






    template <typename T, int n>
    void compute( T& p, const Vector<int,n>& sizes, const Vector<double,n>& dt, const Vector<bool,n>& closed, const Vector<int,n>& d, const Vector<int,n>& ed ) {

      const Vector<int,n> k = sizes - Vector<int,n>(1);
      const Vector<double,n> dt2 = dt * 2;

      Private::Static_For_Dim_<1,n>::compute(p,k,dt2,closed,d,ed);
    }


  } // end namespace DD
}   // end namespace GMlib
