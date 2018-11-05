#ifndef GM_SCENE_RENDER_RENDERTARGET_H
#define GM_SCENE_RENDER_RENDERTARGET_H


// gmlib
#include <core/types/gmpoint.h>


namespace GMlib {

class RenderTarget {
public:
  RenderTarget() : _size() {}
  virtual ~RenderTarget() {}

  void                              prepare() const;
  void                              bind() const;
  void                              unbind() const;

  const Vector<int,2>&              getSize() const;
  int                               getWidth() const;
  int                               getHeight() const;
  void                              resize( const Vector<int,2>& size );

private:
  Vector<int,2>                     _size;

  virtual void                      doPrepare() const = 0;
  virtual void                      doBind() const = 0;
  virtual void                      doUnbind() const = 0;
  virtual void                      doResize() = 0;
}; // END class RenderTarget





inline
void RenderTarget::prepare() const { doPrepare(); }

inline
void RenderTarget::bind() const { doBind(); }

inline
void RenderTarget::unbind() const { doUnbind(); }

inline
const Vector<int,2>&RenderTarget::getSize() const { return _size; }

inline
int RenderTarget::getWidth() const { return _size(0); }

inline
int RenderTarget::getHeight() const { return _size(1); }

inline
void RenderTarget::resize(const Vector<int,2>& size)  { _size = size; doResize(); }


} // End namespace GMlib

#endif // GM_SCENE_RENDER_RENDERTARGET_H
