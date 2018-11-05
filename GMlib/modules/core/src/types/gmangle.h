/**********************************************************************************
**
** Copyright (C) 1994 - 2017 University of Tromsø - The Arctic University of Norway
** Contact: GMlib Online Portal at https://source.uit.no/gmlib/gmlib/wikis/home
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




#ifndef GM_CORE_TYPES_ANGLE_H
#define GM_CORE_TYPES_ANGLE_H


// stl
#include <cmath>



namespace GMlib {


/*! \def  POS_TOLERANCE
 *  Tolerance of the position
 */
#ifndef POS_TOLERANCE
#define POS_TOLERANCE     1e-6
#endif


/*! \def  ANGLE_TOLERANCE
 *  Tolerance of the angle
 */
#ifndef ANGLE_TOLERANCE
#define ANGLE_TOLERANCE		1e-3
#endif


/*! \def  M_E
 *  Value of the exponent : 2.71828182845904523536
 */
#ifndef M_E
#define M_E               2.71828182845904523536
#endif


/*! \def  M_LOG2E
 *  Binary logarithm of the exponent : 1.44269504088896340736
 */
#ifndef M_LOG2E
#define M_LOG2E           1.44269504088896340736
#endif


/*! \def  M_LOG10E
 *  Decimal logarithm of the exponent : 0.434294481903251827651
 */
#ifndef M_LOG10E
#define M_LOG10E          0.434294481903251827651
#endif


/*! \def  M_LN2
 *  : 0.693147180559945309417
 */
#ifndef M_LN2
#define M_LN2             0.693147180559945309417
#endif


/*! \def  M_LN10
 *  : 2.30258509299404568402
 */
#ifndef M_LN10
#define M_LN10            2.30258509299404568402
#endif


/*! \def  M_PI
 *  Value of Pi : 3.14159265358979323846
 */
#ifndef M_PI
#define M_PI              3.14159265358979323846
#endif


/*! \def  M_2PI
 *  Value of 2Pi : 6.28318530717958647692
 */
#ifndef M_2PI
#define M_2PI             6.28318530717958647692
#endif


/*! \def  M_2PI_3
 *  Value of 2Pi / 3 : 2.094395102393195492308
 */
#ifndef M_2PI_3
#define M_2PI_3           2.094395102393195492308
#endif


/*! \def  M_PI_2
 *  Value of Pi / 2 : 1.57079632679489661923
 */
#ifndef M_PI_2
#define M_PI_2            1.57079632679489661923
#endif

/*! \def  M_PI_3
 *  Value of Pi / 3 : 1.0471975511965977461542
 */
#ifndef M_PI_3
#define M_PI_3            1.0471975511965977461542
#endif

/*! \def  M_PI_4
 *  Value of Pi / 4 : 0.785398163397448309616
 */
#ifndef M_PI_4
#define M_PI_4            0.785398163397448309616
#endif


/*! \def  M_PI_180
 *  Value of Pi / 180 : 0.0174532925199432957692
 */
#ifndef M_PI_180
#define M_PI_180	        0.0174532925199432957692
#endif


/*! \def  M_1_PI
 *  Value of 1 / Pi : 0.318309886183790671538
 */
#ifndef M_1_PI
#define M_1_PI            0.318309886183790671538
#endif


/*! \def  M_2_PI
 *  Value of 2 / Pi : 0.636619772367581343076
 */
#ifndef M_2_PI
#define M_2_PI            0.636619772367581343076
#endif


/*! \def  M_180_PI
 *  Value of 180 / Pi : 57.29577951308232087679
 */
#ifndef M_180_PI
#define M_180_PI	        57.29577951308232087679
#endif


/*! \def  M_1_SQRTPI
 *  Value of 1 / Squart(Pi) : 0.564189583547756286948
 */
#ifndef M_1_SQRTPI
#define M_1_SQRTPI        0.564189583547756286948
#endif


/*! \def  M_2_SQRTPI
 *  Value of 2 / Suqart(Pi) : 1.12837916709551257390
 */
#ifndef M_2_SQRTPI
#define M_2_SQRTPI        1.12837916709551257390
#endif


/*! \def  M_SQRT2
 *  Value of Squart(2) : 1.41421356237309504880
 */
#ifndef M_SQRT2
#define M_SQRT2           1.41421356237309504880
#endif


/*! \def  M_SQRT1_2
 *  Value of Squart(1/2) : 0.707106781186547524401
 *  Remember that: Squart(1/2) = 1/Squart(2) = Squart(2)/2
 */
#ifndef M_SQRT1_2
#define M_SQRT1_2         0.707106781186547524401
#endif


/*! \def  M_SQRT3
 *  Value of Squart(3) : 1.73205080756887729353
 */
