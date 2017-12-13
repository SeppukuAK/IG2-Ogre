#include "PanelMan.h"

using namespace Ogre;

namespace OgreBites 
{

	PanelMan::PanelMan(Ogre::SceneNode* nod, Ogre::Vector3 pos) : ObjectMan(nod, pos)
	{
		scnMgr = node->getCreator();

		node->rotate(Ogre::Vector3(-1, 0, 0), Ogre::Radian(Ogre::Degree(90.0f)));

		//-------PANEL: malla rectangular con textura ---------
		//MeshManager es una clase de instancia unica
		MeshPtr plane = MeshManager::getSingleton().createPlane("mFondo",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Plane(Vector3::UNIT_Z, 0),
			(Real)scnMgr->getCamera("Cam")->getViewport()->getActualWidth(), //Antes estaba con mWindow
			(Real)scnMgr->getCamera("Cam")->getViewport()->getActualHeight(),
			10, 10, true, 1, 1.0, 1.0, Vector3::UNIT_Y);

		//Creamos la entidad a partir de la malla
		Entity* entPlano = scnMgr->createEntity("entFondo", "mFondo");
		//Material por defecto es Blanco

		//Añadimos textura al material del panel
		//entPlano->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->createTextureUnitState("RustedMetal.jpg");
		entPlano->getSubEntity(0)->setMaterialName("panel", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		setObjMan(entPlano);

		//-------PANEL: malla rectangular con textura ---------

		//---------CAMARA DEL REFLEJO---------
		//Añadimos nueva cámara para el reflejo
		Camera* camRef = scnMgr->createCamera("RefCam");

		//Configuramos frustum (igual que la camara de escena)
		camRef->setNearClipDistance(scnMgr->getCamera("Cam")->getNearClipDistance());
		camRef->setFarClipDistance(scnMgr->getCamera("Cam")->getFarClipDistance());
		camRef->setAutoAspectRatio(scnMgr->getCamera("Cam")->getAutoAspectRatio());

		//Añadimos al mismo nodo de la camara
		scnMgr->getSceneNode("nCam")->attachObject(camRef);

		//Configuramos el plano sobre el que se quiere el reflejo
		camRef->enableReflection(Plane(Vector3::UNIT_Y, 0));
		camRef->enableCustomNearClipPlane(Plane(Vector3::UNIT_Y, 0));
		//---------CAMARA DEL REFLEJO---------


		//---------TEXTURA DEL REFLEJO---------
		//Creamos textura para usarla de RenderTarget y de textura del panel
		TexturePtr rttTex = TextureManager::getSingleton().createManual(
			"texRtt",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			TEX_TYPE_2D,
			(Real)scnMgr->getCamera("Cam")->getViewport()->getActualWidth(),
			(Real)scnMgr->getCamera("Cam")->getViewport()->getActualHeight(),
			0, PF_R8G8B8, TU_RENDERTARGET);

		//Añadimos un puerto de vista al Render Target con la nueva cam
		RenderTexture* renderTexture = rttTex->getBuffer()->getRenderTarget();
		Viewport* vpText = renderTexture->addViewport(camRef);
		vpText->setClearEveryFrame(true);
		vpText->setBackgroundColour(ColourValue::Black);

		//Añadimos la nueva unidad de textura al material del panel
		TextureUnitState* t = entPlano->getSubEntity(0)->getMaterial()->
			getTechnique(0)->getPass(0)->
			createTextureUnitState("texRtt");
		t->setColourOperation(LBO_ADD); // backgroundColour -> black
		// LBO_MODULATE / LBO_REPLACE / LBO_ALPHA_BLEND;
		t->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
		t->setProjectiveTexturing(true, camRef);

		//Hay que ser observadores de la nueva textura, para que nos avise antes y depués del renderizado
		renderTexture->addListener(this);
		//---------TEXTURA DEL REFLEJO---------

		entPlano->setQueryFlags(O_QUERY_MASK); // |= 0_QUERY_MASK
	}


	PanelMan::~PanelMan()
	{
	}

	// ocultar el panel y poner luz ambiente
	void  PanelMan::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt){
		scnMgr->getEntity("entFondo")->setVisible(false);
		scnMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
	}


	// restablecer los cambios
	void PanelMan::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt){
		scnMgr->getEntity("entFondo")->setVisible(true);
		scnMgr->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
	}
}
