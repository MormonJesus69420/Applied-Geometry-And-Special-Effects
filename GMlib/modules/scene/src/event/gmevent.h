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

#ifndef GM_SCENE_EVENT_EVENT_H
#define GM_SCENE_EVENT_EVENT_H


// gmlib
#include <core/utils/gmutils.h>



namespace GMlib {

/*! \class Event gmevent.h <gmEvent>
 *
 *  \brief Base class to represent a scene event.
 *
 *  Stores the relative time, x, within the current dt when the event occurs.
 *
 */
  class Event {
  public:
    Event(double x = 1.0);

    double getX() const;

    bool   operator <  ( const Event& v ) const;
    bool   operator == ( const Event& v ) const;

  private:
    double _x;
  };

  inline double
  Event::getX() const {
    return _x;
  }

  inline bool
  Event::operator <(const Event &v) const {
    return _x < v._x;
  }

  inline bool
  Event::operator ==(const Event &v) const {
    return GMutils::compValueF(_x,v._x);
  }

}

#endif // GM_SCENE_EVENT_EVENT_H

