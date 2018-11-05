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




#ifndef GM_SCENE_SCENEOBJECT_H
#define GM_SCENE_SCENEOBJECT_H



#include "gmscaleobject.h"
#include "gmscene.h"
#include "event/gmsceneobjectattribute.h"
#include "utils/gmmaterial.h"


// gmlib
#include <opengl/gmprogram.h>

// stl
#include <string>





/////////////////////

// getIdentity
#define GM_DECLARE_SO_IDENTITY( CNAME ) \
  std::string getIdentity() const override { \
    return #CNAME; \
  }

// makeCopy
#define GM_DECLARE_SO_MAKECOPY( CNAME ) \
  GMlib::SceneObject* makeCopy() override { \
    return new CNAME(*this); \
  }

#define GM_DECLARE_SO_MAKECOPY_NULL() \
  GMlib::SceneObject* makeCopy() override { \
    return 0x0; \
  }


// SceneObject macros
#define GM_SCENEOBJECT( CNAME ) \
  public: \
    GM_DECLARE_SO_IDENTITY( CNAME ) \
    GM_DECLARE_SO_MAKECOPY( CNAME ) \
  private:

#define GM_SCENEOBJECT_NULL( CNAME ) \
  public: \
    GM_DECLARE_SO_IDENTITY( CNAME ) \
    GM_DECLARE_SO_MAKECOPY_NULL() \
  private:

//////////////////





namespace GMlib{


  class Camera;
  class Renderer;
  class DefaultRenderer;
  class Visualizer;


  enum GM_SO_TYPE {

    // Basic Types
    GM_SO_TYPE_SCENEOBJECT                    = 0x0001,
    GM_SO_TYPE_CAMERA                         = 0x0002,
    GM_SO_TYPE_LIGHT                          = 0x0003,
    GM_SO_TYPE_SELECTOR                       = 0x0050,
    GM_SO_TYPE_SELECTOR_GRID                  = 0x0051,

    // Point
    GM_SO_TYPE_POINT                          = 0x0200,

    // Curves
    GM_SO_TYPE_CURVE                          = 0x1000,
    GM_SO_TYPE_CURVE_BEZIER                   = 0x1001,
    GM_SO_TYPE_CURVE_BSPLINE                  = 0x1002,
    GM_SO_TYPE_CURVE_ERBS                     = 0x1003,

    // Surfaces
    GM_SO_TYPE_SURFACE                        = 0x2000,
    GM_SO_TYPE_SURFACE_BEZIER                 = 0x2001,
    GM_SO_TYPE_SURFACE_BSPLINE                = 0x2002,
    GM_SO_TYPE_SURFACE_ERBS                   = 0x2003,

    // Volumes
    GM_SO_TYPE_VOLUME                         = 0x3000,

    // Flows
    GM_SO_TYPE_FLOW                           = 0x4000
  };







  /*! \class SceneObject gmsceneobject.h <gmSceneObject>
   *  \brief SceneObject is the base class for anything that steps onto a Scene
   *
   *   It contains as private, a unique name for indentification used in Select(), and an array containg all its children.
   *
   *   Further, as protected members, two matrices for local transformation of the object, and a SurroundingSphere that always should be made.
   *
   *
   *        Det er to virtuelle funksjoner localDisplay og localSelect
   *          som alle avledede klasser må ha sin versjon av.
   *         Arbeidsdelingen er slik at display og select foretar
   *           transformasjonen lagret i matrix, og displayer alle
   *           sub-objektene til objektet. Men etter transformasjonen
   *           og før sub-objektene displayes kalles localDisplay
   *           (eventuelt localSelect). I localDisplay kan en så
   *           displaye det som ikke displayes i sub-objektene og/eller
   *           foreta transformasjoner av sub-objekter for
   *           å simulere bevegelser.
   *         Det er også en insertSubObject-funksjon som avledede klasser
   *             kan bruke for å lage sub-objekt. Hvis en avledet klasse
   *           trenger tilgang til et objekt for bevegelsessimulering
   *           bør den lage en egen peker for det.
   *     public:
   *         Konstuktør med mulige default verdier og sletter
   *         En display funksjon for displaying av objektet.
   *         En select funksjon for plukking(select) av objektet med mus.
   *         En simulate funksjon for at objektet kan bevege subobjekter.
   *         En find funksjon for å finne en funksjon med et gitt navn.
   *         En rotate, scale og translate funksjon for å
   *            posisjonere objektet relativt i forhold til morsobjektet
   */
  class SceneObject {
  public:

