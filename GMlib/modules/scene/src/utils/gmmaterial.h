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




#ifndef GM_SCENE_UTILS_MATERIAL_H
#define GM_SCENE_UTILS_MATERIAL_H



// gmlib
#include <core/utils/gmstring.h>
#include <core/containers/gmarray.h>
#include <opengl/gmopengl.h>


namespace GMlib {



  class Material;
  class MaterialObject;
  class MaterialObjectList;



  /*! \class Material gmmaterial.h <gmMaterial>
   *  \brief The Material class
   *
   *  containing:
   *    ambient, diffuce and specular colors,
   *    shininess (0-100) and
   *    texture ID and
   *    source and destination blending factors
   */
  class Material {
  public:
    Material(
      const Color& amb   = Color( 0.7f, 0.0f, 0.0f, 1.0f ),
      const Color& dif   = Color( 0.7f, 0.0f, 0.0f, 1.0f ),
      const Color& spc   = Color( 0.7f, 0.0f, 0.0f, 1.0f ),
      float shininess     = 0.0f
    );
    Material( const Material& copy );
    ~Material();

    const Color&    getAmb() const;
    const Color&    getDif() const;
    const Color&    getSpc() const;
    float           getShininess() const;
    bool            isTransparent() const;
    void            set( const Color& amb, const Color& dif, const Color& spc, float shininess/*, const Texture& texture */);
    void            set( const Color& amb, const Color& dif, const Color& spc );
    void            set( const GLenum sfactor, const GLenum dfactor );
    void            set( float shininess );
    void            setAmb( const Color& amb );
    void            setDif( const Color& dif );
    void            setDoubleSided( bool s );
    void            setSided( GLenum s );
    void            setSpc( const Color& spc );
    void            setTransparancy( double t );

    Material&       operator =  ( const Material& m );
    bool            operator == ( const Material& m ) const;
    bool            operator <  ( const Material& m ) const;


  protected:
    Color           _amb;
    Color           _dif;
    Color           _spc;
    float           _shininess;


  private:
    GLenum          _source_blend_factor;
    GLenum          _destination_blend_factor;
    GLenum          _sided;


  // *****************************
    // IOSTREAM overloaded operators

  #ifdef GM_STREAM

  public:
    template <class T_Stream>
    friend T_Stream& operator << ( T_Stream& out, const Material& m ) {

      out << m._amb       << GMseparator::element()
          << m._dif       << GMseparator::element()
          << m._spc       << GMseparator::element()
          << m._shininess << GMseparator::element();

      return out;
    }

    template <class T_Stream>
    friend T_Stream& operator >> ( T_Stream& in, Material& m ) {

      static Separator es(GMseparator::element());
      static Separator gs(GMseparator::group());
      char fn[255];  // think about this

      in  >> m._amb       >> es
          >> m._dif       >> es
          >> m._spc       >> es
          >> m._shininess >> es
          >> fn           >> gs;

      m.set( GL_ONE, GL_ONE );
//      delete fn;
      return in;
    }

    #endif

  }; // END class Material


  // ********************
  // Predefined Materials

  namespace GMmaterial {


    const Material& blackPlastic();
    const Material& blackRubber();
    const Material& brass();
    const Material& bronze();
    const Material& chrome();
    const Material& copper();
    const Material& emerald();
    const Material& gold();
    const Material& jade();
    const Material& obsidian();
    const Material& pearl();
    const Material& pewter();
    const Material& plastic();
    const Material& polishedBronze();
    const Material& polishedCopper();
    const Material& polishedGold();
    const Material& polishedGreen();
    const Material& polishedRed();
    const Material& polishedSilver();
    const Material& ruby();
    const Material& sapphire();
    const Material& silver();
    const Material& snow();
    const Material& turquoise();

  } // END namespace GMlib::Material







  /*! \class MaterialObject gmmaterial.h <gmMaterial>
   *  \brief The MaterialObject class
   *
   *  containing:
   *    A name and
   *    is a Materia
   */
  class MaterialObject: public Material {
  public:
    MaterialObject(
      const Material& mat = GMmaterial::snow(),
      const char* name = "Snow"
    );

    MaterialObject(
      const Material& mat,
      std::string name
    );

    MaterialObject(  const MaterialObject& m);

    const Material&    getMaterial() const;
    const String&      getName() const;
    const char*        getNameC() const;
    bool               is(const char* name) const;
    bool               is(const std::string& name) const;
    bool               is(const Material& m) const;
    void               setMaterial(const Material& m);
    void               setName(const std::string& name);
    void               setName(const char* name);

    MaterialObject&    operator=(const MaterialObject& m);


  private:
    String    _name;      // Size of name is max 16 letters

  }; // END MaterialObject class








  /*! \class MaterialObjectList gmmaterial.h <gmMaterial>
   *  \brief List of materialObjects class
   *
   *  containing:
   *    List of materialObjects
   */
  class MaterialObjectList : public Array<MaterialObject> {
  public:
    MaterialObjectList(bool init=true);
    MaterialObjectList(char* file_name);

    void  initPreDef();
    bool  readFromFile(char* file_name);
    bool  storeToFile(char* file_name);

  }; // END class MaterialObjectList












  /*! Material::Material(  const Color& amb, const Color& dif, const Color& spc, float shininess, const Texture& texture )
   *  \brief Pending Documentation
   *
   *  Default/Standar constructor
   */
  inline
  Material::Material(  const Color& amb, const Color& dif, const Color& spc, float shininess )  {

    set( amb, dif, spc, shininess );
    set(GL_ONE,GL_ONE);
    _sided = GL_FRONT_AND_BACK;
  }


