#include "hidkbmouseinput.h"


// Qt
#include <QKeySequence>
#include <QStringList>


KeyModifierInput::KeyModifierInput( const Qt::KeyboardModifiers& keymods, const QVariant& type )
  : HidInput(type) {  setKeyboardModifiers( keymods ); }

Qt::KeyboardModifiers
KeyModifierInput::getKeyboardModifiers() const {
  return static_cast<Qt::KeyboardModifiers>(getData( "keyboard_modifiers" ).toInt());
}

void
KeyModifierInput::setKeyboardModifiers( const Qt::KeyboardModifiers& keymods ) {
  setData( "keyboard_modifiers", QVariant(int(keymods)) );
}

bool KeyModifierInput::isKeyboardModifiersActive(const Qt::KeyboardModifiers& modifiers) const {
  return getKeyboardModifiers() == modifiers;
}

QString KeyModifierInput::toString() const {

  QStringList keymods_str;

  const Qt::KeyboardModifiers keymods = getKeyboardModifiers();

  if( keymods.testFlag( Qt::ShiftModifier ) )         keymods_str += QString("Shift");
  if( keymods.testFlag( Qt::ControlModifier ) )       keymods_str += QString("Ctrl");
  if( keymods.testFlag( Qt::AltModifier ) )           keymods_str += QString("Alt");
  if( keymods.testFlag( Qt::MetaModifier ) )          keymods_str += QString("Meta");
  if( keymods.testFlag( Qt::KeypadModifier ) )        keymods_str += QString("Keypad");
  if( keymods.testFlag( Qt::GroupSwitchModifier ) )   keymods_str += QString("GrpSw");

  return keymods_str.join(" + ");
}






KeyInput::KeyInput( const Keymap& keymap, const Qt::KeyboardModifiers& keymods, const QVariant& type )
  : KeyModifierInput( keymods, type ) {

  setKeymap( keymap );

  setSinglekey(false);
}

KeyInput::KeyInput(const Qt::Key &key, const Qt::KeyboardModifiers &keymods, const QVariant &type)
  : KeyModifierInput( keymods, type ) {

  Keymap keymap;
  keymap[key] = true;
  setKeymap(keymap);

  setSinglekey(true);
}

KeyInput::Keymap
KeyInput::getKeymap() const {

  QList<QVariant>         keys_var    = getData("keymap_keys").toList();
  QMap<QString,QVariant>  keymap_var  = getData("keymap").toMap();
  Keymap                  keymap;
  for( QList<QVariant>::iterator itr = keys_var.begin(); itr != keys_var.end(); ++itr )
    keymap[static_cast<Qt::Key>((*itr).toInt())] = keymap_var.value( QString((*itr).toInt()), QVariant(false) ).toBool();

  return keymap;
}

void
KeyInput::setKeymap( const Keymap& keymap ) {

  QList<Qt::Key> keys = keymap.keys();
  QList<QVariant> keys_var;
  QMap<QString,QVariant> keymap_var;
  for( QList<Qt::Key>::iterator itr = keys.begin(); itr != keys.end(); ++itr ) {
    keys_var.append( int(*itr) );
    keymap_var[QString(int(*itr))] = keymap.value( *itr, false );
  }

  setData( "keymap_keys", keys_var );
  setData( "keymap", keymap_var );
}

bool KeyInput::isSingleKey() const {

  return getData("keyinput_single_key").toBool();
}

void KeyInput::setSinglekey(bool state) {

  setData("keyinput_single_key", QVariant(state));
}

bool KeyInput::isKeyActive(Qt::Key key) const {

  const Keymap keymap = getKeymap();
  return keymap.value( key, false );
}

bool KeyInput::isKeymapEqual(const Keymap &keymap_other) const {

  const Keymap keymap = getKeymap();

  QList<Qt::Key> keys, keys_other;
  keys        = keymap.uniqueKeys();
  keys_other  = keymap_other.uniqueKeys();

  if( keys.size() != keys_other.size() )
    return false;

  qSort(keys);
  qSort(keys_other);

  return keys == keys_other;
}

QString KeyInput::toString() const {

  const Keymap keymap = getKeymap();

  int keys_value = 0;
  for( Keymap::const_iterator itr = keymap.begin(); itr != keymap.end(); ++itr )
    keys_value += itr.key();

  QKeySequence ks(keys_value);

  if( !getKeyboardModifiers() )
    return ks.toString();
  else
    return KeyModifierInput::toString() + " + " + ks.toString();
}

bool KeyInput::equals(const HidInput & other) const {

  if( getType() != other.getType() )
    return false;

  const KeyInput key_input = static_cast<const KeyInput&>(other);

  if( getKeyboardModifiers() != key_input.getKeyboardModifiers() )
    return false;

  if( isSingleKey() )
    return key_input.isKeyActive( getKeymap().keys().first() );
  else
    return key_input.isKeymapEqual( getKeymap() );
}






KeyPressInput::KeyPressInput( const Keymap& keymap, const Qt::KeyboardModifiers& keymods )
  : KeyInput( keymap, keymods, QVariant( "KeyPress" ) ) {}

KeyPressInput::KeyPressInput(const Qt::Key &key, const Qt::KeyboardModifiers &keymods)
  : KeyInput( key, keymods, QVariant( "KeyPress" ) ) {}