    mutable const SceneObject                 *_copy_of;    //! Internal variable for use when coping the object.

//    SceneObject(
//      const Vector<float,3>& trans  = Vector<float,3>(0,0,0),
//      const Point<float,3>&  scale  = Point<float,3>(1,1,1),
//      const Vector<float,3>& rotate = Vector<float,3>(1,0,0),
//      Angle a=0);

    SceneObject(
      const Point<float,3>&  pos = Point<float,3>(0,0,0),
      const Vector<float,3>& dir = Vector<float,3>(1,0,0),
      const Vector<float,3>& up  = Vector<float,3>(0,0,1)
    );

    SceneObject(
      const Point<float,3>&  lock_pos,
      const Point<float,3>&  pos = Point<float,3>(0,0,0),
      const Vector<float,3>& up  = Vector<float,3>(0,0,1)
    );

    SceneObject(
      SceneObject* lock_object,
      const Point<float,3>&  pos = Point<float,3>(0,0,0),
      const Vector<float,3>& up  = Vector<float,3>(0,0,1)
    );

    SceneObject( const SceneObject& d );
    virtual ~SceneObject();

    virtual SceneObject*                makeCopy() = 0;
    virtual std::string                 getIdentity() const = 0;

    Scene*                              getScene();
    const Scene*                        getScene() const;

    int                                 getTypeId() const;
    unsigned int                        getName() const;
    virtual unsigned int                getVirtualName() const;
    virtual int                         getNumber() const { return -1;}

    void                                insert(SceneObject* obj);
    void                                remove(SceneObject* obj);
    bool                                isPart() const;
    void                                setIsPart( bool part );

    Array<SceneObject*>&                getChildren();
    const Array<SceneObject*>&          getChildren() const;
    SceneObject*                        getParent() const;
    void                                setParent(SceneObject* obj);
    SceneObject*                        getDerived() const;
    void                                setDerived(SceneObject* obj);

    const SceneObject*                  find(unsigned int name) const;
    SceneObject*                        find(unsigned int name);

    const Point<float,3>&               getCenterPos() const;

    ArrayT<SceneObjectAttribute*>&      accessSceneObjectAttributes();

    // Matrix/orientation
    virtual const HqMatrix<float,3>&    getMatrix() const;
    virtual HqMatrix<float,3>&          getMatrix();
    const HqMatrix<float,3>&            getMatrixGlobal() const;
    const HqMatrix<float,3>&            getMatrixParentGlobal() const;
    void                                setMatrix( const HqMatrix<float,3>& mat );

    const HqMatrix<float,3>&            getMatrixToScene() const;
    const HqMatrix<float,3>&            getMatrixToSceneInverse() const;


    const HqMatrix<float,3>&            getModelViewMatrix( const Camera* cam, bool local_cs = true ) const;
    const HqMatrix<float,3>&            getModelViewProjectionMatrix( const Camera* cam, bool local_cs = true ) const;
    const HqMatrix<float,3>&            getProjectionMatrix( const Camera* cam ) const;
    const SqMatrix<float,3>&            getNormalMatrix( const Camera* cam ) const;



    const Vector<float,3>&              getDir() const;
    const Vector<float,3>&              getSide() const;
    const Vector<float,3>&              getUp() const;
    const Point<float,3>&               getPos() const;

    Vector<float,3>                     getGlobalDir() const;
    Vector<float,3>                     getGlobalSide() const;
    Vector<float,3>                     getGlobalUp() const;
    Point<float,3>                      getGlobalPos() const;

