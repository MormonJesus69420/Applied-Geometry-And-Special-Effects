#include "window.h"

//local
#include "fboinsgrenderer.h"

// qt
#include "QOpenGLContext"

Window::Window(QWindow *parent) : QQuickView(parent) {

  qmlRegisterType<FboInSGRenderer>("SceneGraphRendering", 1, 0, "Renderer");


  setSurfaceType(QSurface::OpenGLSurface);
  setPersistentOpenGLContext(true);
  setPersistentSceneGraph(true);

  setResizeMode(SizeRootObjectToView);
  setMinimumSize( QSize( 800, 600 ) );


  QSurfaceFormat format;
  if(QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {

    format.setVersion(4,0);                                   // GMlib is compatible with OpenGL >= 3.3
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setOption(QSurfaceFormat::DeprecatedFunctions);

    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSwapInterval(1);                                // 0 = vsync off
  }
  format.setDepthBufferSize(24);
  format.setRedBufferSize(8);
  format.setGreenBufferSize(8);
  format.setBlueBufferSize(8);
  format.setAlphaBufferSize(8);
  format.setSamples(4);
  format.setStencilBufferSize(8);

  QSurfaceFormat::setDefaultFormat(format);
  setFormat(format);

  create();
}