KeyReleaseInput::KeyReleaseInput(const Qt::Key &key, const Qt::KeyboardModifiers &keymods)
  : KeyInput( key, keymods, QVariant( "KeyRelease" ) ) {}

QString KeyReleaseInput::toString() const {

  return KeyInput::toString() + " (Release)";
}









MouseButtonInput::MouseButtonInput( const Qt::MouseButtons& buttons, const Qt::KeyboardModifiers& keymods, const QVariant& type )
  : KeyModifierInput( keymods, type ) {  setMouseButtons(buttons); }

Qt::MouseButtons
MouseButtonInput::getMouseButtons() const {
  return static_cast<Qt::MouseButtons>(getData( "mouse_buttons" ).toInt());
}

void
MouseButtonInput::setMouseButtons( const Qt::MouseButtons& buttons ) {
  setData( "mouse_buttons", QVariant(int(buttons)) );
}

QString MouseButtonInput::toString() const {

  QStringList mb_str_list;

  const Qt::MouseButtons mouse_buttons = getMouseButtons();

  if( mouse_buttons.testFlag( Qt::LeftButton ) )      mb_str_list += QString("Left");
  if( mouse_buttons.testFlag( Qt::RightButton ) )     mb_str_list += QString("Right");
  if( mouse_buttons.testFlag( Qt::MiddleButton ) )    mb_str_list += QString("Middle");
  if( mouse_buttons.testFlag( Qt::ExtraButton1 ) )    mb_str_list += QString("X1");
  if( mouse_buttons.testFlag( Qt::ExtraButton2 ) )    mb_str_list += QString("X2");
  if( mouse_buttons.testFlag( Qt::ExtraButton3 ) )    mb_str_list += QString("X3");
  if( mouse_buttons.testFlag( Qt::ExtraButton4 ) )    mb_str_list += QString("X4");
  if( mouse_buttons.testFlag( Qt::ExtraButton5 ) )    mb_str_list += QString("X5");
  if( mouse_buttons.testFlag( Qt::ExtraButton6 ) )    mb_str_list += QString("X6");
  if( mouse_buttons.testFlag( Qt::ExtraButton7 ) )    mb_str_list += QString("X7");
  if( mouse_buttons.testFlag( Qt::ExtraButton8 ) )    mb_str_list += QString("X8");
  if( mouse_buttons.testFlag( Qt::ExtraButton9 ) )    mb_str_list += QString("X9");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X10");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X11");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X12");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X13");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X14");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X15");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X16");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X17");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X18");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X19");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X20");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X21");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X22");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X23");
  if( mouse_buttons.testFlag( Qt::ExtraButton10 ) )   mb_str_list += QString("X24");


  QString mb_anot;
  if( mb_str_list.size() > 1 )
    mb_anot = " Buttons";
  else if( mb_str_list.size() > 0 )
    mb_anot = " Button";

  if( !getKeyboardModifiers() )
    return mb_str_list.join( " and " ) + mb_anot;
  else
    return KeyModifierInput::toString() + " + " + mb_str_list.join( " and " ) + mb_anot;
}

bool MouseButtonInput::equals(const HidInput &other) const {

  if( other.getType() != getType() )
    return false;

  const MouseButtonInput& mbi = static_cast<const MouseButtonInput&>( other );

  if( mbi.getKeyboardModifiers() != getKeyboardModifiers() )
    return false;

  return  mbi.getMouseButtons() == getMouseButtons();
}




MousePressInput::MousePressInput( const Qt::MouseButtons& buttons, const Qt::KeyboardModifiers& keymods )
  : MouseButtonInput( buttons, keymods, "MousePress" ) {}


MouseReleaseInput::MouseReleaseInput( const Qt::MouseButtons& buttons, const Qt::KeyboardModifiers& keymods )
  : MouseButtonInput( buttons, keymods, "MouseRelease" ) {}

QString MouseReleaseInput::toString() const {

  return MouseButtonInput::toString() + " (Release)";
}


MouseDoubleClickInput::MouseDoubleClickInput( const Qt::MouseButtons& buttons, const Qt::KeyboardModifiers& keymods )
  : MouseButtonInput( buttons, keymods, "MouseDoubleClick" ) {}

QString MouseDoubleClickInput::toString() const {

  return "DBL Click: " + MouseButtonInput::toString();
}


MouseMoveInput::MouseMoveInput( const Qt::MouseButtons& buttons, const Qt::KeyboardModifiers& keymods )
  : MouseButtonInput( buttons, keymods, "MouseMove" ) {}

QString MouseMoveInput::toString() const {

  return "Move: " + MouseButtonInput::toString();
}









WheelInput::WheelInput( const Qt::KeyboardModifiers& keymods )
  : KeyModifierInput( keymods, "Wheel" ) {}

QString WheelInput::toString() const {

  QString wheel_str = QString( "Wheel");

  if( !getKeyboardModifiers() )
    return wheel_str;
  else
    return KeyModifierInput::toString() + " + " + wheel_str;
}

bool WheelInput::equals(const HidInput& other) const {

  if( other.getType() != getType() )
    return false;

  const WheelInput wi = static_cast<const WheelInput&>( other );
  return wi.getKeyboardModifiers() == getKeyboardModifiers();
}




