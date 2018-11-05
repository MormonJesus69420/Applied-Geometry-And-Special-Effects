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

#ifndef GM_SCENE_EVENT_GMEVENTCONTROLLER_H
#define GM_SCENE_EVENT_GMEVENTCONTROLLER_H

//- gmlib
#include <core/containers/gmarray.h>

namespace GMlib {

  class Event;
  class SceneObject;

  /*!
   * \class EventController gmeventcontroller.h <gmEventController>
   * \brief EventController base class.
   *
   *  Provides an interface to detect and control Events that can happen
   *  to SceneObjects during a time step dt.
   *
   *  Inherited classes need to implement functions which detects
   *    1. Events within a given dt (first pass)
   *    2. Any events following after handling of a specific event
   *       within that dt (second pass)
   *    NB! When storing events in Array, use insertAlways for performance reasons.
   *
   *  Optionally, inherited classes can store customized information
   *  and perform updates based on events.
   *
   */
  class EventController {
  public:
    explicit EventController();
    virtual ~EventController();

    double  getFirstEventX() const;

    bool    detectEvents( double dt );
    void    handleFirstEvent();
    void    finalize();

  private:
    /*!
     *  \brief void clear()
     *
     *  Clears remaining events before event detection is initialized.
     */
    virtual void    clear() = 0;

    /*!
     *  \brief bool detect( double dt )
     *  \return Whether any events where detected.
     *
     *  Detects events within a given dt.
     *  This is the first pass of event detection.
     *  It is also where the controller gets <i>dt</i>.
     */
    virtual bool    detect( double dt ) = 0;

    /*!
     *  \brief void handleFirst()
     *
     *  This function is to handle the first event in the controller
     *  and only the first event, then pop it.
     *  Depending on the type and purpose of the controller this function might
     *  invalidate, detect new or remove events as a direct cause of handlig the first event.
     *  It should also sort the remaining events.
     */
    virtual void    handleFirst() = 0;

    /*!
     *  \brief void doFinalize()
     *
     *  This function is called after all events have been handled.
     */
    virtual void    doFinalize() {}

    /*!
     *  \brief double getFirstX() const
     *  \return The X of the first event.
     *
     *  Returns the X (time of the events relative to the dt)
     *  of the first event in the controller.
     *  If the controller has no events it should return 0.0,
     *  as an event is only valid on (0.0, x]
     */
    virtual double  getFirstX() const = 0;
  };

}

#endif // GM_SCENE_EVENT_GMEVENTCONTROLLER_H

