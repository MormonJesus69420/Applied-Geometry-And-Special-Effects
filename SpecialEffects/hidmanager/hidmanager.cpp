#include "hidmanager.h"

// local
#include "hidinputevent.h"
#include "hidaction.h"
//#include "hidmanagermodel.h"
#include "hidmanagertreemodel.h"































HidManager::HidManager( QObject *parent) :
  QObject(parent) {

  _model = new HidManagerTreeModel(this,this);
}

const HidManager::HidActions& HidManager::getHidActions() const {

  return _hid_actions;
}

const HidManager::HidBindings &HidManager::getHidBindings() const {

  return _hid_bindings;
}

void HidManager::customEvent(QEvent *event) {

  if( event->type() != HidInputEvent::HID_INPUT )
    return;

  HidInputEvent *he= static_cast<HidInputEvent*>( event );

//  qDebug() << "-----------------";
//  qDebug() << "Processing Hid Input";
//  qDebug() << "\t Type: " << he->getInput().getType();
//  qDebug() << "\t Data: " << he->getInput().getData();

//  qDebug() << "Mappings count: " << _hid_bindings.size();

  QSet<const HidBinding>::iterator bnd_itr = std::find_if( _hid_bindings.begin(),
                                                           _hid_bindings.end(),
                                                           HidBindingCompare( he->getInput() ) );
  if( bnd_itr == _hid_bindings.end() ) {
//    qDebug() << "  Input does not exist";
    return;
  }

  QSet<const HidAction*>::iterator act_itr = std::find_if( _hid_actions.begin(),
                                                           _hid_actions.end(),
                                                           HidActionCompareId( (*bnd_itr).getActionName() ) );
  if( act_itr == _hid_actions.end() ) {

//    qDebug() << "  Input is not mapped to any action";
    return;
  }

//  auto act_event_handler = const_cast<QObject*>((*act_itr)->getEventHandler());
//  if(act_event_handler) {
//    _ceq01.enqueue(event);
////    QCoreApplication::sendEvent(act_event_handler, event);
//    return;
//  }

  event->setAccepted(true);
  triggerAction( *act_itr, he->getParams() );
}

void HidManager::triggerAction(const HidAction* action, const HidInputEvent::HidInputParams& params) {

  emit signBeforeHidAction();
  action->trigger( params );
  emit signAfterHidAction();
}

QString HidManager::registerHidAction( const QString& group,
                                       const QString& name,
                                       const QString& description,
                                       const QObject *receiver, const char *method,
                                       unsigned int custom_trigger
                                       ) {

//  if( _hid_actions.contains( text ) )
//    return 0x0;


  QString identifier = QString("%1.%2").arg(group).arg(name);
//  qDebug() << "Registering HidAction: " << identifier;

  QSet<const HidAction*>::iterator item_itr =
      std::find_if( _hid_actions.begin(), _hid_actions.end(), HidActionCompareId( identifier ) );
  if( item_itr != _hid_actions.end() ) {
//    qDebug() << "  Does already exist";
    return QString();
  }
//  qDebug() << "  Does not exist";

  HidAction *act = new HidAction( group, name, description, custom_trigger );
  connect( act, SIGNAL(signTrigger(HidInputEvent::HidInputParams)), receiver, method, Qt::DirectConnection );

  _hid_actions.insert( act );

  _model->update(_hid_actions, _hid_bindings);

  return identifier;
}

bool HidManager::registerHidMapping(const QString& action_name, const HidInput *hid_input) {

  if( action_name.isEmpty() )
    return false;

  if( !hid_input )
    return false;

  HidActions::iterator act_itr =
      std::find_if( _hid_actions.begin(), _hid_actions.end(), HidActionCompareId(action_name) );
  if( act_itr == _hid_actions.end() )
    return false;

  HidBindings::iterator bnd_itr = std::find_if(_hid_bindings.begin(), _hid_bindings.end(), HidBindingCompare(action_name) );
  if( bnd_itr != _hid_bindings.end() )
    return false;

  _hid_bindings.insert(HidBinding(action_name,hid_input));

  _model->update(_hid_actions,_hid_bindings);

  return true;
}

HidManagerTreeModel *HidManager::getModel() const {

  return _model;
}

void HidManager::forceUpdate() {

  _model->update(_hid_actions,_hid_bindings);
}