#ifndef M_SQRT3
#define M_SQRT3         1.73205080756887729353
#endif


/*! \def  M_SQRT1_3
 *  Value of Squart(1/3) : 0.57735026918962576451
 *  Remember that: Squart(1/3) = 1/Squart(3) = Squart(3)/3
 */
#ifndef M_SQRT1_3
#define M_SQRT1_3         0.57735026918962576451
#endif


/*! \def  NULL
 *  Value of NULL : 0
 */
#ifndef NULL
#define NULL              0
#endif


  /*! \class  Angle gmangle.h <gmAngle>
   *  \brief  Pending Documentation
   */
  class Angle {
  public:
    Angle( double r = 0.0 );
    Angle( int d );
    Angle( const Angle &a );

    int       getDeg() const;
    double    getRad() const;
    double    translate( double b = 0.0 );

    Angle&    operator =  ( double r );
    Angle&    operator =  ( int d );
    Angle&    operator =  ( Angle a );

    // Additions and substractions
    Angle&    operator += ( const Angle& a );
    Angle&    operator -= ( const Angle& a );
    Angle     operator -  () const;
    Angle     operator +  ( Angle a ) const;
    Angle     operator -  ( Angle a ) const;
    Angle     operator +  ( double r ) const;
    Angle     operator -  ( double r ) const;
    Angle     operator +  ( int d ) const;
    Angle     operator -  ( int d ) const;

    // Scaling
    Angle&    operator *= ( double r );
    Angle     operator *  ( double r ) const;

    // Invers scaling
    Angle&    operator /= ( double r );
    Angle     operator /  ( double r ) const;

    // Boolean on equality
    bool      operator == ( const Angle &a ) const;
    bool      operator != ( const Angle &a ) const;

    // Booleand on sorting
    bool      operator <  ( const Angle &a ) const;
    bool      operator >  ( const Angle &a ) const;
    bool      operator <= ( const Angle &a ) const;
    bool      operator >= ( const Angle &a ) const;


              operator double ();
              operator int ();


    /*! Angle Angle::operator * ( double r, Angle a )
     *  \brief  Multiply Angel object with the new angle (Radian)
     *
     *  Multiply Angle object with the new angle in radian.
     *
     *  \param  r - Value of the angle in radian
     *  \param  a - Angle object
     *  \return Copy of the Angle object that is scaled by r.
     */
    friend
    Angle     operator * ( double r, Angle a ) {
      a *= r;
      return a;
    }


  private:
    double      _angle;

  }; // END class Angle


  /*! Angle::Angle( double r = 0.0 )
   *  \brief  Default Constructor (Radian)
   *
   *  Default initialize angle to 0.0.
   *
   *  \param  r - Value of the angle in radian
   */
  inline
  Angle::Angle( double r ) {

    _angle = r;
  }


  /*! Angle::Angle( int d )
   *  \brief  Default Constructor (Degrees)
   *
   *  Initialize incomming degrees as radian. Conversion between degrees
   *  and radian is defined as Radian = Degrees * PI / 180.
   *
   *  \param  d - Value of the angle in degrees
   */
  inline
  Angle::Angle( int d ) {

    _angle = d * M_PI_180;
  }


  /*! Angle::Angle( const Angle &a )
   *  \brief  Default Copy Constructor
   *
   *  Copy constructor.
   *
   *  \param  a - Angle object
   */
  inline
  Angle::Angle( const Angle &a ) {

    _angle = a._angle;
  }


  /*! int Angle::getDegree() const
   *  \brief  Return the angle in degrees (integer)
   *
   *  Return the angle in degrees. Conversion between radian and degrees
   *  is defined as Degrees = Radian * 180 / PI.
   *
   *  \return The angle in degrees (int)
   */
  inline
  int Angle::getDeg() const {

    return int(_angle * M_180_PI);
  }


  /*! double Angle::getRadian() const
   *  \brief  Return the angle radian (double precision floating point)
   *
   *  Return the angle in radian. That is double precision floating point.
   *
   *  \return The angle in radian (double)
   */
  inline
  double Angle::getRad() const {

    return _angle;
  }


  /*! double Angle::translate( double b = 0.0 )
   *  \brief  Translate periodic
   *
   *  This function translate current angle to a choosen period
   *  and also ensure that the angle is inside the choosen period.
   *  Default period is [0, 2Pi].
   *
   *  \param  b Lower boundary of the choosen period (2 PI)
   *  \return The angle of the new period
   */
  inline
  double Angle::translate( double b ) {

    double tmp_angle = _angle;

    // If current angle (tmpAngle) is lower than LowerBondary (b),
    // then translate current angle forward with 2Pi until it
    // is higher than the bondary
    while( tmp_angle < b ) { tmp_angle += M_2PI; }

    // Change to UpperBondary
    b += M_2PI;

    // If current angle (tmpAngle) is higher than UpperBondary (b),
    // then translate current angle backward with 2Pi until it
    // is lower than the bondary
    while( tmp_angle > b ) { tmp_angle -= M_2PI; }

    return tmp_angle;
  }


  /*! Angle& Angle::operator = ( double r )
   *  \brief  Initialize current angle (Radian)
   *
   *  Initialize current the angle.
   *
   *  \param  r Value of the angle in radian
   *  \return The reference of the Angle object
   */
  inline
  Angle& Angle::operator = ( double r ) {

    _angle = r;
    return (*this);
  }


  /*! Angle& Angle::operator = ( int d )
   *  \brief  Initialize current angle (Degrees)
   *
   *  Convert the degrees value into radian and
   *  then initialize the angle.
   *
   *  \param  d Value of the angle in degrees
   *  \return The reference of the Angle object
   */
  inline
  Angle& Angle::operator = ( int d ) {

    _angle = d * M_PI_180;
    return (*this);
  }


  /*! Angle& Angle::operator = ( Angle a )
   *  \brief  Initialize current angle
   *
   *  Initialize current angle to incomming Angle.
   *
   *  \param  a Angle object
   *  \return The reference of the Angle object
   */
  inline
  Angle& Angle::operator = ( Angle a ) {

    _angle = a._angle;
    return (*this);
  }


  /*! Angle& Angle::operator += ( const Angle &a)
   *  \brief  Add to current angle
   *
   *  Add to current angle.
   *
   *  \param a Angle object
   *  \return The reference of the Angle object
   */
  inline
  Angle& Angle::operator += ( const Angle &a) {

    _angle += a._angle;
    return (*this);
  }


  /*! Angle& Angle::operator -= ( const Angle &a )
   *  \brief  Substract current angle from the new angle
   *
   *  Substract current angle from the new angle.
   *
   *  \param  a Angle object
   *  \return The reference of the Angle object
   */
  inline
  Angle& Angle::operator -= ( const Angle &a ) {

    _angle -= a._angle;
    return (*this);
  }


  /*! Angle Angle::operator - () const
   *  \brief  Return negative of current angle
   *
   *  Return the negative of current angle.
   *
   *  \return Copy of the Angle object
   */
  inline
  Angle Angle::operator - () const {

    return Angle(-_angle);
  }


  /*! Angle Angle::operator + ( Angle a ) const
   *  \brief  Add Angle object with current angle
   *
   *  Add Angle object with current angle.
   *
   *  \param  a Angle object
   *  \return Copy of the Angle object
   */
  inline
  Angle Angle::operator + ( Angle a ) const {

    a += (*this);
    return a;
  }


  /*! Angle Angle::operator - ( Angle a ) const
   *  \brief  Substract Angle object from current angle
   *
   *  Substract Angle object from current angle.
   *
   *  \param  a Angle object
   *  \return Copy of the Angle object
   */
  inline
  Angle Angle::operator - ( Angle a ) const {

    a -= (*this);
    return -a;
  }


  /*! Angle Angle::operator + ( double r ) const
   *  \brief  Add to current angle (Radian)
   *
   *  Add to current angle in radian.
   *
   *  \param  r Value of the angle in radian
   *  \return Copy of the Angle object
   */
  inline
  Angle Angle::operator + ( double r ) const {

    return (*this) + Angle(r);
  }


  /*! Angle Angle::operator - ( double r ) const
   *  \brief  Substract current angle from the new angle (Radian)
   *
   *  Substract current angle from the new angle in radian.
   *
   *  \param  r Value of the angle in radian
   *  \return Copy of the Angle object
   */
  inline
  Angle Angle::operator - ( double r ) const {

    return (*this) - Angle(r);
  }


  /*! Angle Angle::operator + ( int d ) const
   *  \brief  Add to current angle (Degrees)
   *
   *  Add to current angle in degrees.
   *
   *  \param  d Value of the angle in degrees
   *  \return Copy of the Angle object
   */
  inline
  Angle Angle::operator + ( int d ) const {

    return (*this) + Angle(d);
  }


  /*! Angle Angle::operator - ( int d ) const
   *  \brief  Substract current angle from the new angle (Degrees)
   *
   *  Substract current angle from the new angle in degrees.
   *
   *  \param  d Value of the angle in degrees
   *  \return Copy of the Angle object
   */
  inline
  Angle Angle::operator - ( int d ) const {

    return (*this) - Angle(d);
  }


  /*! Angle& Angle::operator *= ( double r )
   *  \brief  Multiply current angle with the new angle (Radian)
   *
   *  Multiply current angle with the new angle in radian.
   *
   *  \param  r Value of the anlge in radian
   *  \return The reference of the Angle object
   */
  inline
  Angle& Angle::operator *= ( double r ) {

    _angle *= r;
    return (*this);
  }


  /*! Angle Angle::operator * ( double r ) const
   *  \brief  Multiply current angle with the new angle (Radian)
   *
   *  Multiply current angle with the new angle in radian.
   *
   *  \param  r Value of the angle in radian
   *  \return The reference of the Angle object
   */
  inline
  Angle Angle::operator * ( double r ) const {

    return Angle(_angle * r);
  }


  /*! Angle& Angle::operator /= ( double r )
   *  \brief  Divide current angle with the new angle (Radian)
   *
   *  Divide current angle with the new angle in radian.
   *
   *  \param  r Value of the angle in radian
   *  \return The reference of the Angle object
   */
  inline
  Angle& Angle::operator /= ( double r ) {

    r = 1 / r;
    (*this) *= r;
    return (*this);
  }


  /*! Angle Angle::operator / ( double r ) const
   *  \brief  Divide current angle with the new angle (Radian)
   *
   *  Divide current angle with the new angle in radian.
   *
   *  \param  r Value of the angle in radian
   *  \return Copy of the Angle object
   */
  inline
  Angle Angle::operator / ( double r ) const {

    r = 1 / r;
    return (*this) * r;
  }


  /*! bool Angle::operator == ( const Angle &a ) const
   *  \brief  Compare the angle
   *
   *  This function compares the angles by taking absolute difference
   *  between the angles and compares the result with a tolerance
   *  value. Return true if the result is lower than the tolerance.
   *
   *  \param  a Angle object
   *  \return TRUE if equal
   */
  inline
  bool Angle::operator == ( const Angle &a ) const {

    return std::abs(_angle - a._angle) < ANGLE_TOLERANCE;
  }


  /*! bool Angle::operator != ( const Angle &a ) const
   *  \brief  Compare the angle
   *
   *  This function compares the angles.
   *
   *  \param  a Angle objec
   *  \return TRUE if not equal
   */
  inline
  bool Angle::operator != ( const Angle &a ) const {

    return !((*this) == a);
  }


  /*! bool Angle::operator < ( const Angle &a ) const
   *  \brief  Compare the angle
   *
   *  This function check if current angle is lower than
   *  the incomming angle.
   *
   *  \param  a Angle object
   *  \return TRUE if lower than incomming angle
   */
  inline
  bool Angle::operator < ( const Angle &a ) const {

    return _angle < a._angle;
  }


  /*! bool Angle::operator > ( const Angle &a ) const
   *  \brief  Compare the angle
   *
   *  This function check if current angle is greater than
   *  the incomming angle.
   *
   *  \param  a Angle object
   *  \return TRUE if greater than incomming angle
   */
  inline
  bool Angle::operator > ( const Angle &a ) const {

    return _angle > a._angle;
  }


  /*! bool Angle::operator <= ( const Angle &a ) const
   *  \brief  Compare the angle
   *
   *  This function check if current angle is lower or
   *  equals to the incomming angle.
   *
   *  \param  a Angle object
   *  \return TRUE if lower or euqals to the incomming angle
   */
  inline
  bool Angle::operator <= ( const Angle &a ) const {

    return !((*this) > a);
  }


  /*! bool Angle::operator >= ( const Angle &a ) const
   *  \brief  Compare the angle
   *
   *  This function check if current angle is greater or
   *  equals to the incomming angle.
   *
   *  \param  a Angle object
   *  \return TRUE if greater or equals to the incomming angle.
   */
  inline
  bool Angle::operator >= ( const Angle &a ) const {

    return !((*this) < a);
  }


  /*! Angle::operator double()
   *  \brief  Return the angle
   *
   *  Return the angle in radian, use the public funtion getRadian().
   *
   *  \return The angle in radian
   */
  inline
  Angle::operator double() {

    return getRad();
  }


  /*! Angle::operator int()
   *  \brief  Return the angle
   *
   *  Return the angle in degrees, use the public function getDegree().
   *
   *  \return The angle in degrees
   */
  inline
  Angle::operator int() {

    return getDeg();
  }


#ifdef GM_STREAM

  template <typename T_Stream>
  T_Stream& operator << ( T_Stream &out, const Angle &a ) {

    out << a.getRad();
    return out;
  }



  template <typename T_Stream>
  T_Stream& operator >> ( T_Stream &in, Angle &a ) {

    double tmp;

    in >> tmp;
    a = tmp;

    return in;
  }

#endif


} // END namespace GMlib




#endif // GM_CORE_TYPES_ANGLE_H

