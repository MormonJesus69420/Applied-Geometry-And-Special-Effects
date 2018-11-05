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



#ifndef GM_CORE_UTILS_SORTOBJECT_H
#define GM_CORE_UTILS_SORTOBJECT_H


namespace GMlib {


  template <typename T, typename G>
  class SortObject {
  public:
    SortObject( const T& obj = 0x0, const G& value = G(0) );

    const T&    getObject() const;
    const G&    getValue() const;

    bool        operator < ( const SortObject<T,G>& obj ) const;

  protected:
    T     _obj;
    G     _value;

  };




} // END namespace


// Include SortObject class function implementations
#include "gmsortobject.c"


#endif // GM_CORE_UTILS_SORTOBJECT_H
