#include "standardhidmanager.h"

// local
#include "hidinputevent.h"
#include "hidkbmouseinput.h"


// qt
#include <QtCore>
#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#define QT_NO_OPENGL
//#include <QtWidgets>

// stl
#include <cmath>
#include <cassert>


StandardHidManager::StandardHidManager( QObject* parent )
  : HidManager(parent) {

  // Mutable registers
  _reg_wheel_state = false;
  _reg_next_key_event_type = KEY_NONE;
  _reg_next_mouse_event_type = MOUSE_NONE;
  _reg_key_last_unreg = Qt::Key_Any;
}


void StandardHidManager::registerKeyPressEvent(const QString& name, QKeyEvent *e) {

  registerRCPairName( name );
  registerKey( Qt::Key(e->key()), e->modifiers() );
  registerKeyEventType( KEY_PRESS );
  generateEvent();
}


void StandardHidManager::registerWindowPosition( const QPoint& pos ) {

  _reg_view_prev_pos = _reg_view_pos;
  _reg_view_pos = pos;
}


void StandardHidManager::registerKeyReleaseEvent(const QString& name, QKeyEvent *e) {

  registerRCPairName( name );
  unregisterKey( Qt::Key(e->key()), e->modifiers() );
  registerKeyEventType( KEY_RELEASE );

  generateEvent();
}


void StandardHidManager::registerMouseDoubleClickEvent(const QString& name, QMouseEvent* e ) {

  registerRCPairName( name );
  registerWindowPosition( e->pos() );
  registerMouseButtons( e->buttons(), e->modifiers() );
  registerMouseEventType( MOUSE_DBL_CLICK );

  generateEvent();
}


void StandardHidManager::registerMouseMoveEvent(const QString& name, QMouseEvent* e) {

  registerRCPairName( name );
  registerWindowPosition( e->pos() );
  registerMouseEventType( MOUSE_MOVE );

  generateEvent();
}


void StandardHidManager::registerMousePressEvent(const QString& name, QMouseEvent* e) {

  registerRCPairName( name );
  registerWindowPosition( e->pos() );
  registerMouseButtons( e->buttons(), e->modifiers() );
  registerMouseEventType( MOUSE_CLICK );

  generateEvent();
}


void StandardHidManager::registerMouseReleaseEvent(const QString& name, QMouseEvent* e){

  registerRCPairName( name );
  registerWindowPosition( e->pos() );
  registerMouseButtons( e->buttons(), e->modifiers() );
  registerMouseEventType( MOUSE_RELEASE );

  generateEvent();
}


void StandardHidManager::registerKey(Qt::Key key, Qt::KeyboardModifiers modifiers) {

  _reg_keymods = modifiers;
  if( isKeyRegistered( key ) )
    return;

  _reg_keymap[key] = true;
}


void StandardHidManager::registerMouseButtons(Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers ) {

  _reg_mouse_buttons = buttons;
  _reg_keymods = modifiers;
}


void StandardHidManager::registerKeyEventType(StandardHidManager::KeyEventType type) {

  _reg_next_key_event_type = type;
}


void StandardHidManager::registerMouseEventType(StandardHidManager::MouseEventType type ) {

  _reg_next_mouse_event_type = type;
}


void StandardHidManager::unregisterKey(Qt::Key key, Qt::KeyboardModifiers modifiers) {


  // Update modifier
  _reg_keymods = modifiers;

  // Update keymap
  if( !isKeyRegistered(key) )
    return;

  _reg_key_last_unreg = key;

  _reg_keymap.remove( key );
}


void StandardHidManager::registerWheelData(bool state, int delta) {

  _reg_wheel_state = state;
  _reg_wheel_delta = delta;
}


void StandardHidManager::registerWheelEvent(const QString& name, QWheelEvent *e) {

  // Save position and wheel delta
  registerRCPairName( name );
  registerWindowPosition( e->pos() );
  registerWheelData( true, e->angleDelta().y() );

  generateEvent();
}


QString
StandardHidManager::viewNameFromParams(const HidInputEvent::HidInputParams& params) {
  return params["view_name"].toString();
}


