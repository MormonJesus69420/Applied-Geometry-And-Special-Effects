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





#ifndef GM_SCENE_SELECTOR_SELECTOR_H
#define GM_SCENE_SELECTOR_SELECTOR_H


#include "../gmsceneobject.h"

// gmlib
#include <opengl/gmbufferobject.h>


namespace GMlib{



  /*! \class Selector gmselector.h <gmSelector>
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  template <typename T, int n>
  class Selector: public SceneObject {
    GM_SCENEOBJECT_NULL(Selector)

  public:
    Selector(const Selector<T,n>& s);
    Selector(Point<T,n>& mp, int id, SceneObject* parent, T r=1, const Color& c = Color(0.0,0.0,0.7), Selector<T,n>* root=NULL );
    Selector(Point<T,n>& mp, int id, SceneObject* parent, const Vector<T,n>& scale, T r=1, const Color& c = Color(0.0,0.0,0.7), Selector<T,n>* root=NULL );
    virtual ~Selector();

    void                  disable();
    void                  disableAll();
    void                  enable();
    void                  enableAll();
    int                   getId() const;
    const Point<T,n>&     getPosition() const;
    bool                  isEnabled() const;
    bool                  toggle();
    void                  update();
    void                  update(const Point<T,n>& p);

    // *******************
    // Derived functions
    // from SceneObject
    void                  editPos(Vector<float,3> dp) override;
    void                  edit() override;
    void                  scale(const Point<float,3>& scale_factor, bool propagate = true ) override;


  protected:
    bool                  _enabled;
    Selector<T,n>*        _root;
    Color                 _default;
    Color                 _marked;

    void                  allDisable();
    void                  allEnable();


  private:
    int                   _id;
    SceneObject*          _parent;
    Point<T,n>&          _position;

    Vector<T,n>           _scale_inv;
    bool                  _scaled;

  }; // END class Selector

} // END namespace GMlib

// Include inline Selector class implementations
#include "gmselector.c"



#endif // GM_SCENE_SELECTOR_SELECTOR_H
