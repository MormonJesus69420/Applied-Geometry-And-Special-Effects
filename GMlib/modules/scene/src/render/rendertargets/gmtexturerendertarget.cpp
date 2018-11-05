#include "gmtexturerendertarget.h"
using namespace GMlib;

TextureRenderTarget::TextureRenderTarget() {

  init();
}

TextureRenderTarget::TextureRenderTarget(const std::string& name) {

  init(name);
}

const GL::FramebufferObject&TextureRenderTarget::getFbo() const {

  return _fbo;
}

const GL::Texture& TextureRenderTarget::getTexture() const {

  return _tex_rb_color;
}

void TextureRenderTarget::setClearColor(const Color& c) {

  _clear_color = c;
}

void TextureRenderTarget::init(const std::string &name) {

  _fbo.create();

  if(name.length()) _tex_rb_color.create(name,GL_TEXTURE_2D);
  else              _tex_rb_color.create(GL_TEXTURE_2D);
  _tex_rb_color.texParameteri( GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  _tex_rb_color.texParameteri( GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  _tex_rb_color.texParameterf( GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  _tex_rb_color.texParameterf( GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  _fbo.attachTexture2D( _tex_rb_color, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 );

  _clear_color = Color( 127, 127, 127 );
}

void TextureRenderTarget::doPrepare() const {
  _fbo.clearColorBuffer(_clear_color);
  GL_CHECK(::glViewport(0, 0, getWidth(), getHeight()));
}

void TextureRenderTarget::doBind() const {

  _fbo.bind();
}

void TextureRenderTarget::doUnbind() const {

  _fbo.unbind();
}

void TextureRenderTarget::doResize() {

  _tex_rb_color.texImage2D( 0, GL_RGBA8, getWidth(), getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0x0 );
}
