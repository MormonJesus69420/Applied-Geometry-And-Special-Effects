

#include <gtest/gtest.h>

#include <gmCoreModule>
#include <gmscene.h>
#include <gmsceneobject.h>
using namespace GMlib;

#include <vector>
#include <iostream>

namespace {

  class BasicSceneObject : public SceneObject {
    GM_SCENEOBJECT(BasicSceneObject)
  };


  TEST(Scene, SceneObject_default_values_through_get) {

    BasicSceneObject so;



    const auto dir   = so.getDir();
    const auto side  = so.getSide();
    const auto up    = so.getUp();
    const auto pos   = so.getPos();

    const auto gold_dir  = Vector<float,3>(1.0f,0.0f,0.0f);
    const auto gold_side = Vector<float,3>(0.0f,1.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_pos  = Point<float,3>(0.0f,0.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );
  }

  TEST(Scene, SceneObject_set_default_and_get) {

    BasicSceneObject so;

    so.set(Point<float, 3>(0.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );



    const auto dir   = so.getDir();
    const auto side  = so.getSide();
    const auto up    = so.getUp();
    const auto pos   = so.getPos();

    const auto gold_dir  = Vector<float,3>(1.0f,0.0f,0.0f);
    const auto gold_side = Vector<float,3>(0.0f,1.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_pos  = Point<float,3>(0.0f,0.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );
  }

  TEST(Scene, SceneObject_set_mirrored_up_and_get) {

    BasicSceneObject so;

    so.set(
          Point<float,3>(0.0f,0.0f,0.0f),     // pos
          Vector<float,3>(1.0f,0.0f,0.0f),    // dir
          Vector<float,3>(0.0f,0.0f,-1.0f)     // up
          );



    const auto dir   = so.getDir();
    const auto side  = so.getSide();
    const auto up    = so.getUp();
    const auto pos   = so.getPos();

    const auto gold_dir  = Vector<float,3>(1.0f,0.0f,0.0f);
    const auto gold_side = Vector<float,3>(0.0f,-1.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,-1.0f);
    const auto gold_pos  = Point<float,3>(0.0f,0.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );
  }

  TEST(Scene, SceneObject_translate_get) {

    BasicSceneObject so;

    so.set(Point<float, 3>(0.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );

    so.translate(Vector<float,3>(0.0f,0.0f,200.0f));



    const auto dir   = so.getDir();
    const auto side  = so.getSide();
    const auto up    = so.getUp();
    const auto pos   = so.getPos();

    const auto gold_dir  = Vector<float,3>(1.0f,0.0f,0.0f);
    const auto gold_side = Vector<float,3>(0.0f,1.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_pos  = Point<float,3>(0.0f,0.0f,200.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );
  }

  TEST(Scene, SceneObject_translateParent_get) {

    BasicSceneObject so;

    so.set(Point<float, 3>(0.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );

    so.translateParent(Vector<float,3>(0.0f,0.0f,200.0f));



    const auto dir   = so.getDir();
    const auto side  = so.getSide();
    const auto up    = so.getUp();
    const auto pos   = so.getPos();

    const auto gold_dir  = Vector<float,3>(1.0f,0.0f,0.0f);
    const auto gold_side = Vector<float,3>(0.0f,1.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_pos  = Point<float,3>(0.0f,0.0f,200.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );
  }

  TEST(Scene, SceneObject_rotate_90d_dir_get) {

    BasicSceneObject so;

    so.set(Point<float, 3>(0.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );

    so.rotate(Angle(90), so.getDir());



    const auto dir   = so.getDir();
    const auto side  = so.getSide();
    const auto up    = so.getUp();
    const auto pos   = so.getPos();

    const auto gold_dir  = Vector<float,3>(1.0f,0.0f,0.0f);
    const auto gold_side = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_up   = Vector<float,3>(0.0f,-1.0f,0.0f);
    const auto gold_pos  = Point<float,3>(0.0f,0.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );
  }


  TEST(Scene, SceneObject_transl_and_rotate_90d_dir_get) {

    BasicSceneObject so;

    so.set(Point<float, 3>(0.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );

    so.translate(so.getDir()*10.0f);
    so.rotate(Angle(90), so.getDir());



    const auto dir   = so.getDir();
    const auto side  = so.getSide();
    const auto up    = so.getUp();
    const auto pos   = so.getPos();

    const auto gold_dir  = Vector<float,3>(1.0f,0.0f,0.0f);
    const auto gold_side = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_up   = Vector<float,3>(0.0f,-1.0f,0.0f);
    const auto gold_pos  = Point<float,3>(10.0f,0.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );
  }

  TEST(Scene, SceneObject_transl_dir_and_rotate_90d_up_get) {

    BasicSceneObject so;

    so.set(Point<float, 3>(0.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );

    so.translate(so.getDir()*10.0f);
    so.rotate(Angle(90), so.getUp());



    const auto dir   = so.getDir();
    const auto side  = so.getSide();
    const auto up    = so.getUp();
    const auto pos   = so.getPos();

    const auto gold_dir  = Vector<float,3>(0.0f,1.0f,0.0f);
    const auto gold_side = Vector<float,3>(-1.0f,0.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_pos  = Point<float,3> (10.0f,0.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );
  }

  TEST(Scene, SceneObject_transl_dir_and_rotateParent_90d_up_get) {

    BasicSceneObject so;

    so.set(Point<float, 3>(0.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );

    so.translate(so.getDir()*10.0f);
    so.rotateParent(Angle(90), so.getUp());



    const auto dir   = so.getDir();
    const auto side  = so.getSide();
    const auto up    = so.getUp();
    const auto pos   = so.getPos();

    const auto gold_dir  = Vector<float,3>(0.0f,1.0f,0.0f);
    const auto gold_side = Vector<float,3>(-1.0f,0.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_pos  = Point<float,3> (10.0f,0.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );
  }

  TEST(Scene, SceneObject_transl_dir_and_rotate_90d_up_wrt_origo_get) {

    BasicSceneObject so;

    so.set(Point<float, 3>(0.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );

    so.translate(so.getDir()*10.0f);
    so.rotate(Angle(90), Point<float,3>(0.0f,0.0f,0.0f), so.getUp());



    const auto dir   = so.getDir();
    const auto side  = so.getSide();
    const auto up    = so.getUp();
    const auto pos   = so.getPos();

    const auto gold_dir  = Vector<float,3>(0.0f,1.0f,0.0f);
    const auto gold_side = Vector<float,3>(-1.0f,0.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_pos  = Point<float,3> (10.0f,0.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );
  }

  TEST(Scene, SceneObject_transl_dir_and_rotateParent_90d_up_wrt_origo_get) {

    BasicSceneObject so;

    so.set(Point<float, 3>(0.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );

    so.translate(so.getDir()*10.0f);
    so.rotateParent(Angle(90), Point<float,3>(0.0f,0.0f,0.0f), so.getUp());



    const auto dir   = so.getDir();
    const auto side  = so.getSide();
    const auto up    = so.getUp();
    const auto pos   = so.getPos();

    const auto gold_dir  = Vector<float,3>(0.0f,1.0f,0.0f);
    const auto gold_side = Vector<float,3>(-1.0f,0.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_pos  = Point<float,3> (0.0f,10.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );
  }

  TEST(Scene, SceneObject_child_set_getGlobal) {

    Scene scene;
    BasicSceneObject so, parent;
    parent.insert(&so);
    scene.insert(&parent);

    so.set(Point<float, 3>(10.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );
    parent.set(Point<float, 3>(10.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );

    scene.prepare();

//    std::cout << "P_M: " << std::endl << parent.getMatrix() << std::endl;
//    std::cout << "C_M: " << std::endl << so.getMatrix() << std::endl;
//    std::cout << "C_SM: " << std::endl << so.getMatrixToScene() << std::endl;
//    std::cout << "C_GM: " << std::endl << so.getMatrixGlobal() << std::endl;


    const auto dir   = so.getGlobalDir();
    const auto side  = so.getGlobalSide();
    const auto up    = so.getGlobalUp();
    const auto pos   = so.getGlobalPos();

//    std::cout << "dir:  " << dir << std::endl;
//    std::cout << "side: " << side << std::endl;
//    std::cout << "up:   " << up << std::endl;
//    std::cout << "pos:  " << pos << std::endl;

    const auto gold_dir  = Vector<float,3>(1.0f,0.0f,0.0f);
    const auto gold_side = Vector<float,3>(0.0f,1.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_pos  = Point<float,3> (20.0f,0.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );

    parent.remove(&so);
    scene.remove(&parent);
  }

  TEST(Scene, SceneObject_child_rotate_child_90d_getGlobal) {

    Scene scene;
    BasicSceneObject so, parent;
    parent.insert(&so);
    scene.insert(&parent);

    so.set(Point<float, 3>(10.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );
    parent.set(Point<float, 3>(10.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );

    so.rotateParent(Angle(90), Point<float,3>(0.0f,0.0f,0.0f), so.getUp());
    scene.prepare();

//    std::cout << "P_M: " << std::endl << parent.getMatrix() << std::endl;
//    std::cout << "C_M: " << std::endl << so.getMatrix() << std::endl;
//    std::cout << "C_SM: " << std::endl << so.getMatrixToScene() << std::endl;
//    std::cout << "C_GM: " << std::endl << so.getMatrixGlobal() << std::endl;


    const auto dir   = so.getGlobalDir();
    const auto side  = so.getGlobalSide();
    const auto up    = so.getGlobalUp();
    const auto pos   = so.getGlobalPos();

//    std::cout << "dir:  " << dir << std::endl;
//    std::cout << "side: " << side << std::endl;
//    std::cout << "up:   " << up << std::endl;
//    std::cout << "pos:  " << pos << std::endl;

    const auto gold_dir  = Vector<float,3>(0.0f,1.0f,0.0f);
    const auto gold_side = Vector<float,3>(-1.0f,0.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_pos  = Point<float,3> (10.0f,10.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );

    parent.remove(&so);
    scene.remove(&parent);
  }

  TEST(Scene, SceneObject_child_rotateGlobal_child_90d_getGlobal) {

    Scene scene;
    BasicSceneObject so, parent;
    parent.insert(&so);
    scene.insert(&parent);

    so.set(Point<float, 3>(10.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );
    parent.set(Point<float, 3>(10.0f, 0.0f, 0.0f),    // pos
           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
           );

    scene.prepare();
    so.rotateGlobal(Angle(90), Point<float,3>(10.0f,0.0f,0.0f), Vector<float,3>(0.0f,0.0f,1.0f));
    scene.prepare();

//    std::cout << "P_M: " << std::endl << parent.getMatrix() << std::endl;
//    std::cout << "C_M: " << std::endl << so.getMatrix() << std::endl;
//    std::cout << "C_SM: " << std::endl << so.getMatrixToScene() << std::endl;
//    std::cout << "C_GM: " << std::endl << so.getMatrixGlobal() << std::endl;


    const auto dir   = so.getGlobalDir();
    const auto side  = so.getGlobalSide();
    const auto up    = so.getGlobalUp();
    const auto pos   = so.getGlobalPos();

//    std::cout << "dir:  " << dir << std::endl;
//    std::cout << "side: " << side << std::endl;
//    std::cout << "up:   " << up << std::endl;
//    std::cout << "pos:  " << pos << std::endl;

    const auto gold_dir  = Vector<float,3>(0.0f,1.0f,0.0f);
    const auto gold_side = Vector<float,3>(-1.0f,0.0f,0.0f);
    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
    const auto gold_pos  = Point<float,3> (10.0f,10.0f,0.0f);

    EXPECT_EQ( dir,  gold_dir  );
    EXPECT_EQ( side, gold_side );
    EXPECT_EQ( up,   gold_up   );
    EXPECT_EQ( pos,  gold_pos );

    parent.remove(&so);
    scene.remove(&parent);
  }

//  TEST(Scene, SceneObject_child_transl_dir_and_rotateParent_90d_up_wrt_parent_origo_get) {

//    Scene scene;
//    BasicSceneObject so, parent;
//    parent.insert(&so);
//    scene.insert(&parent);

//    so.set(Point<float, 3>(10.0f, 0.0f, 0.0f),    // pos
//           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
//           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
//           );
//    parent.set(Point<float, 3>(10.0f, 0.0f, 0.0f),    // pos
//           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
//           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
//           );


//    so.translate(so.getDir()*10.0f);
//    so.rotateParent(Angle(90), Point<float,3>(0.0f,0.0f,0.0f), so.getUp());



//    const auto dir   = so.getDir();
//    const auto side  = so.getSide();
//    const auto up    = so.getUp();
//    const auto pos   = so.getPos();

//    const auto gold_dir  = Vector<float,3>(0.0f,1.0f,0.0f);
//    const auto gold_side = Vector<float,3>(-1.0f,0.0f,0.0f);
//    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
//    const auto gold_pos  = Point<float,3> (0.0f,10.0f,0.0f);

//    EXPECT_EQ( dir,  gold_dir  );
//    EXPECT_EQ( side, gold_side );
//    EXPECT_EQ( up,   gold_up   );
//    EXPECT_EQ( pos,  gold_pos );

//    parent.remove(&so);
//    scene.remove(&parent);
//  }

//  TEST(Scene, SceneObject_transl_dir_and_rotateParent_90d_up_wrt_pos_get) {

//    BasicSceneObject so;

//    so.set(Point<float, 3>(0.0f, 0.0f, 0.0f),    // pos
//           Vector<float, 3>(1.0f, 0.0f, 0.0f),   // dir
//           Vector<float, 3>(0.0f, 0.0f, 1.0f)    // up
//           );

//    so.translate(so.getDir()*10.0f);
//    so.rotateParent(Angle(90), so.getPos(), so.getUp());



//    const auto dir   = so.getDir();
//    const auto side  = so.getSide();
//    const auto up    = so.getUp();
//    const auto pos   = so.getPos();

//    std::cout << "dir:  " << dir << std::endl;
//    std::cout << "side: " << side << std::endl;
//    std::cout << "up:   " << up << std::endl;
//    std::cout << "pos:  " << pos << std::endl;

//    const auto gold_dir  = Vector<float,3>(0.0f,1.0f,0.0f);
//    const auto gold_side = Vector<float,3>(-1.0f,0.0f,0.0f);
//    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
//    const auto gold_pos  = Point<float,3> (10.0f,0.0f,0.0f);

//    EXPECT_EQ( dir,  gold_dir  );
//    EXPECT_EQ( side, gold_side );
//    EXPECT_EQ( up,   gold_up   );
//    EXPECT_EQ( pos,  gold_pos );
//  }





//  TEST(Scene, SceneObject_NEXT_TEST) {

//    BasicSceneObject so;



//    const auto dir   = so.getDir();
//    const auto side  = so.getSide();
//    const auto up    = so.getUp();
//    const auto pos   = so.getPos();

//    const auto gold_dir  = Vector<float,3>(1.0f,0.0f,0.0f);
//    const auto gold_side = Vector<float,3>(0.0f,1.0f,0.0f);
//    const auto gold_up   = Vector<float,3>(0.0f,0.0f,1.0f);
//    const auto gold_pos  = Point<float,3>(0.0f,0.0f,0.0f);

//    EXPECT_EQ( dir,  gold_dir  );
//    EXPECT_EQ( side, gold_side );
//    EXPECT_EQ( up,   gold_up   );
//    EXPECT_EQ( pos,  gold_pos );
//  }


}

























