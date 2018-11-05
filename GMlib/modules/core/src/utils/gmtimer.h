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



#ifndef GM_CORE_UTILS_TIMER_H
#define GM_CORE_UTILS_TIMER_H

// system
#include <sys/timeb.h>

namespace GMlib {


  /*! \class  GMTimer gmtimer.h <gmTimer>
   *  \brief  This is GMTimer class
   *
   *  This is GMTimer class.
   */
  class GMTimer {
  public:
    GMTimer();

    unsigned long   getMilli( bool set = false );
    double          getSec( bool set = false );
    void            update();


  private:
    timeb   _time;


  }; // END class GMTimer

} // END namespace GMlib

// Include inline GMTimer class implementations
#include "gmtimer.c"

#endif // GM_CORE_UTILS_TIMER_H
