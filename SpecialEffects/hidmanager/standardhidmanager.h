
#ifndef STANDARDHIDMANAGER_H
#define STANDARDHIDMANAGER_H

// local
#include "hidmanager.h"
#include "hidkbmouseinput.h"
#include "hidinputevent.h"

// Qt
class QMouseEvent;
class QKeyEvent;
class QWheelEvent;


// stl
#include <memory>


class StandardHidManager : public HidManager {
  Q_OBJECT
public:
  explicit StandardHidManager( QObject* parent = 0x0 );

  //**********************************
  //
  // Standard Hid manager setup

public slots:
  virtual void                registerMousePressEvent( const QString& name, QMouseEvent* event );
  virtual void                registerMouseReleaseEvent( const QString& name, QMouseEvent* event );
  virtual void                registerMouseDoubleClickEvent(  const QString& name, QMouseEvent* event );
  virtual void                registerMouseMoveEvent( const QString& name, QMouseEvent* event );
  virtual void                registerKeyPressEvent( const QString& name, QKeyEvent* event );
  virtual void                registerKeyReleaseEvent( const QString& name,  QKeyEvent* event );
  virtual void                registerWheelEvent( const QString& name, QWheelEvent* event );

protected:
  static QString              viewNameFromParams( const HidInputEvent::HidInputParams& params );
  static QPoint               posFromParams( const HidInputEvent::HidInputParams& params );
  static QPoint               prevPosFromParams( const HidInputEvent::HidInputParams& params );
  static int                  wheelDeltaFromParams( const HidInputEvent::HidInputParams& params );

private:
  enum MouseEventType {
    MOUSE_NONE,
    MOUSE_CLICK,
    MOUSE_DBL_CLICK,
    MOUSE_RELEASE,
    MOUSE_MOVE
  };

  enum KeyEventType {
    KEY_NONE,
    KEY_PRESS,
    KEY_RELEASE
  };

  void                        registerKeyEventType( KeyEventType type );
  void                        registerMouseEventType( MouseEventType type );
  virtual void                generateEvent();

  void                        registerKey( Qt::Key key,
                                           Qt::KeyboardModifiers modifiers );
  void                        unregisterKey( Qt::Key key,
                                             Qt::KeyboardModifiers modifiers );
  void                        registerMouseButtons( Qt::MouseButtons buttons,
                                                    Qt::KeyboardModifiers modifiers );
  void                        registerWheelData( bool state, int delta );
  void                        registerWindowPosition(const QPoint& pos );
  void                        registerRCPairName( const QString& name );

  KeyInput::Keymap            _reg_keymap;
  Qt::KeyboardModifiers       _reg_keymods;
  Qt::Key                     _reg_key_last_unreg;

  Qt::MouseButtons            _reg_mouse_buttons;

  bool                        _reg_wheel_state;
  int                         _reg_wheel_delta;

  QString                     _reg_rcpair_name    = {""};    // Current view name
  QPoint                      _reg_view_pos       = {0,0};
  QPoint                      _reg_view_prev_pos  = {0,0};

  bool                        isKeyRegistered( Qt::Key key ) const;
  bool                        isAnyKeysRegistered() const;
  bool                        isModKeyRegistered( Qt::KeyboardModifier keymod ) const;
  bool                        isMouseButtonRegistered( Qt::MouseButton button ) const;

  KeyEventType                _reg_next_key_event_type;
  MouseEventType              _reg_next_mouse_event_type;

};


#endif // STANDARDHIDMANAGER_H
