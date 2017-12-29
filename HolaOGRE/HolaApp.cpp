#include "HolaApp.h"
#include <iostream>
#include "Sinbad.h"
#include "PanelMan.h"
#include "KnotFlyMan.h"
#include "BombMan.h"
using namespace Ogre;

void HolaApp::frameRendered(const FrameEvent &  evt)
{
	for (int i = 0; i < vecObjMan.size(); ++i){
		vecObjMan[i]->frameRendered(evt);
	}
  //trayMgr->frameRendered(evt);
}

bool HolaApp::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	//Termina el bucle de renderizado
  if (evt.keysym.sym == SDLK_ESCAPE)
  {
    mRoot->queueEndRendering();
  }

  //POR QUÉ ESTO NO VA AYUDA ANA AAAAAAAAAAAAAAAAAAAAAAAA
  if (evt.keysym.sym == SDLK_e)
  {
	  // para saber las fuentes de luz que hay en la caja
	  Ogre::SceneQueryResult& results = lightScnQuery->execute();
	  SceneQueryResultMovableList::iterator it;
	  for (it = results.movables.begin(); it != results.movables.end(); ++it)
		  std::cout << "hola amigo";
		  // recorremos la lista de resultados de objetos del tipo MovableObject.

  }
  if (evt.keysym.sym == SDLK_t)
  {
	  //Si está apuntando a Sinbad, apunta al root
	  if (cameraMgr->getTarget()->getName() == "nSinbad")
		  cameraMgr->setTarget(scnMgr->getRootSceneNode());
	  else
		  cameraMgr->setTarget(scnMgr->getEntity("entSinbad")->getParentSceneNode());

  }


  return true;
}

bool HolaApp::mousePressed(const OgreBites::MouseButtonEvent &  evt)
{
	Camera * cam = scnMgr->getCamera("Cam");
	
	rayScnQuery->setRay(cam->getCameraToViewportRay(
		evt.x / (Real)mWindow->getViewport(0)->getActualWidth(),
		(evt.y / (Real)cam->getViewport()->getActualHeight())));
	// coordenadas normalizadas en [0,1]

	//Soy una lista, cada elemento con tres datos: distancia, movable o worldFragment
	RaySceneQueryResult& qryResult = rayScnQuery->execute();
	RaySceneQueryResult::iterator it = qryResult.begin();
	//También existe SceneQueryResult o IntersectionSceneQueryResult, listas muy raras

	if (it != qryResult.end())
	{
		UserControl* pCtrl = any_cast<UserControl*>(it->movable->
			getUserObjectBindings().getUserAny());
		pCtrl->getControl()->mousePicking(evt);
		++it;//No se si esto debe ir aqui
	}
	
  return true;
}

bool HolaApp::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
  //trayMgr->mouseMoved(evt);
  
  return true;
}

void HolaApp::setupInput()
{
  // do not forget to call the base first
  MyApplicationContext::setupInput(); 
  // register for input events
  addInputListener(this);
}

void HolaApp::shutdown()
{
	for (int i = 0; i< vecObjMan.size(); ++i) 
		delete vecObjMan[i];

	//Destruye la pregunta
	scnMgr->destroyQuery(rayScnQuery);
	scnMgr->removeRenderQueueListener(mOverlaySystem);

	delete trayMgr;  trayMgr = nullptr;
	delete cameraMgr; cameraMgr = nullptr;
	//No hay delete de ScnMgr, se lo carga el root, por factoria
	// do not forget to call the base 
	MyApplicationContext::shutdown();
}

//Configura el proyecto
void HolaApp::setup(void)
{
  // do not forget to call the base first
  MyApplicationContext::setup();

  //Crea un gestor para la escena, puede haber varios simultaneamente en el proyecto
 /* Oreg::SceneTypes: ST_GENERIC, ST_INTERIOR,
	  ST_EXTERIOR_CLOSE, ST_EXTERIOR_FAR*/
  //OGRE::SceneType -> automaticamente Ogre buscará el tipo más apropiado
  scnMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

  //Adaptaciones con otros gestores creados, se crea shaders en el contexto
  // register our scene with the RTSS
  mShaderGenerator->addSceneManager(scnMgr);

  //Interfaces gráficos, sistemas de sobreposiciones
  scnMgr->addRenderQueueListener(mOverlaySystem);

  //Gestor de paneles
  trayMgr = new OgreBites::TrayManager("TrayGUISystem", mWindow);//Hereda del inputListener
  trayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);//Sale en la esquina inferior izquierda
  
  //Ya recibe él los eventos y no los gestionamos desde aquí
  addInputListener(trayMgr);

  setupScene();
}


//Configura la escena para que salgan 4 ogritos
void HolaApp::setupScene(void)
{
	//TODOS LOS OBJETOS SE PONEN EN UN NODO

	createLight();

	createPriCam();

	createObjects();

	createQueries();
}

