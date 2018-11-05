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





#ifndef GM_SCENE_LIGHT_H
#define GM_SCENE_LIGHT_H

// gmlib
#include <core/utils/gmcolor.h>
#include <opengl/gmopengl.h>
#include <opengl/bufferobjects/gmuniformbufferobject.h>


namespace GMlib{

  class Camera;

  /*! \class Light gmlight.h <gmLight>
	 * \brief Pending Documentatioo
	 *
	 *	Pending Documentatioo
	 */
  class Light {
  public:
    Light();
    Light( const Color& amb , const Color& dif, const Color& spe );
    Light( const Light& );
    virtual ~Light();

    virtual void                  culling( const Camera& );
    bool                          isActive() const;
    bool                          isCullable();
    bool                          isEnabled() const;
    void                          setEnabled( bool state );
    void                          setColor(
                                    const Color& ambient = Color( 0.2f, 0.2f, 0.2f ),
                                    const Color& diffuse = Color( 1.0f, 1.0f, 1.0f ),
                                    const Color& specular= Color( 1.0f, 1.0f, 1.0f )
                                  );

    void                          setCullable( bool cullable );
    void                          setIntensity(double d,int i=0);


    Color                         getAmbient() const;
    Color                         getDiffuse() const;
    unsigned int                  getLightName() const;
    Color                         getSpecular() const;

  protected:
    static float                  _min_light_contribution; // 1/100
    bool                          _culled;

  private:
    static unsigned int           _next_light;
    unsigned int                  _light_name;

    Color                         _ambient;
    Color                         _diffuse;
    Color                         _specular;
    bool                          _cullable;

    Vector<double,3>              _intensity;
    bool                          _enabled;

  };	// END class Light










  /*! void Light::culling( const Frustum& )
   * \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Light::culling( const Camera& ) {}

  inline
  bool Light::isActive() const {

    return _enabled && !_culled;
  }

  /*! bool Light::isCullable()
   * \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool Light::isCullable() {

    return _cullable;
  }

  inline
  bool Light::isEnabled() const {

    return _enabled;
  }

  /*! void Light::setColor( const Color& ambient, const Color& diffuse, const Color& specular )
   * \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Light::setColor(
    const Color& ambient,
    const Color& diffuse,
    const Color& specular
  ) {

    _ambient		= Color( ambient );
    _diffuse		= Color( diffuse );
    _specular		= Color( specular );

    setIntensity(1.0);
  }


  /*! void Light::setCullable( bool cullable )
   * \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Light::setCullable( bool cullable ) {

    _cullable = cullable;
  }

  /*! void Light::setIntensity(double d,int i)
   * \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Light::setIntensity(double d,int i) {

    if(i==0 || i == 1)
      _intensity[0] = d;
    if(i==0 || i == 2)
      _intensity[1] = d;
    if(i==0 || i == 3)
      _intensity[2] = d;
  }

  /*! const Color& Light::getAmbient()
   * \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  Color Light::getAmbient() const {

    return _ambient * _intensity(0);
  }


  /*! const Color& Light::getDiffuse()
   * \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  Color Light::getDiffuse() const {

    return  _diffuse * _intensity(1);
  }


  /*! unsigned int Light::getLightName()
   * \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  unsigned int Light::getLightName() const {

    return _light_name;
  }


  /*! const Color& Light::getSpecular()
   * \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  Color Light::getSpecular() const	{

    return _specular * _intensity(2);
  }


}	// END namespace GMlib


#endif // GM_SCENE_LIGHT_H
