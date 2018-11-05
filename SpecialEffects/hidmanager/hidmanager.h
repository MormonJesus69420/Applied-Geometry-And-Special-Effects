#ifndef HIDMANAGER_H
#define HIDMANAGER_H


// local
#include "hidinput.h"
#include "hidbinding.h"
#include "hidinputevent.h"

class HidAction;
//class HidManagerModel;
class HidManagerTreeModel;

// GMlib
#include <gmCoreModule>

namespace GMlib {
  class Camera;
  class GMWindow;
  class Scene;
  class SceneObject;
}



// Qt
#include <QtCore>

class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;


// stl
#include <string>
#include <cassert>


class CustomEventLoop;
class HidManager;



class HidManager : public QObject {
  Q_OBJECT
public:
  typedef QSet<const HidAction*>    HidActions;
  typedef QList<const HidAction*>   HidActionsList;
  typedef QSet<const HidBinding>    HidBindings;

  explicit HidManager( QObject *parent = 0);

  const HidActions&         getHidActions() const;
  const HidBindings&        getHidBindings() const;

  QString                   registerHidAction( const QString& name,
                                               const QString& description,
                                               const QString& group,
                                               const QObject* receiver, const char* method,
                                               unsigned int custom_trigger= 0
                                               );

  bool                      registerHidMapping( const QString& action_name, const HidInput* hid_input );

  HidManagerTreeModel*      getModel() const;

  void                      forceUpdate();         // Needs to go

protected:
  void                      customEvent( QEvent* event ) override;

  virtual void              triggerAction( const HidAction* action, const HidInputEvent::HidInputParams& params );

public:
  HidActions              _hid_actions;
  HidBindings             _hid_bindings;
private:

  HidManagerTreeModel     *_model;


signals:
  void        signBeforeHidAction();
  void        signAfterHidAction();



}; // END class HidManager



#endif // HIDMANAGER_H
