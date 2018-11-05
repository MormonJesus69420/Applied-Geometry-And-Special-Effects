#ifndef GM_SCENE_RENDER_RENDERTARGETS_TEXTURERENDERTARGET_H
#define GM_SCENE_RENDER_RENDERTARGETS_TEXTURERENDERTARGET_H


#include "../gmrendertarget.h"

// gmlib
#include <opengl/gmframebufferobject.h>
#include <opengl/gmtexture.h>

namespace GMlib {

class TextureRenderTarget : public RenderTarget {
public:
  explicit TextureRenderTarget ();
  explicit TextureRenderTarget ( const std::string& name );

  const GL::FramebufferObject&    getFbo() const;
  const GL::Texture&              getTexture() const;
  void                            setClearColor( const Color& c );

private:
  GL::FramebufferObject     _fbo;
  GL::Texture               _tex_rb_color;

  Color                     _clear_color;

  void      init( const std::string& name = std::string());

  // Virtual from RenderTarget
  void      doPrepare() const override;
  void      doBind() const override;
  void      doUnbind() const override;
  void      doResize() override;

}; // END class TextureRenderTarget

} // End namespace GMlib

#endif // GM_SCENE_RENDER_RENDERTARGETS_TEXTURERENDERTARGET_H
