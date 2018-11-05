#ifndef HIDINPUT_H
#define HIDINPUT_H

// qt
#include <QVariant>

class HidInput {
public:
  HidInput();
  HidInput( const QVariant& type );
  HidInput( const HidInput& copy );

  const QVariant&                 getType() const;

  const QMap<QString,QVariant>&   getData() const;
  QVariant                        getData( const QString& name ) const;
  void                            setData( const QString& name, const QVariant& data );

  virtual QString                 toString() const;

  bool                            operator <  ( const HidInput& other ) const;
  bool                            operator == ( const HidInput& other ) const;

  static const HidInput&          getDefault(); 

private:
  QVariant                    _type;
  QMap<QString,QVariant>      _data;

  virtual bool                equals( const HidInput& /*other*/ ) const;
};

#endif //HIDINPUT_H
