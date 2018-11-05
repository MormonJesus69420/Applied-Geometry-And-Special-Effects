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

#ifndef GM_SCENE_EVENT_GMEVENTMANAGER_H
#define GM_SCENE_EVENT_GMEVENTMANAGER_H

//- gmlib
#include <core/containers/gmarray.h>

namespace GMlib {

  class Event;
  class EventController;

  /*!
   * \class EventManager gmeventmanager.h <gmEventManager>
   * \brief The EventManager class
   *
   *  The central EventManager class that handles EventControllers.
   *
   */
  class EventManager {
  public:
    EventManager();
    ~EventManager();

    void    processEvents(double dt);
    bool    registerController(EventController* controller);

  private:
    bool    handleFirstEvent();

    Array<EventController*> _event_controllers;
  };

}

#endif // GM_SCENE_EVENT_GMEVENTMANAGER_H

