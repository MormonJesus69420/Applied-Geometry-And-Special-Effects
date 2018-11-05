/**********************************************************************************
**
** Copyright (C) 1994 Narvik University College
** Contact: GMlib Online Portal at http://episteme.hin.no
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/




#ifndef GM_SCENE_RENDER_DEFAULTRENDERER_H
#define GM_SCENE_RENDER_DEFAULTRENDERER_H


#include "gmrenderer.h"

// local
#include "../camera/gmcamera.h"
#include "../visualizers/gmstdrepvisualizer.h"
#include "../visualizers/gmcoordsysrepvisualizer.h"
//#include "../window/gmviewset.h"
//#include "../window/gmwindow.h"
#include "gmrendertarget.h"

// gmlib
#include <opengl/gmframebufferobject.h>
#include <opengl/gmrenderbufferobject.h>
#include <opengl/gmtexture.h>
#include <opengl/bufferobjects/gmvertexbufferobject.h>
//#include <scene/render/rendertargets/gmtexturerendertarget.h>
//#include <scene/render/rendertargets/gmnativerendertarget.h>


namespace GMlib {



  class RenderTarget;



  class DefaultRenderer : public Renderer {
  public:
    explicit DefaultRenderer();
    virtual ~DefaultRenderer();

    const GL::Texture&      getRenderTexture() const;
    const GL::Texture&      getSelectTexture() const;

    const Color&            getClearColor() const;
    void                    setClearColor(const Color &color);

    const Color&            getSelectColor() const;
    void                    setSelectColor( const Color& color );


    const Vector<int,2>&    getViewportSize() const;

    void                    render(RenderTarget& target);


    const RenderTarget&      getFrontRenderTarget() const;
    const RenderTarget&      getBackRenderTarget() const;

    const GL::UniformBufferObject&    getDirectionalLightUBO() const;
    const GL::UniformBufferObject&    getPointLightUBO() const;
    const GL::UniformBufferObject&    getSpotLightUBO() const;

    /* virtual from Renderer */
    void                    prepare() override {}
    void                    render()override ;
    void                    swap() override ;
    void                    reshape( const Vector<int,2>& size ) override ;

  protected:
    virtual void            prepare(Camera *cam);

    mutable Array<const SceneObject*>    _objs;


  private:
    /* Object rendering */
    GL::Program             _render_prog;
    GL::FramebufferObject   _fbo;
    GL::Texture             _rbo_color;
    GL::Texture             _rbo_depth;

    /* Selection rendering */
    GL::Program             _render_select_prog;
    GL::FramebufferObject   _fbo_select;
    GL::Texture             _rbo_select;

    GL::FramebufferObject   _fbo_select_depth;
    GL::Texture             _rbo_select_depth;

    Color                   _clear_color;
    Color                   _select_color;

    /* other suff */
    HqMatrix<float,3>       _ortho_mat;

    GL::VertexBufferObject  _quad_vbo;

    void                    render(const SceneObject *obj) const;
    void                    renderSelectedGeometry(const SceneObject *obj) const;
    void                    renderCoordSys() const;


    CoordSysRepVisualizer   *_coord_sys_visu;

    void                    initRenderProgram();
    void                    initRenderSelectProgram();



    void                    renderScene();
    void                    renderToTarget();


    Vector<int,2>           _size;
    RenderTarget     *_front_rt;
    RenderTarget     *_back_rt;

    GL::UniformBufferObject           _dirlight_ubo;
    GL::UniformBufferObject           _pointlight_ubo;
    GL::UniformBufferObject           _spotlight_ubo;
    void                              updateLightUBO();




  }; // END class DisplayRenderer













} // END namespace GMlib

#endif // GM_SCENE_RENDER_DEFAULTRENDERER_H