    void                                set( const Point<float,3>&  pos,
                                             const Vector<float,3>& dir,
                                             const Vector<float,3>& up );


    const Point<float,3>&               getLockPos() const;
    double                              getLockDist() const;
    bool                                isLocked() const;
    virtual void                        lock(SceneObject* obj);
    virtual void                        lock(const Point<float,3>& pos);
    virtual void                        lock(double d);
    void                                unLock();


    // surrounding sphere
    const Sphere<float,3>&              getSurroundingSphere() const;
    const Sphere<float,3>&              getSurroundingSphereClean() const;

    // editing/interaction
    virtual void                        edit(int selector_id);
    virtual void                        edit(int selector_id, const Vector<float,3>& delta);
    virtual void                        edit(int selector_id, const Vector<double,3>& delta);
    virtual void                        edit(SceneObject* lp);
    virtual void                        edit();
    virtual void                        editPos(Vector<float,3> delta);
    virtual void                        enableChildren( bool enable = true );

    void                                setEditDone(bool val=true) const { _edit_done = val; }
    bool                                getEditDone() const { return _edit_done; }
    void                                getEditedObjects(Array<const SceneObject*>& e_obj) const;

    // properties
    bool                                isSelected() const;
    bool                                toggleSelected();
    virtual void                        selectEvent(int selector_id);
    virtual void                        setSelected(bool s);

    virtual bool                        isVisible() const;
    virtual void                        setVisible( bool v, int prop = 0 );
    virtual bool                        toggleVisible();

    bool                                isScaled() { return _scale.isActive(); }

    // transformation
    virtual void                        rotate(Angle a, const Vector<float,3>& rot_axel, bool propagate = true );
    virtual void                        rotate(Angle a, const Point<float,3>& p,const UnitVector<float,3>& d, bool propagate = true );
    virtual void                        rotate(const UnitQuaternion<float>& q, bool propagate = true  );
    virtual void                        rotateParent(Angle a, const Vector<float,3>& rot_axel, bool propagate = true );
    virtual void                        rotateParent(Angle a, const Point<float,3>& p,const UnitVector<float,3>& d, bool propagate = true );
    virtual void                        rotateParent(const UnitQuaternion<float>& q, bool propagate = true  );
    virtual void                        rotateGlobal(Angle a, const Vector<float,3>& rot_axel, bool propagate = true );
    virtual void                        rotateGlobal(Angle a, const Point<float,3>& p,const UnitVector<float,3>& d, bool propagate = true );
    virtual void                        rotateGlobal(const UnitQuaternion<float>& q, bool propagate = true  );
    virtual void                        scale(const Point<float,3>& scale_factor, bool propagate = true );
    virtual void                        translate(const Vector<float,3>& trans_vector, bool propagate = true );
    virtual void                        translateParent(const Vector<float,3>& trans_vector, bool propagate = true );
    virtual void                        translateGlobal(const Vector<float,3>& trans_vector, bool propagate = true );

    virtual void                        move(float d, bool propagate = true);
    virtual void                        move(const Vector<float,3>& t, bool propagate = true);
    virtual void                        move(char,double, bool propagate = true);
    virtual void                        move(const Vector<float,2>& t, bool propagate = true);
    virtual void                        roll(Angle a, bool propagate = true);
    virtual void                        tilt(Angle a, bool propagate = true);
    virtual void                        turn(Angle a, bool propagate = true);



    const Color&                        getColor() const;
    Color&                              getColor();
    void                                setColor( const Color& c );
    const Material&                     getMaterial() const;
    Material&                           getMaterial();
    virtual void                        setMaterial(const Material& m);

    bool                                isCollapsed() const;
    virtual void                        setCollapsed(bool c);
    virtual bool                        toggleCollapsed();
    virtual void                        toggleClose() {}
    virtual void                        toggleSelectors() {}

    bool                                isLighted() const;
    void                                setLighted( bool lighted );