  /*! Material::Material( const Material&  m )
   *  \brief Pending Documentation
   *
   *  Copy constructor
   */
  inline
  Material::Material( const Material&  copy ) {

    _amb = copy._amb;
    _dif = copy._dif;
    _spc = copy._spc;
    _shininess = copy._shininess;

    _source_blend_factor = copy._source_blend_factor;
    _destination_blend_factor = copy._destination_blend_factor;
    _sided = copy._sided;
  }


  /*! Material::~Material()
   *  \brief Pending Documentations
   *
   *  Pending Documentation
   */
  inline
  Material::~Material() {}


  inline
  const Color& Material::getAmb() const {

    return _amb;
  }


  inline
  const Color& Material::getDif() const {

    return _dif;
  }


  inline
  const Color& Material::getSpc() const {

    return _spc;
  }

  inline
  float Material::getShininess() const {

    return _shininess;
  }


  /*! bool Material::isTransparent()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool Material::isTransparent() const {

      return ( (_amb.getAlpha() < 1.0) && (_dif.getAlpha() < 1.0) && (_spc.getAlpha() < 1.0) );
  }


  /*! void Material::set( const Color& amb, const Color& dif, const Color& spc, float shininess, const Texture& texture )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Material::set( const Color& amb, const Color& dif, const Color& spc, float shininess ) {

    _amb = amb;
    _dif = dif;
    _spc = spc;
    _shininess = shininess;
  }


  /*! void Material::set(const Color& amb, const Color& dif, const Color& spc )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Material::set(const Color& amb, const Color& dif, const Color& spc ) {

    _amb = amb;
    _dif = dif;
    _spc = spc;
  }


  /*! void Material::setAmb(const Color& amb)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Material::setAmb(const Color& amb)  {

    _amb = amb;
  }


  /*! void Material::setDif(const Color& dif)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Material::setDif(const Color& dif)  {

    _dif = dif;
  }


  /*! void Material::setDoubleSided(bool s)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Material::setDoubleSided(bool s)  {

    if(s) _sided = GL_FRONT_AND_BACK;
    else _sided = GL_FRONT;
  }


  /*! void Material::setSided(GLenum s)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Material::setSided(GLenum s) {

    _sided = s;
  }


  /*! void Material::setSpc(const Color& spc)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Material::setSpc(const Color& spc)  {

    _spc=spc;
  }


  /*! void Material::setTransparancy(double t)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void Material::setTransparancy(double t) {

    _amb.setAlpha( t );
    _dif.setAlpha( t );
    _spc.setAlpha( t );
  }


  /*! Material& Material::operator=(const Material& m)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   *  Makes a copy
   */
  inline
  Material& Material::operator=(const Material& m) {

    memcpy(this,&m,sizeof(Material));
    return *this;
  }


  /*! bool Material::operator==(const Material& m) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool Material::operator==(const Material& m) const {

    return _amb == m._amb && _dif == m._dif && _spc == m._spc
           && GMutils::compValueF(_shininess, m._shininess);
    /* && _texture == m._texture*/
  }

  /*! MaterialObject::MaterialObject(  const Material& mat, const char* name )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  MaterialObject::MaterialObject(  const Material& mat, const char* name ) : Material(mat)  {

    _name = name;
  }


  /*! MaterialObject::MaterialObject(  const Material& mat, string name )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  MaterialObject::MaterialObject(  const Material& mat, std::string name ) : Material(mat)  {

    _name = name;
  }


  /*! MaterialObject::MaterialObject(  const MaterialObject& m)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  MaterialObject::MaterialObject(  const MaterialObject& m) : Material(m) {

    _name = m._name;
  }


  /*! const Material&    MaterialObject::getMaterial() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  const Material&    MaterialObject::getMaterial() const {

    return *this;
  }


  /*! const GM_String&  MaterialObject::getName() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  const String&  MaterialObject::getName() const {

    return _name;
  }


  /*! const char* MaterialObject::getNameC() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  const char* MaterialObject::getNameC() const {

    return _name.c_str();
  }


  /*! bool MaterialObject::is(const char* name) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool MaterialObject::is(const char* name) const {

    return _name == name;
  }


  /*! bool MaterialObject::is(const string& name) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool MaterialObject::is(const std::string& name) const {

    return _name == name;
  }


  /*! bool MaterialObject::is(const Material& m) const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool MaterialObject::is(const Material& m) const {

    return m == Material(*this);
  }


  /*! void MaterialObject::setMaterial(const Material& m)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void MaterialObject::setMaterial(const Material& m)  {

    Material::operator = ( m );
  }

  /*! void MaterialObject::setName(const string& name)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void MaterialObject::setName(const std::string& name) {

    _name = name;
  }

  /*! void MaterialObject::setName(const char* name)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void MaterialObject::setName(const char* name) {

    _name = name;
  }


  /*! MaterialObject& MaterialObject::operator=(const MaterialObject& m)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  MaterialObject& MaterialObject::operator=(const MaterialObject& m) {

    Material::operator = ( m );

    _name         = m._name;

    return *this;
  }

  /*! MaterialObjectList::MaterialObjectList(bool init)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  MaterialObjectList::MaterialObjectList(bool init) {

    if(init)
      initPreDef();
  }


  /*! MaterialObjectList::MaterialObjectList(char* file_name)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  MaterialObjectList::MaterialObjectList( char* file_name ) {

    if(file_name) readFromFile(file_name);
  }

} // END namespace GMlib


#endif // GM_SCENE_UTILS_MATERIAL_H
