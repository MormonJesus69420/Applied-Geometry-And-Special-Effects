#ifndef HIDINPUTEVENT_H
#define HIDINPUTEVENT_H

// local
#include "hidinput.h"


// qt
#include <QEvent>

class HidInputEvent : public QEvent {
public:
  static const QEvent::Type HID_INPUT;
  typedef QHash<QString,QVariant> HidInputParams;

  explicit HidInputEvent( const HidInput& input, const HidInputParams& params = HidInputParams() );
  explicit HidInputEvent( const HidInputEvent& copy );

  const QVariant&                 getType() const;
  const HidInput&                 getInput() const;
  const HidInputParams&           getParams() const;

private:
  const HidInput                  _input;
  const HidInputParams            _params;
};


#endif // HIDINPUTEVENT_H