void HolaApp::createLight()
{
	// without light we would just get a black screen   
	Light* light = scnMgr->createLight("Light");
	//light->setType(Ogre::Light::LT_DIRECTIONAL); //Pueden ser direccional, point, spotlight
	light->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z); // !!! opngl <-> dirección a la fuente de luz //
	lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->setPosition(0, 0, 100);
	lightNode->attachObject(light);
	
	//Hay metodos que configuran a nivel de luz

	//Crea una luz ambiente
	//scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	//luz1
	//Axis - Aligned Bounding - Box(AABB) Query Example
	Light* light1 = scnMgr->createLight("Light1");

	light1->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);
	light1->setType(Ogre::Light::LT_POINT);

	lightNode1 = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode1->setPosition(12, 12, 12);
	lightNode1->attachObject(light1);
	//luz2
	
	Light* light2 = scnMgr->createLight("Light2");

	light2->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);
	light2->setType(Ogre::Light::LT_POINT);

	lightNode2 = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode2->setPosition(5, 5, 5);
	lightNode2->attachObject(light2);
	
}

void HolaApp::createPriCam()
{
	// create the camera
	Camera* cam = scnMgr->createCamera("Cam");
	cam->setNearClipDistance(1);
	cam->setFarClipDistance(10000);
	cam->setAutoAspectRatio(true);
	//cam->setPolygonMode(Ogre::PM_WIREFRAME);  // en material

	// also need to tell where we are
	camNode = scnMgr->getRootSceneNode()->createChildSceneNode("nCam");
	camNode->setPosition(0, 0, 100);
	camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_WORLD);
	camNode->attachObject(cam);

	cameraMgr = new OgreBites::CameraMan(camNode);
	cameraMgr->setStyle(OgreBites::CameraStyle::CS_ORBIT);

	addInputListener(cameraMgr);

	// and tell it to render into the main window
	Viewport* vp = getRenderWindow()->addViewport(cam);//cada Viewport se añade aun RenderTarget indicando la cámara con la que se renderizará
	//vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1));
}

void HolaApp::createObjects()
{
	//Puedo mover con TS_WORLD, TS_PARENT, TS_LOCAL, rotar y cualquier otra transformación tambien
	//sceneNode->translate(100.0, 10.0, 0.0, TS_WORLD);
	//Puedo decire que no se vea afectada por el nodo padre
	//sceneNode->setInheritOrientation(bool inherit); Default true

	createSinbad();
	
	createMirror();

	createKnotFly();

	createBomb();
}

void HolaApp::createSinbad()
{
	SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode("nSinbad");
	Vector3 pos(0,25,0);
	OgreBites::Sinbad* aux = new OgreBites::Sinbad(node, pos);

	vecObjMan.push_back(aux);
	addInputListener(aux);
}

//Utilizamos un RenderTexture() con un viewport y camara propia.
//Frame Buffer Object
void HolaApp::createMirror()
{
	SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode("nPanel");

	Vector3 pos(0, 0, 0);
	OgreBites::PanelMan * aux = new OgreBites::PanelMan(node, pos);

	vecObjMan.push_back(aux);
}

void HolaApp::createKnotFly()
{
	//Creamos el nodo en referencia a Sinbad
	SceneNode * nodeKnot = scnMgr->getEntity("entSinbad")->getParentSceneNode()->createChildSceneNode("nKnot");
	nodeKnot->setInheritOrientation(false);

	Vector3 pos(0, 5, -1);
	OgreBites::KnotFlyMan * aux = new OgreBites::KnotFlyMan(nodeKnot, pos);

	vecObjMan.push_back(aux);
}

void HolaApp::createBomb()
{
	SceneNode * nodeKnot = scnMgr->getRootSceneNode()->createChildSceneNode("nBomb");

	Vector3 pos(0, 0, 0);
	OgreBites::BombMan * aux = new OgreBites::BombMan(nodeKnot, pos);

	vecObjMan.push_back(aux);
}

void HolaApp::createQueries()
{
	//---------RAY SCENE QUERIE---------
	rayScnQuery = nullptr;
	rayScnQuery = scnMgr->createRayQuery(Ray());
	//Configuramos la pregunta con los filtros, solo se ejecutan los objetos que lo cumplen
	// Filtros: configuramos la clase de objetos que nos interesan 
	rayScnQuery->setQueryTypeMask(SceneManager::ENTITY_TYPE_MASK); //ENTITY_TYPE_MASK, ::LIGHT_TYPE_MASK,::FRUSTUM_TYPE_MASK, ::WORLD_GEOMETRY_TYPE_MASK, ::STATICGEOMETRY_TYPE_MASK, …
	// podemos especificar objetos concretos
	rayScnQuery->setQueryMask(MY_QUERY_MASK);
	// podemos ordenar los resultados por la distancia
	rayScnQuery->setSortByDistance(true);
	//---------RAY SCENE QUERIE---------

	//-------- Axis-Aligned Bounding-Box (AABB) Query Example--------------

	lightScnQuery = nullptr;
	lightScnQuery = scnMgr->createAABBQuery(AxisAlignedBox(0, 0, 0, 10, 10, 10));
	lightScnQuery->setQueryTypeMask(SceneManager::LIGHT_TYPE_MASK);
	//-------- Axis-Aligned Bounding-Box (AABB) Query Example--------------

}

