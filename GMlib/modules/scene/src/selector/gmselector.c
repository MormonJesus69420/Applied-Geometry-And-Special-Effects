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






#include "../sceneobjects/gmsphere3d.h"
#include "../visualizers/gmselectorvisualizer.h"

namespace GMlib {


  /*! Selector<T,n>::Selector( Point<T,n>& mp, int id, SceneObject* parent, T r, const Color& c, Selector<T,n>* root )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  Selector<T,n>::Selector( Point<T,n>& mp, int id, SceneObject* parent,
                           T r, const Color& c, Selector<T,n>* root )
    : _position(mp)
  {

    Sphere<float,3> ts(Point<float,3>(float(0)),1);
    setSurroundingSphere(ts);
    _type_id    = GM_SO_TYPE_SELECTOR;
    _id         = id;
    _parent     = parent;
    _enabled    = true;
    _default    = c;
    _marked     = c.getInverse();
    _selected   = false;
    _root       = root;
    _scaled     = false;
    translateParent( _position.template toType<float>() );
    if(r != 1.0) scale(Vector<float,3>(r,r,r));

    insertVisualizer( SelectorVisualizer::getInstance() );
  }


  /*! Selector<T,n>::Selector( Point<T,n>& mp, int id, SceneObject* parent, const Vector<T,n>& scale, T r, const Color& c, Selector<T,n>* root )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  Selector<T,n>::Selector( Point<T,n>& mp, int id, SceneObject* parent, const Vector<T,n>& scale_v,
                           T r, const Color& c, Selector<T,n>* root )
    : _position(mp)
  {

    Sphere<float,3> ts(Point<float,3>(float(0)),1);
    setSurroundingSphere(ts);
    _type_id    = GM_SO_TYPE_SELECTOR;
    _id         = id;
    _parent     = parent;
    _enabled    = true;
    _default    = c;
    _marked     = c.getInverse();
    _selected   = false;
    _root       = root;
    scale(scale_v, false);
    translateParent( (scale_v % _position).template toType<float>() );
    if(r != 1.0) this->scale(Vector<float,3>(r,r,r));

    insertVisualizer( SelectorVisualizer::getInstance() );
  }


  /*! Selector<T,n>::Selector(const Selector<T,n>& s)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  Selector<T,n>::Selector(const Selector<T,n>& s)
    : SceneObject(s), _position( s._position )
   {

    _type_id  = GM_SO_TYPE_SELECTOR;
    _id       = s._id;
    _parent   = s._parent;
    _enabled  = s._enabled;
    _default  = s._default;
    _marked   = s._marked;
    _root     = s._root;

    insertVisualizer( SelectorVisualizer::getInstance() );
  }

  /*!virtual Selector<T,n>::~Selector()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  Selector<T,n>::~Selector() {

    removeVisualizer( SelectorVisualizer::getInstance() );
  }


  /*! void Selector<T,n>::allEnable()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  all children on
   */
  template <typename T, int n>
  void Selector<T,n>::allEnable() {

    for(int i=0; i<_children.getSize(); i++)
    {
      Selector<T,n> *s=dynamic_cast<Selector<T,n> *>(_children[i]);
      if(s)
        s->enableAll();
    }
  }


  /*! void Selector<T,n>::allDisable()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  all children off
   */
  template <typename T, int n>
  void Selector<T,n>::allDisable() {

    for(int i=0; i<_children.getSize(); i++)
    {
      Selector<T,n> *s=dynamic_cast<Selector<T,n> *>(_children[i]);
      if(s)
        s->disableAll();
    }
  }




  /*! void  Selector<T,n>::disable()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  void  Selector<T,n>::disable() {

    _enabled=false;
    allEnable();

    if(_root)
      _root->enable();
  }


  /*! void  Selector<T,n>::disableAll()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  void  Selector<T,n>::disableAll() {

    _enabled = false;
    allEnable();
  }



  /*! void  Selector<T,n>::editPos(Vector<float,3> dp)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  void  Selector<T,n>::editPos(Vector<float,3> dp) {

    HqMatrix<float,3> invmat = _present;
    invmat.invertOrthoNormal();

    Vector<float,3> dp_local = invmat*dp;
    translateParent(dp_local);

    if(_scaled)
        dp_local %= _scale_inv;

    Vector<T,n>  dp_coef  = dp_local;
    _position += dp_coef;
    _parent->edit(_id, dp_coef);
  }



  /*! virtual void  Selector<T,n>::edit()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  void  Selector<T,n>::edit() {

    _parent->selectEvent(_id);
  }



  /*! void Selector<T,n>::scale(const Point<float,3>& scale_factor)
   *  \brief Pending Documentation
   *
   *  \param scale_factor Scaling separatly in all dimentions
   *  \param propagate    True means scale of the selector, false scale of mother object
   *  ( Only geometry )
   */
  template <typename T, int n>
  void Selector<T,n>::scale(const Point<float,3>& scale_factor, bool propagate) {

      SceneObject::scale(scale_factor, false);

      if(propagate) {

          for(int i=0; i<_children.getSize(); i++)
          {
              Point<float,3> tr = (scale_factor - Point<float,3>(1.0f))%_children[i]->getPos();
              _children[i]->translateParent(tr);
              _children[i]->scale(scale_factor);
          }
          if(_parent )
              _parent->edit( this );
      }
      else {
          _scaled = true;
          _scale_inv = scale_factor.getInverse();
      }
  }



  /*! void  Selector<T,n>::enable()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  void  Selector<T,n>::enable() {

    _enabled=true;

    if( _root )
      _root->disable();

    allDisable();
  }


  /*! void  Selector<T,n>::enableAll()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  void  Selector<T,n>::enableAll() {

    _enabled = true;
    allEnable();
  }

  template <typename T, int n>
  inline
  int Selector<T,n>::getId() const {

    return _id;
  }

  template <typename T, int n>
  inline
  const Point<T,n>& Selector<T,n>::getPosition() const {

    return _position;
  }


  /*! bool  Selector<T,n>::isEnabled() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  Is the children or my self on?
   */
  template <typename T, int n>
  bool  Selector<T,n>::isEnabled() const {

    for(int i=0; i<_children.getSize(); i++)
    {
      Selector<T,n> *s=dynamic_cast<Selector<T,n> *>(this->_children(i));
      if(s)
        if(s->isEnabled())
          return true;
    }
    return _enabled;
  }

  /*! bool Selector<T,n>::toggle()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  visibility
   */
  template <typename T, int n>
  inline
  bool Selector<T,n>::toggle() {

    _enabled=!_enabled;
    return _enabled;
  }


  /*! void Selector<T,n>::update()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  void Selector<T,n>::update() {

    Vector<T,n> d = _position - _pos;
std::cout << "Selector id=" << _id << " Selektor translate:" << d << std::endl;
    translateParent(d.template toType<float>());
    for(int i=0; i<_children.getSize(); i++)
    {
      Selector<T,n> *s = dynamic_cast<Selector<T,n> *>(_children[i]);
      if(s)  s->translateParent(-d.template toType<float>());  // don't bring children
    }
  }


  /*! void Selector<T,n>::update( const Point<T,n>& p )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  inline
  void Selector<T,n>::update( const Point<T,n>& p ) {

    Vector<T,n> d=p-_position;
    _position=p;
    translateParent(d.template toType<float>());
    for(int i=0; i<_children.getSize(); i++)
    {
      Selector<T,n> *s = dynamic_cast<Selector<T,n> *>(_children[i]);
      if(s)  s->translateParent(-d.template toType<float>());  // don't bring children
    }
  }

}
