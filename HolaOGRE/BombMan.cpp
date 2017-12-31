#include "BombMan.h"


using namespace Ogre;

namespace OgreBites {

	BombMan::BombMan(Ogre::SceneNode* nod, Ogre::Vector3 pos, Sinbad * pSinbad) : ObjectMan(nod, pos), ptrSin(pSinbad), boom(false)
	{
		ent = node->getCreator()->createEntity("entBomb", "uv_sphere.mesh");

		ent->getSubEntity(0)->setMaterialName("KnotFly", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		setObjMan(ent);// Examples.material -> "2 – Default " -> MtlPlat2.jpg

		/**/
		float duracion = 2;

		animation = node->getCreator()->createAnimation("animKnot", duracion);//Duracion total de la animación
		track = animation->createNodeTrack(0);//Camino 0
		track->setAssociatedNode(node);
		float tamDesplazamiento = 2;

		Real longitudPaso = duracion / 4.0;
		TransformKeyFrame * kf; // 5 keyFrames: origen(0), arriba, origen, abajo, origen(4)
		kf = track->createNodeKeyFrame(longitudPaso * 0); // Keyframe 0: origen.
		Vector3 keyframePos(-15, 0, 15);
		kf->setTranslate(keyframePos); // Origen: Vector3
		kf->setScale(Vector3{ 0.03f, 0.03f, 0.03f });
		//... ->
		kf = track->createNodeKeyFrame(longitudPaso * 1); // Keyframe 1: arriba.
		keyframePos += Ogre::Vector3::UNIT_Y * tamDesplazamiento;
		kf->setTranslate(keyframePos); // Arriba
		kf->setScale(Vector3{ 0.03f, 0.03f, 0.03f });

		// Keyframe 2: origen. ….
		kf = track->createNodeKeyFrame(longitudPaso * 3); // Keyframe 3: abajo.
		keyframePos += Ogre::Vector3::UNIT_Y * tamDesplazamiento * -2;
		kf->setTranslate(keyframePos); // Abajo
		kf->setScale(Vector3{ 0.03f, 0.03f, 0.03f });

		kf = track->createNodeKeyFrame(longitudPaso * 4); // Keyframe 4: origen.
		keyframePos += Ogre::Vector3::UNIT_Y * tamDesplazamiento;
		kf->setTranslate(keyframePos); // Origen.
		kf->setScale(Vector3{ 0.03f, 0.03f, 0.03f });

		animationState = node->getCreator()->createAnimationState("animKnot");
		animationState->setLoop(true);
		animationState->setEnabled(true);


		animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);

		//"Nombre para el sistema" , "Nombre del script de definicion"
		pSys = node->getCreator()->createParticleSystem("parSys", "Smoke");
		node->attachObject(pSys);
		pSys->setEmitting(false);

		ent->setQueryFlags(MY_QUERY_MASK);
	}


	BombMan::~BombMan()
	{
	}

}