QPoint
StandardHidManager::posFromParams(const HidInputEvent::HidInputParams& params) {
  return params["pos"].toPoint();
}


QPoint
StandardHidManager::prevPosFromParams(const HidInputEvent::HidInputParams& params) {
  return params["prev_pos"].toPoint();
}


int
StandardHidManager::wheelDeltaFromParams(const HidInputEvent::HidInputParams& params) {
  return params["wheel_delta"].toInt();
}


bool StandardHidManager::isKeyRegistered(Qt::Key key) const {
  return _reg_keymap.value(key, false);
}


bool StandardHidManager::isAnyKeysRegistered() const {
  return _reg_keymap.size() > 0;
}


bool StandardHidManager::isModKeyRegistered(Qt::KeyboardModifier keymod) const {

  // No modifier is a special case
  if( keymod == Qt::NoModifier ) {
    return _reg_keymods == Qt::NoModifier;
  }

  return (_reg_keymods & keymod) == keymod;
}


bool StandardHidManager::isMouseButtonRegistered(Qt::MouseButton button) const {

  // No button is a special case
  if( button == Qt::NoButton ) {
    return _reg_mouse_buttons == Qt::NoButton;
  }

  return (_reg_mouse_buttons & button) == button;
}


void StandardHidManager::generateEvent() {

  HidInputEvent::HidInputParams params;
  params["view_name"] = QVariant( _reg_rcpair_name );

  HidInputEvent::HidInputParams key_params {params};

  HidInputEvent::HidInputParams mouse_params {params};
  mouse_params["pos"]      = QVariant( _reg_view_pos );
  mouse_params["prev_pos"] = QVariant( _reg_view_prev_pos );

  HidInputEvent::HidInputParams wheel_params {params};
  wheel_params["wheel_delta"] = QVariant(_reg_wheel_delta);

  if( _reg_next_mouse_event_type == MOUSE_MOVE ) {
    QCoreApplication::sendEvent( this, new HidInputEvent( MouseMoveInput( _reg_mouse_buttons, _reg_keymods ), mouse_params ) );
    registerMouseEventType( MOUSE_NONE );
  }
  else if( _reg_wheel_state ) {
    QCoreApplication::sendEvent( this, new HidInputEvent( WheelInput( _reg_keymods ), wheel_params ) );
    registerWheelData(false,0);
  }
  else if( _reg_next_mouse_event_type != MOUSE_NONE ) {

    switch( _reg_next_mouse_event_type ) {
      case MOUSE_DBL_CLICK:
        QCoreApplication::sendEvent( this, new HidInputEvent( MouseDoubleClickInput( _reg_mouse_buttons, _reg_keymods ), mouse_params ) );
        break;
      case MOUSE_CLICK:
        QCoreApplication::sendEvent( this, new HidInputEvent( MousePressInput( _reg_mouse_buttons, _reg_keymods ), mouse_params ) );
        break;
      case MOUSE_RELEASE:
        QCoreApplication::sendEvent( this, new HidInputEvent( MouseReleaseInput( _reg_mouse_buttons, _reg_keymods ), mouse_params ) );
        break;
      case MOUSE_NONE:
      case MOUSE_MOVE:
      default:
        assert(false);
    }

    registerMouseEventType( MOUSE_NONE );
  }
  else if( _reg_next_key_event_type != KEY_NONE ) {

    switch( _reg_next_key_event_type ) {
      case KEY_PRESS: {
        QCoreApplication::sendEvent( this, new HidInputEvent( KeyPressInput( _reg_keymap, _reg_keymods ), key_params ) );
      } break;
      case KEY_RELEASE: {
        QCoreApplication::sendEvent( this, new HidInputEvent( KeyReleaseInput( _reg_key_last_unreg, _reg_keymods ), key_params ) );
      } break;
      case KEY_NONE:
      default:
        assert(false);
    }

    registerKeyEventType( KEY_NONE );
  }

}


void StandardHidManager::registerRCPairName(const QString& name) {
  _reg_rcpair_name = name;
}


