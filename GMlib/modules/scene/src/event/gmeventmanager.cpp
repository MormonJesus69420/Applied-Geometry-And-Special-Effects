#include "gmeventmanager.h"

#include <cassert>

#include "gmevent.h"
#include "gmeventcontroller.h"

using namespace GMlib;

EventManager::EventManager() {
}

EventManager::~EventManager() {
}

/*!
 * \brief EventManager::processEvents
 * \param dt - delta time
 *
 *  The algorithm works as follows:
 *    1. Asks controllers to detect new events with a <i>dt</i> simulation step
 *    2. Asks to handle the firt event for as long as there are valid events
 *    3. Call finalize to handle the "rest" of the simulation step in each controller
 */
void
EventManager::processEvents(double dt) {

  // detect events
  for( int i = 0; i < _event_controllers.size(); ++i )
    _event_controllers[i]->detectEvents( dt );

  // handle each event ...
  while( handleFirstEvent() );

  // finalize processEvents step
  for( int i = 0; i < _event_controllers.size(); ++i )
    _event_controllers[i]->finalize();
}

/*!
 * \brief EventManager::registerController
 * \param controller - EventController
 * \return false if the controller is already registered
 *
 *  Add a controller to be handled by the manager.
 */
bool
EventManager::registerController(EventController *controller) {
  return _event_controllers.insert(controller);
}

/*!
 * \brief EventManager::handleFirstEvent
 * \return Whether an event was handled
 *
 *  Handle the first event if an event exists in any controller.
 */
bool
EventManager::handleFirstEvent() {

  // 1) Find controller with lowest x event
  EventController* ctl = 0x0;
  double x = 0.0;
  for( int i = 0; i < _event_controllers.size(); ++i ) {

    EventController* c_ctl = _event_controllers[i];

    // Controller returns x == 0.0 if there is no event. and event must be in (0.0, x]
    const double c_x = c_ctl->getFirstEventX();
    if( c_x > 0.0 && ( c_x < x || x == 0.0 ) ) {  // Optimized: early termination
      x = c_x;
      ctl = c_ctl;
    }
  }

  // 2) Controller handle first event
  //  2.0) Controller detects new events
  //  2.1) Controller sorts events
  //  2.2) Controller invalidates events
  if( ctl ) {

    ctl->handleFirstEvent();
    return true;
  }

  return false;
}
