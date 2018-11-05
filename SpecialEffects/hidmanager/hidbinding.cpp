#include "hidbinding.h"

// local
#include "hidinput.h"

// stl
#include <cassert>


HidBinding::HidBinding( const QString& action_name, const HidInput* input )
  : _action_name(action_name), _input(input) {   assert(_input);  }

const QString&
HidBinding::getActionName() const { return _action_name; }

QString HidBinding::getHidBindingText() const {

  return _input->toString();
}

const HidInput*
HidBinding::getInput() const { return _input; }

bool
HidBinding::operator == ( const HidBinding& binding ) const {

  return (_action_name == binding._action_name) && binding.getInput()->operator ==( *_input );
}






HidBindingCompare::HidBindingCompare( const HidInput& input ) : _comp_input(true), _text(), _input(input) {}
HidBindingCompare::HidBindingCompare( const QString text ) : _comp_input(false), _text(text), _input(HidInput::getDefault()) {}

bool
HidBindingCompare::operator () ( const HidBinding& binding ) const {

  if( _comp_input )
    return *binding.getInput() == _input;

  return binding.getActionName() == _text;
}
