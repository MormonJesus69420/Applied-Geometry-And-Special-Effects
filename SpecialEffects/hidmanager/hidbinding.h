#ifndef HIDBINDING_H
#define HIDBINDING_H


// local
class HidInput;
#include "hidinput.h"

// qt
#include <QString>
#include <QHash>



class HidBinding {
public:
  HidBinding( const QString& action_name, const HidInput* input );

  const QString&      getActionName() const;
  QString             getHidBindingText() const;
  const HidInput*     getInput() const;

  bool                operator == ( const HidBinding& binding ) const;

private:
  const QString       _action_name;
  const HidInput*     _input;
};

inline
uint qHash( const HidBinding& binding ) { return qHash( binding.getActionName() );   }


struct HidBindingCompare : public std::unary_function<const HidBinding, bool> {

  explicit HidBindingCompare( const HidInput& input );
  explicit HidBindingCompare( const QString text );

  bool                  operator () ( const HidBinding& binding ) const;

  bool                  _comp_input;
  const QString         _text;
  const HidInput&       _input;
};



#endif // HIDBINDING_H
