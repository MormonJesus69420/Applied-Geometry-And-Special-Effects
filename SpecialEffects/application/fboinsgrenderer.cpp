#include "fboinsgrenderer.h"

#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QOpenGLFunctions>

#include "gmlibwrapper.h"
#include "window.h"

#include <scene/gmrendertarget>


class QQuickFboInlineRenderTarget : public GMlib::RenderTarget {
public:
  QQuickFboInlineRenderTarget() { _gl.initializeOpenGLFunctions(); }
  GLint _fbo;

private:
  mutable QOpenGLFunctions _gl;

  void doPrepare()  const override {}
  void doBind()   const override { _gl.glBindFramebuffer(GL_FRAMEBUFFER,_fbo); }
  void doUnbind() const override { _gl.glBindFramebuffer(GL_FRAMEBUFFER,0x0); }
  void doResize()  override {}
};

class GMlibInFboRenderer : public QQuickFramebufferObject::Renderer {

public:
  GMlibInFboRenderer() { _gl.initializeOpenGLFunctions(); }

  void render() override {

    if(!_rcpair_name.length()) return;

    // Pick up the FBO set by the QQuickFrameBufferObject upon the render() call
    _gl.glGetIntegerv(GL_FRAMEBUFFER_BINDING,&_rt._fbo);

    // Prepare render and camera
    auto &gmlib = GMlibWrapper::instance();
    gmlib.render(_rcpair_name,QRect(QPoint(0,0),QSize(_size)),_rt);

    // Restore to QML's GLState;
    // we do not know what GMlib has done
    _item->window()->resetOpenGLState();

    // Not necessary, but for clarity let's restore the full GL state as we entered the render() method
    _gl.glBindFramebuffer(GL_FRAMEBUFFER,_rt._fbo);

    // Throttle
    update();
  }

  QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override {

    _size = size;

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
  }

  void synchronize(QQuickFramebufferObject *item) override {

    _item = static_cast<FboInSGRenderer*>(item);
    _rcpair_name = _item->rcPairName();
  }

  QOpenGLFunctions            _gl;
  FboInSGRenderer*            _item;
  Window*                     _window;
  QSize                       _size;
  QQuickFboInlineRenderTarget _rt;
  QString                     _rcpair_name;
};




FboInSGRenderer::FboInSGRenderer() {

  setAcceptedMouseButtons(Qt::AllButtons);
  setFocus(true,Qt::ActiveWindowFocusReason);

  setMirrorVertically(true);

  connect( this, &QQuickItem::windowChanged,
           this, &FboInSGRenderer::onWindowChanged );
}

const QString&
FboInSGRenderer::rcPairName() const { return _name; }

void
FboInSGRenderer::setRcPairName(const QString& name) {
  _name = name;
  update();
}

QQuickFramebufferObject::Renderer* FboInSGRenderer::createRenderer() const { return new GMlibInFboRenderer(); }

void FboInSGRenderer::onWindowChanged(QQuickWindow* w) {

  if(!w) return;

  auto window = static_cast<Window*>(w);

  connect( this,   &FboInSGRenderer::signKeyPressed,
           window, &Window::signKeyPressed );

  connect( this,   &FboInSGRenderer::signKeyReleased,
           window, &Window::signKeyReleased );

  connect( this,   &FboInSGRenderer::signMouseDoubleClicked,
           window, &Window::signMouseDoubleClicked );

  connect( this,   &FboInSGRenderer::signMouseMoved,
           window, &Window::signMouseMoved );

  connect( this,   &FboInSGRenderer::signMousePressed,
           window, &Window::signMousePressed );

  connect( this,   &FboInSGRenderer::signMouseReleased,
           window, &Window::signMouseReleased );

  connect( this,   &FboInSGRenderer::signWheelEventOccurred,
           window, &Window::signWheelEventOccurred );
}

void FboInSGRenderer::keyPressEvent(QKeyEvent* event)           { emit signKeyPressed(_name,event); }
void FboInSGRenderer::keyReleaseEvent(QKeyEvent* event)         { emit signKeyReleased(_name,event); }
void FboInSGRenderer::mousePressEvent(QMouseEvent* event)       { emit signMousePressed(_name,event); }
void FboInSGRenderer::mouseReleaseEvent(QMouseEvent* event)     { emit signMouseReleased(_name,event); }
void FboInSGRenderer::mouseDoubleClickEvent(QMouseEvent* event) { emit signMouseDoubleClicked(_name,event); }
void FboInSGRenderer::mouseMoveEvent(QMouseEvent* event)        { emit signMouseMoved(_name,event); }
void FboInSGRenderer::wheelEvent(QWheelEvent* event)            { emit signWheelEventOccurred(_name,event); }
