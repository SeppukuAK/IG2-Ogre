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
  virtual void frameRendered(const Ogre::FrameEvent &  evt);

  //Permite configurar el grafo de la escena: cámaras, luces, entidades. 
  //Factoria de varios tipos de objetos
  //Puede haber más de una instancia en la aplicación
  Ogre::SceneManager* scnMgr = nullptr;

  OgreBites::TrayManager* trayMgr = nullptr;
  OgreBites::CameraMan* cameraMgr = nullptr;

  Ogre::SceneNode* lightNode = nullptr;
  Ogre::SceneNode* lightNode1 = nullptr;
  Ogre::SceneNode* lightNode2 = nullptr;
  Ogre::SceneNode* camNode = nullptr;

  //Atributo para el rayo
  Ogre::RaySceneQuery * rayScnQuery;
  Ogre::AxisAlignedBoxSceneQuery* lightScnQuery;

  //Atributo para la máscara de los objetos
  static const Ogre::uint32 MY_QUERY_MASK = 1; // << 0;
  static const Ogre::uint32 O_QUERY_MASK = 0;

  std::vector<ObjectMan*> vecObjMan;

  void createLight();
  void createPriCam();
  void createObjects();
  void createSinbad();
  void createMirror();
  void createQueries();

};


#endif
