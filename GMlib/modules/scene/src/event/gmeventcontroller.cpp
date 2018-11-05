#include "gmeventcontroller.h"

using namespace GMlib;


EventController::EventController() {}
EventController::~EventController() {}

double EventController::getFirstEventX() const {

  return getFirstX();
}


bool EventController::detectEvents(double dt) {

  clear();
  return detect( dt );
}

void EventController::handleFirstEvent() {

  handleFirst();
}

/*!
 * \brief EventController::finalize
 *
 *  Finalizes the simulation frame.
 *  Is run once after all events of a frame has been handled
 */
void EventController::finalize() {

  doFinalize();
}