    bool                                isOpaque() const;
    void                                setOpaque( bool o );


    Array<Visualizer*>&                 getVisualizers();
    const Array<Visualizer*>&           getVisualizers() const;
    virtual void                        insertVisualizer( Visualizer* visualizer );
    virtual void                        removeVisualizer( Visualizer* visualizer );
    virtual void                        replot() const;

    virtual void                        simulate( double dt );

    void                                getRenderList( Array<const SceneObject*>&, const Camera& ) const;
    void                                getRenderList( Array<const SceneObject*>& ) const;




    // deprecated
    virtual void                        localDisplay( const DefaultRenderer* ) const {}
      //!< Lingering function convenient for Rapid Prototyping
      //!< (may be removed without further notice!!!)
      //!< \deprecated

    virtual void                        localSelect( const Renderer*, const Color& ) const {}
      //!< Lingering function convenient for Rapid Prototyping
      //!< (may be removed without further notice!!!)
      //!< \deprecated

  protected:
    HqMatrix<float,3>                   _matrix;                //!< The difference matrix from mother to this.
    HqMatrix<float,3>                   _present;               //!< The difference matrix from global to this.
    HqMatrix<float,3>                   _matrix_scene;          //!< Matrix from this to scene
    HqMatrix<float,3>                   _matrix_scene_inv;      //!< Matrix from scene to this
    ScaleObject                         _scale;                 //!< The scaling for this and the children.
    bool                                _local_cs;              //!< Using local coordinate system, default is true

    Point<float,3>                      _pos;
    UnitVector<float,3>                 _dir;
    UnitVector<float,3>                 _side;
    UnitVector<float,3>                 _up;

    Point<float,3>                      _lock_pos;
    SceneObject*                        _lock_object;
    bool                                _locked;

    bool                                _collapsed;
    Material                            _material;
    Color                               _color;
    bool                                _lighted;
    bool                                _opaque;

    Array<Visualizer*>                  _visualizers;

    Scene*                              _scene;                 //!< The scene of the display hiearchy
    SceneObject*                        _parent;                //!< the mother in the hierarchy (tree).
    SceneObject*                        _derived;               //!< _derived is derived from this object.
    int                                 _type_id;
    Array<SceneObject*>                 _children;
    bool                                _is_part;               //! true if the object is seen as a part of a larger object




    Sphere<float,3>                     _global_sphere;         //!< for this object
    Sphere<float,3>                     _global_total_sphere;   //!< included all children

    bool                                _selected;
    bool                                _visible;               //!< culling on invisible items
    mutable bool                        _edit_done;             //!< message that the object need to be replotted
    mutable bool                        _is_editable;           //!< This object is not editable

    ArrayT<SceneObjectAttribute*>       _scene_object_attributes;


    void                                reset();

    void                                setSurroundingSphere( const Sphere<float,3>& b ) const;
    void                                updateSurroundingSphere( const Point<float,3>& p );

    Point<float,3>                      getSceneLockPos();
    void                                updateOrientation(const Point<float,3>& lock_at_p);

  protected:
    static unsigned int                 _free_name;             //!< For automatisk name-generations.
    unsigned int                        _name;                  //!< Unic name for this object, used for selecting
    mutable Sphere<float,3>             _sphere;                //!< Surrounding sphere for this object

    virtual void                        basisChange( const Vector<float,3>& dir,
                                                     const Vector<float,3>& side,
                                                     const Vector<float,3>& up,
                                                     const Vector<float,3>& pos);
    virtual void                        localSimulate(double dt);



  friend void Scene::prepare();
  int                                   prepare(Array<HqMatrix<float,3> >& mat, Scene* s, SceneObject* mother = 0);

  private:
  void                                  _init( const Point<float,3>&  pos, const Vector<float,3>& dir, const Vector<float,3>& up);

  // *****************************
  // IOSTREAM overloaded operators

  #ifdef GM_STREAM

  public:

