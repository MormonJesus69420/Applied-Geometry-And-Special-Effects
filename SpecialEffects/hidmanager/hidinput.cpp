#include "hidinput.h"


HidInput::HidInput() : _type(), _data() {}

HidInput::HidInput(const QVariant &type) : _type(type), _data() {}

HidInput::HidInput(const HidInput &copy) : _type(copy._type), _data(copy._data) {}

const
QVariant& HidInput::getType() const {  return _type; }

bool
HidInput::equals( const HidInput& /*other*/ ) const { return false; }

const QMap<QString,QVariant>&
HidInput::getData() const { return _data; }

QVariant
HidInput::getData( const QString& name ) const { return _data.value(name); }

void
HidInput::setData( const QString& name, const QVariant& data ) { _data[name] = data; }

QString
HidInput::toString() const { return QString(); }

bool
HidInput::operator <  ( const HidInput& other ) const { return sizeof(_type) + sizeof(_data); }

bool
HidInput::operator == ( const HidInput& other ) const {  return equals(other); }

const HidInput&
HidInput::getDefault() {

  static const HidInput hid_input = HidInput();
  return hid_input;
}


