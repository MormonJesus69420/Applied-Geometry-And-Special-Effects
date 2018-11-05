#ifndef GM_SCENE_RENDER_RENDERTARGETS_NATIVERENDERTARGETS_H
#define GM_SCENE_RENDER_RENDERTARGETS_NATIVERENDERTARGETS_H


#include "../gmrendertarget.h"


namespace GMlib {

  // PassThroughTarget
class NativeRenderTarget : public RenderTarget {
public:
  explicit NativeRenderTarget() {}

private:
  // Virtual from RenderTarget
  void    doPrepare() const override {}
  void    doBind() const override {}
  void    doUnbind() const override {}
  void    doResize() override {}

}; // END class NativeRenderTarget

} // End namespace GMlib

#endif // GM_SCENE_RENDER_RENDERTARGETS_NATIVERENDERTARGETS_H