    template <typename T_Stream>
    SceneObject( T_Stream& in, int /*st*/ ) {

      in >> *this;

      _name       = _free_name++;
      _local_cs   = true;
      _visible    = true;

      prIn(in);

      // init begin
      _lighted          = true;
      _opaque           = true;
      _material         = GMmaterial::polishedCopper();
      _color            = GMcolor::red();
      _collapsed        = false;
      //init end

      _side	= _up^_dir;
      _locked	= false;
      _lock_object	= 0;
      basisChange(_side, _up, _dir, _pos);
    }

    template <typename T_Stream>
    friend T_Stream& operator << ( T_Stream& out, SceneObject& s ) {

      out << s._type_id << GMseparator::object();
      out << s._matrix  << GMseparator::object()
          << s._scale   << GMseparator::object();
      s.prOut(out);

  /*  if(st)
      {
        out << _object.size() << GMseparator::object();
        for(int i=0; i<_object.size(); i++)
          if(st>1 || (_object[i]->typeId() != 2))
            out << *(_object[i]);
      }*/
      return out;
    }

    template <typename T_Stream>
    friend T_Stream& operator >> ( T_Stream& in, SceneObject& s ) {

      static Separator os(GMseparator::object());

      in >> s._type_id  >> os;
      in >> s._matrix   >> os >> s._scale >> os;
      s.prIn(in);
  /*  if(st)
      {
        int nr;
        in >> nr >> os;
        for(int i=0; i<nr; i++)
            _object += new SceneObject(in);
      }*/
      return in;
    }



  private:
    template <typename T_Stream>
    void prOut(T_Stream& out) const {

      out << _pos << GMseparator::object() << _dir << GMseparator::object() << _up << GMseparator::object();
    }

    template <typename T_Stream>
    void prIn(T_Stream& in) {

      static Separator os(GMseparator::object());
      in >> _pos >> os >> _dir >> os >> _up >> os;
    }

    #endif

  }; // END class SceneObject










  inline const Vector<float,3>& SceneObject::getDir()  const { return _dir; }
  inline const Vector<float,3>& SceneObject::getSide() const { return _side; }
  inline const Vector<float,3>& SceneObject::getUp()   const { return _up; }
  inline const Point<float,3>&  SceneObject::getPos()  const { return _pos; }

  inline Vector<float,3> SceneObject::getGlobalDir()  const { return _matrix_scene * _dir; }
  inline Vector<float,3> SceneObject::getGlobalSide() const { return _matrix_scene * _side; }
  inline Vector<float,3> SceneObject::getGlobalUp()   const { return _matrix_scene * _up; }
  inline Point<float,3>  SceneObject::getGlobalPos()  const { return _matrix_scene * _pos; }


  inline
  const HqMatrix<float,3>& SceneObject::getMatrixToScene() const {  return _matrix_scene; }

  inline
  const HqMatrix<float,3>& SceneObject::getMatrixToSceneInverse() const {  return _matrix_scene_inv; }


  inline bool SceneObject::isCollapsed() const  { return _collapsed; }
  inline void SceneObject::setCollapsed(bool c) { _collapsed = c; }
  inline bool SceneObject::toggleCollapsed()    { return _collapsed = !_collapsed; }

  inline bool SceneObject::isOpaque() const     { return _opaque; }
  inline void SceneObject::setOpaque( bool o )  { _opaque = o; }




  inline
  const Point<float,3>& SceneObject::getLockPos() const {

    if(_lock_object)
      return  _lock_object->getCenterPos();
    else
      return  _lock_pos;
  }

  inline
  double SceneObject::getLockDist() const {

    if(_locked)
      return  ( getLockPos()-getPos() ).getLength();
    else
      return  0.0;
  }

  inline
  bool SceneObject::isLocked() const {  return _locked; }





  /*! Point<float,3> SceneObject::getSceneLockPos()
   *  Get Lock Position in Scene coordinates
   */
  inline
  Point<float,3> SceneObject::getSceneLockPos() {

    if(_lock_object)
      return _matrix_scene_inv * _lock_object->getCenterPos();
    else
      return _matrix_scene_inv * _lock_pos;
  }

