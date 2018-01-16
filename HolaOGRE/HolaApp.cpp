#include "HolaApp.h"
#include <iostream>
#include "Sinbad.h"
#include "PanelMan.h"
#include "KnotFlyMan.h"
#include "BombMan.h"

using namespace Ogre;

void HolaApp::frameRendered(const FrameEvent &  evt)
{
	for (int i = 0; i < vecObjMan.size(); ++i)
		vecObjMan[i]->frameRendered(evt);
	
}

bool HolaApp::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	//Termina el bucle de renderizado
  if (evt.keysym.sym == SDLK_ESCAPE)
	mRoot->queueEndRendering();
  
  //Cambia hacia donde apunta la cámara
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
	
	//Crea el rayo en función de donde pulso
	rayScnQuery->setRay(cam->getCameraToViewportRay(
		evt.x / (Real)mWindow->getViewport(0)->getActualWidth(),
		(evt.y / (Real)cam->getViewport()->getActualHeight())));
	// coordenadas normalizadas en [0,1]

	//Soy una lista, cada elemento con tres datos: distancia, movable o worldFragment
	RaySceneQueryResult& qryResult = rayScnQuery->execute();
	RaySceneQueryResult::iterator it = qryResult.begin();
	//También existe SceneQueryResult o IntersectionSceneQueryResult, listas muy raras

	//Al primer elemento con el que choco, llamo a su mousePicking
	if (it != qryResult.end())
	{
		UserControl* pCtrl = any_cast<UserControl*>(it->movable->
			getUserObjectBindings().getUserAny());
		pCtrl->getControl()->mousePicking(evt);
		//++it;//No se si esto debe ir aqui. Si hubiera más objetos haría falta
	}
	
  return true;
}

bool HolaApp::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{  
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

//Configura la escena 
void HolaApp::setupScene(void)
{
	createLight();

	createPriCam();

	createObjects();

	createQueries();

	createShader();
}


void HolaApp::createShader(){

	scnMgr->setSkyPlane(true, Plane(Vector3::UNIT_Z, -20),
		"ejemploMaterial", 1000, 10, true, 1.0, 1, 1);
	// enable, plane, materialName, scale = 1000, tiling = 10, drawFirst,
	// bow = 0, xsegments = 1, ysegments = 1, …
	// RenderQueueGroup ‐> RENDER_QUEUE_SKIES_EARLY
	//scnMgr->setSkyBox(true, “material", ……);
		//scnMgr->setSkyDome(true, “material", ……); 
}
void HolaApp::createLight()
{ 
	//Luz
	Light* light = scnMgr->createLight("Light");
	//light->setType(Ogre::Light::LT_DIRECTIONAL); //Pueden ser direccional, point, spotlight
	light->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z); // !!! opngl <-> dirección a la fuente de luz //

	//Nodo
	lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	lightNode->setPosition(0, 0, 100);
	lightNode->attachObject(light);
	
	//Hay metodos que configuran a nivel de luz

	//Crea una luz ambiente
	//scnMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
}

void HolaApp::createPriCam()
{
	//Cámara
	Camera* cam = scnMgr->createCamera("Cam");
	cam->setNearClipDistance(1);
	cam->setFarClipDistance(10000);
	cam->setAutoAspectRatio(true);
	//cam->setPolygonMode(Ogre::PM_WIREFRAME);  // en material

	//Nodo
	camNode = scnMgr->getRootSceneNode()->createChildSceneNode("nCam");
	camNode->setPosition(0, 0, 100);
	camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_WORLD);
	camNode->attachObject(cam);

	//Manager
	cameraMgr = new OgreBites::CameraMan(camNode);
	cameraMgr->setStyle(OgreBites::CameraStyle::CS_ORBIT);

	addInputListener(cameraMgr);

	//Tenemos que decir que se renderice en la ventana principal
	Viewport* vp = getRenderWindow()->addViewport(cam);//cada Viewport se añade aun RenderTarget indicando la cámara con la que se renderizará
	//vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1));
}

void HolaApp::createObjects()
{
	//Puedo mover con TS_WORLD, TS_PARENT, TS_LOCAL, rotar y cualquier otra transformación tambien
	//sceneNode->translate(100.0, 10.0, 0.0, TS_WORLD);

	createSinbadAndBomb();
	
	createMirror();

	createKnotFly();
}

void HolaApp::createSinbadAndBomb()
{
	//Sinbad
	SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode("nSinbad");
	Vector3 pos(0,25,0);

	OgreBites::Sinbad* aux = new OgreBites::Sinbad(node, pos);
	vecObjMan.push_back(aux);

	addInputListener(aux);

	//Bomba
	SceneNode * nodeKnot = scnMgr->getRootSceneNode()->createChildSceneNode("nBomb");
	pos = Vector3(0, 0, 0);

	OgreBites::BombMan * auxBomb = new OgreBites::BombMan(nodeKnot, pos, aux);
	vecObjMan.push_back(auxBomb);
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

void HolaApp::createQueries()
{
	rayScnQuery = scnMgr->createRayQuery(Ray());

	//Configuramos la pregunta con los filtros, solo se ejecutan los objetos que lo cumplen
	rayScnQuery->setQueryTypeMask(SceneManager::ENTITY_TYPE_MASK); //ENTITY_TYPE_MASK, ::LIGHT_TYPE_MASK,::FRUSTUM_TYPE_MASK, ::WORLD_GEOMETRY_TYPE_MASK, ::STATICGEOMETRY_TYPE_MASK, …
	
	// podemos especificar objetos concretos
	rayScnQuery->setQueryMask(MY_QUERY_MASK);
	
	// podemos ordenar los resultados por la distancia
	rayScnQuery->setSortByDistance(true);
}

