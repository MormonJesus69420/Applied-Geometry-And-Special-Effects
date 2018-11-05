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





// gmlib
#include <core/types/gmpoint.h>
#include <core/types/gmmatrix.h>
#include <core/utils/gmutils.h>


#ifndef GM_SCENE_SCALEOBJECT_H
#define GM_SCENE_SCALEOBJECT_H

// stl
#include <cmath>
#include <limits>

namespace GMlib {


  /*! \class ScaleObj
   *  \brief Pending Documentation
   *
   *  Scaling Object
   */
  class ScaleObject {
  public:
    ScaleObject();
    ScaleObject(float	s);
    ScaleObject(Point<float,3>	sc);

    const HqMatrix<float,3>&  getMatrix() const;
    float                     getMax() const;
    Point<float,3>const&      getScale() const;
    void                      glScaling();

    bool                      isActive()  const;

    void                      reset();
    void                      reset(const Point<float,3>& sc);
    void                      scale(const Point<float,3>& sc);
    Sphere<float,3>           scaleSphere(const Sphere<float,3>& sp) const;


  private:
    bool                      _scaled;
    Point<float,3>            _s;
    HqMatrix<float,3>         _matrix;
    float                     _max;

    void                      _updateMax();


  #ifdef GM_STREAM
    public:
      template<class T_Stream>
      friend T_Stream& operator<<(T_Stream& out, const ScaleObject& v)
      {
        out << bool(v._scaled) << GMseparator::element() << v._s << GMseparator::element() << v._max ;
        return out;
      }

      template<class T_Stream>
      friend T_Stream& operator>>(T_Stream& in, ScaleObject& v)
      {
        static Separator es(GMseparator::element());
        //		GLubyte r,g,b,a;
        int scaled;
        in >> scaled >> es >> v._s >> es >> v._max;
        v._scaled = bool(scaled);
        return in;
      }

  #endif

  }; // END class ScaleObject












  /*! ScaleObject::ScaleObject()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  ScaleObject::ScaleObject() {
    reset();
  }


  /*! ScaleObject::ScaleObject(float	s)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  ScaleObject::ScaleObject(float	s) {
    reset(Point<float,3>(s));
  }


  /*! ScaleObject::ScaleObject(Point<float,3>	sc)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  ScaleObject::ScaleObject(Point<float,3>	sc) {
    reset(sc);
  }


  /*! void ScaleObject::_updateMax()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ScaleObject::_updateMax() {
    _max = (_s[0] > _s[1] ? ( _s[0]>_s[2] ? _s[0]:_s[2] ):(_s[1]>_s[2] ? _s[1]:_s[2]));
  }


  inline
  const HqMatrix<float,3>& ScaleObject::getMatrix() const {

    static HqMatrix<float,3> mat;
    mat[0][0] = _s(0);
    mat[1][1] = _s(1);
    mat[2][2] = _s(2);
    return mat;
  }


  /*! float ScaleObject::getMax() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  float ScaleObject::getMax() const {
    return _max;
  }


  /*! Point<float,3>const& ScaleObject::getScale()const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  Point<float,3>const& ScaleObject::getScale() const{
    return _s;
  }


  /*! void ScaleObject::glScaling()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ScaleObject::glScaling() {
//    if(_scaled) glScale(_s);
  }


  /*! bool  ScaleObject::isActive()  const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool  ScaleObject::isActive()  const {
    return _scaled;
  }


  /*! void ScaleObject::reset()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ScaleObject::reset() {
    _scaled=false; _s=Point<float,3>(1);
  }


  /*! void ScaleObject::reset(const Point<float,3>& sc)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ScaleObject::reset(const Point<float,3>& sc) {
      if (!GMutils::compValueF(sc(0), 1.0f) ||
          !GMutils::compValueF(sc(1), 1.0f) ||
          !GMutils::compValueF(sc(2), 1.0f)) {
          _s = sc;
          _updateMax();
          _scaled = true;
      }
      else {
          _scaled = false;
          _s      = sc;
      }
  }


  /*! void ScaleObject::scale(const Point<float,3>& sc)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void ScaleObject::scale(const Point<float,3>& sc) {

    _scaled=true;
    _s %= sc;
//    _matrix.scale( sc );
    _updateMax();
  }


  /*! Sphere<float,3> ScaleObject::scaleSphere(const Sphere<float,3>& sp) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  Sphere<float,3> ScaleObject::scaleSphere(const Sphere<float,3>& sp) const {

    if(_scaled)
    {
      if(sp.isValid()) return Sphere<float,3>(_max*sp.getPos(),_max*sp.getRadius());
      else return sp;
    }
    else return sp;
  }


} // END namespace GMlib


#endif // GM_SCENE_SCALEOBJECT_H