  inline
  void SceneObject::updateOrientation(const Point<float,3>& lock_pos ) {

    _dir    = lock_pos - _pos;
    _up     = _up - (_up * _dir) * _dir;
    _side   = _up ^ _dir;
  }


  inline
  void SceneObject::basisChange( const Vector<float,3>& x, const Vector<float,3>& y, const Vector<float,3>& z, const Vector<float,3>& p ) {

    static Vector<float,4> nx, ny, nz, np;
    memcpy( nx.getPtr(), z.getPtr(), 12 );
    memcpy( ny.getPtr(), x.getPtr(), 12 );
    memcpy( nz.getPtr(), y.getPtr(), 12 );
    memcpy( np.getPtr(), p.getPtr(), 12 );
    nx[3] = ny[3] = nz[3] = 0.0f;
    np[3] = 1.0f;

    _matrix.setCol( nx, 0 );
    _matrix.setCol( ny, 1 );
    _matrix.setCol( nz, 2 );
    _matrix.setCol( np, 3 );
  }






  /*! void SceneObject::edit(int selector_id)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void SceneObject::edit( int /* selector_id */ ) {}


  /*! void SceneObject::edit(int selector_id, Vector<float,3> delta)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void SceneObject::edit( int /* selector_id */, const Vector<float,3>& /* delta*/ ) {}


  /*! void SceneObject::edit(int selector_id, Vector<double,3> delta)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void SceneObject::edit( int /* selector_id */, const Vector<double,3>& /* delta*/ ) {}


  /*! void SceneObject::edit(SceneObject* lp)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void SceneObject::edit( SceneObject* /* obj */ ) {}


  /*! void SceneObject::edit()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void SceneObject::edit() {}


  /*! void SceneObject::editPos(Vector<float,3> delta)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void SceneObject::editPos( Vector<float,3> /* delta*/  ) {}


  /*! void SceneObject::enableChildren( bool enable = true )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void SceneObject::enableChildren( bool enable ) {

    for(int i=0;i<_children.getSize();i++) _children[i]->setVisible(enable);
  }


  /*! bool SceneObject::flipSelected()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool SceneObject::toggleSelected() {

    setSelected( !isSelected());

    if(_selected)
      edit();
    return _selected;
  }


  /*! Point<float,3> SceneObject::getCenterPos() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  const Point<float,3>& SceneObject::getCenterPos() const  {

    return  getSurroundingSphere().getPos();
  }


  /*! Array<SceneObject*>& SceneObject::getChildren()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  Array<SceneObject*>& SceneObject::getChildren(){

    return _children;
  }

  inline
  const Array<SceneObject*>& SceneObject::getChildren() const{

    return _children;
  }


  /*! const GL_Matrix& SceneObject::getMatrixGlobal() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  const HqMatrix<float,3>& SceneObject::getMatrixGlobal() const {

    return _present;
  }


  /*! unsigned int SceneObject::getName() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  unsigned int SceneObject::getName() const {

    return _name;
  }


  inline
  SceneObject* SceneObject::getParent() const {

    return _parent;
  }


  inline
  SceneObject* SceneObject::getDerived() const {

    return _derived;
  }


  inline
  Scene*
  SceneObject::getScene() {

    return _scene;
  }


  inline
  const Scene*
  SceneObject::getScene() const {
    return _scene;
  }

  /*! Sphere<float,3>  SceneObject::getSurroundingSphere() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  const Sphere<float,3>& SceneObject::getSurroundingSphere() const  {

    return  _global_total_sphere;
  }


  /*! int SceneObject::getTypeId()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  int SceneObject::getTypeId() const {

    return _type_id;
  }

  inline
  unsigned int SceneObject::getVirtualName() const {

    if( _parent && _is_part ) return _parent->getVirtualName();
    else                      return getName();
  }



  /*! bool SceneObject::isSelected() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool SceneObject::isSelected() const {

    return _selected;
  }


  /*! bool SceneObject::isVisible() const
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool SceneObject::isVisible() const {

    return _visible;
  }


  /*! void SceneObject::selectEvent(int selector_id)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void SceneObject::selectEvent( int /* selector_id */ ) {}



  /*! void SceneObject::setParent( SceneObject* obj )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void SceneObject::setParent( SceneObject* obj ) {

    _parent = obj;
  }




  /*! void SceneObject::setDerivedFrom( SceneObject* obj )
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void SceneObject::setDerived( SceneObject* obj ) {

    _derived = obj;
  }




  /*! void SceneObject::setVisible(bool v)
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  void SceneObject::setVisible( bool v, int prop ) {

    _visible = v;

    // Propagate Children
    if( prop != 0 ) {
      if( prop > 0)
        prop -= 1;

      for( int i = 0; i < _children.getSize(); i++ )
        _children[i]->setVisible( v, prop );
    }
  }


  /*! void SceneObject::toggleVisible()
   *  \brief Pending Documentation
   *
   *  Pending Documentation
   */
  inline
  bool SceneObject::toggleVisible() {

    return _visible =! _visible;
  }



  inline
  const Color&  SceneObject::getColor() const       { return _color; }

  inline
  Color&        SceneObject::getColor()             { return _color; }

  inline
  void          SceneObject::setColor( const Color& c ) { _color = c; }




  inline
  const Material& SceneObject::getMaterial() const        { return _material; }

  inline
  Material&       SceneObject::getMaterial()              { return _material; }

  inline
  void            SceneObject::setMaterial( const Material& m ) { _material = m; }




  inline
  bool          SceneObject::isLighted() const          { return _lighted; }

  inline
  void          SceneObject::setLighted( bool lighted ) { _lighted = lighted; }





  /*! Array<Visualizer*>& SceneObject::getVisualizers()
   *  Pending Documentation
   */
  inline
  Array<Visualizer*>&       SceneObject::getVisualizers()       { return _visualizers; }

  inline
  const Array<Visualizer*>& SceneObject::getVisualizers() const { return _visualizers; }




  /*! void _init( const Point<float,3>&  pos, const Vector<float,3>& dir, const Vector<float,3>& up)
   *  Pending Documentation
   */
  inline
  void SceneObject::_init( const Point<float,3>&  pos, const Vector<float,3>& dir, const Vector<float,3>& up) {

    set( pos, dir, up );
    _scene            = 0x0;
    _parent           = 0x0;
    _derived          = 0x0;
    _name             = _free_name++;
    _local_cs         = true;
    _type_id          = GM_SO_TYPE_SCENEOBJECT;
    _is_part          = false;
    _visible          = true;
    _selected         = false;
    _is_editable      = false;
    _edit_done        = false;

    _lighted          = true;
    _opaque           = true;
    _material         = GMmaterial::polishedCopper();
    _color            = GMcolor::red();
    _collapsed        = false;
  }





#ifdef GM_STREAM
//**************************************************************
//****** IOSTREAM overloaded operators for std::vector *********
//**************************************************************

template<typename T_Stream, typename T>
T_Stream& operator<<( T_Stream& out, const std::vector<T>& v ) {

    out << v.size() << GMlib::GMseparator::group();
    for( unsigned int i = 0; i < v.size(); i++ )
      out << v[i] << GMlib::GMseparator::element();
    return out;
}

template<typename T_Stream, typename T>
T_Stream& operator>>( T_Stream& in, std::vector<T>& v ) {

    static GMlib::Separator gs(GMlib::GMseparator::group());
    static GMlib::Separator es(GMlib::GMseparator::element());
    int a;
    in >> a >> gs;
    v.rezise(a);
    for( int i = 0; i < v.size(); i++ )
      in >> v[i] >> es;
    return in;
}

#endif

} // END namespace GMlib

#endif  // GM_SCENE_SCENEOBJECT_H
