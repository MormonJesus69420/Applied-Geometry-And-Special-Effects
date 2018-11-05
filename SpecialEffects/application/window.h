#ifndef WINDOW_H
#define WINDOW_H



// qt
#include <QQuickView>

// stl
#include <memory>

class Window : public QQuickView {
  Q_OBJECT
public:
  explicit Window(QWindow *parent = 0);

signals:
  void      signRcPairActiveStateChanged( const QString& name, bool state );
  void      signRcPairViewportChanged( const QString& name, const QRectF& geometry );
  void      signFrameReady();

  // Relay singals from qml side
  void      signKeyPressed( const QString& name, QKeyEvent* event );
  void      signKeyReleased( const QString& name, QKeyEvent* event );
  void      signMouseDoubleClicked( const QString& name, QMouseEvent* event );
  void      signMouseMoved( const QString& name, QMouseEvent* event );
  void      signMousePressed( const QString& name, QMouseEvent* event );
  void      signMouseReleased( const QString& name, QMouseEvent* event );
  void      signWheelEventOccurred( const QString& name, QWheelEvent* event );
};

#endif // WINDOW_H
