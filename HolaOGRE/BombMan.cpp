#include "BombMan.h"


using namespace Ogre;

namespace OgreBites {

	BombMan::BombMan(Ogre::SceneNode* nod, Ogre::Vector3 pos, Sinbad * pSin) : ObjectMan(nod, pos), pSinbad(pSin), boom(false)
	{
		//Guardo la referencia al SceneManager
		scnMgr = node->getCreator();

		//Creación de entidad
		ent = scnMgr->createEntity("entBomb", "uv_sphere.mesh");
		ent->getSubEntity(0)->setMaterialName("KnotFly", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		setObjMan(ent);

		//Flags del ratón
		ent->setQueryFlags(MY_QUERY_MASK);

		//Sistema de particulas
		pSys = scnMgr->createParticleSystem("parSys", "Smoke"); //"Nombre para el sistema" , "Nombre del script de definicion"
		node->attachObject(pSys);
		pSys->setEmitting(false);

		initAnimation();
	}


	BombMan::~BombMan()
	{
	}

	void BombMan::initAnimation()
	{
		//Parámetros de la animación
		float duracion = 2;
		float tamDesplazamiento = 2;
		Real longitudPaso = duracion / 4.0;//Se divide entre el número de frames

		//Crea la animación
		Ogre::Animation * animation = scnMgr->createAnimation("animKnot", duracion);//Duracion total de la animación
		Ogre::NodeAnimationTrack * track = animation->createNodeTrack(0);//Camino 0
		track->setAssociatedNode(node);

		//------------------KeyFrames------------------
		TransformKeyFrame * kf; // 5 keyFrames: origen(0), arriba, origen, abajo, origen(4)

		// Keyframe 0: origen.
		kf = track->createNodeKeyFrame(longitudPaso * 0); 
		Vector3 keyframePos(-15, 0, 15);
		setTAfinKF(kf, keyframePos);

		// Keyframe 1: arriba.
		kf = track->createNodeKeyFrame(longitudPaso * 1); 
		keyframePos += Ogre::Vector3::UNIT_Y * tamDesplazamiento;
		setTAfinKF(kf, keyframePos);

		// Keyframe 2: abajo.
		kf = track->createNodeKeyFrame(longitudPaso * 3); //Hace el doble porque hay un "kf" en el origen
		keyframePos += Ogre::Vector3::UNIT_Y * tamDesplazamiento * -2;
		setTAfinKF(kf, keyframePos);

		// Keyframe 3: origen.
		kf = track->createNodeKeyFrame(longitudPaso * 4); 
		keyframePos += Ogre::Vector3::UNIT_Y * tamDesplazamiento;
		setTAfinKF(kf, keyframePos);

		//------------------KeyFrames------------------

		//Crea el estado de la animación y la activa
		animationState = scnMgr->createAnimationState("animKnot");
		animationState->setLoop(true);
		animationState->setEnabled(true);

		//"Modo de interpolacion", lo hace mas suave, "curvas de blender"
		animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
	}

	//Translacion, escala de los keyframes
	void  BombMan::setTAfinKF(TransformKeyFrame * kf, Vector3 keyframePos)
	{
		kf->setTranslate(keyframePos);
		kf->setScale(Ogre::Vector3(0.03f, 0.03f, 0.03f));
	}
}