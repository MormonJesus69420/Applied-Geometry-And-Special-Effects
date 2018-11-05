#ifndef GUIAPPLICATION_H
#define GUIAPPLICATION_H


#include "gmlibwrapper.h"
#include "window.h"
#include "../scenario.h"
#include "../hidmanager/defaulthidmanager.h"

class GLContextSurfaceWrapper;

// gmlib
namespace GMlib {
  class Scene;
}

// qt
#include <QGuiApplication>

// stl
#include <memory>




class GuiApplication : public QGuiApplication {
  Q_OBJECT
public:
  explicit GuiApplication(int& argc, char** argv);
  ~GuiApplication();

private:
  Window                                      _window;
  Scenario                                    _scenario;
  DefaultHidManager                           _hidmanager;

private slots:
  virtual void                                onSceneGraphInitialized();
  virtual void                                onSceneGraphInvalidated();
  virtual void                                afterOnSceneGraphInitialized();

signals:
  void                                        signOnSceneGraphInitializedDone();


private:
  static std::unique_ptr<GuiApplication>      _instance;
public:
  static const GuiApplication&                instance();
};

#endif // GUIAPPLICATION_H
