#ifndef __HolaApp_H__
#define __HolaApp_H__

#include "MyApplicationContext.h"
#include <OgreTrays.h>
#include <OgreCameraMan.h>
#include "ObjectMan.h"

class HolaApp :
	//Hereda de la utilidad y de la gestion de eventos
	public MyApplicationContext, public OgreBites::InputListener
{
public:
  explicit HolaApp() : MyApplicationContext("HolaApp") { };
  virtual ~HolaApp(){ };   // delete ... los elementos creado con métodos factoria (create...) se destruyen automáticamente 
  
protected:
  virtual void setup(); 
  virtual void shutdown();
  virtual void setupInput();
  virtual void setupScene();

  //Se definen los eventos que queremos utilizar, en ogreBites::inputListener tenemos un listado de ellos
  virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);

  //eventos de raton
  virtual bool mousePressed(const OgreBites::MouseButtonEvent &  evt);
  virtual bool mouseMoved(const OgreBites::MouseMotionEvent& evt);

  //Update
  virtual void frameRendered(const Ogre::FrameEvent &  evt);

  //Creates
  void createLight();
  void createPriCam();
  void createObjects();
  void createSinbadAndBomb();
  void createMirror();
  void createKnotFly();
  void createQueries();
  void createShader();

  //Managers
  Ogre::SceneManager* scnMgr = nullptr; //Permite configurar el grafo de la escena: cámaras, luces, entidades. Factoria de varios tipos de objetos. Puede haber más de una instancia en la aplicación
  OgreBites::TrayManager* trayMgr = nullptr;
  OgreBites::CameraMan* cameraMgr = nullptr;

  //Nodos, seguramente no hagan falta aquí
  Ogre::SceneNode* lightNode = nullptr;
  Ogre::SceneNode* camNode = nullptr;

  //Rayos
  Ogre::RaySceneQuery * rayScnQuery;
 //Hay otros tipos de query AABB, ETC

  //Atributo para la máscara de los objetos
  static const Ogre::uint32 MY_QUERY_MASK = 1; // << 0;
  static const Ogre::uint32 O_QUERY_MASK = 0;

  //Vector de objetos
  std::vector<ObjectMan*> vecObjMan;

};


#endif
