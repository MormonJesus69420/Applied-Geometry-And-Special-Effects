#include "hidinputevent.h"

const QEvent::Type HidInputEvent::HID_INPUT =
    static_cast<QEvent::Type>( QEvent::registerEventType() );


HidInputEvent::HidInputEvent(const HidInput& input , const HidInputParams &params)
  : QEvent( HidInputEvent::HID_INPUT ), _input(input), _params(params) {}

HidInputEvent::HidInputEvent( const HidInputEvent& copy )
  : QEvent(copy), _input(copy._input) {}

const QVariant&
HidInputEvent::getType() const { return _input.getType(); }

const HidInput&
HidInputEvent::getInput() const { return _input; }

const HidInputEvent::HidInputParams &HidInputEvent::getParams() const {

  return _params;
}
