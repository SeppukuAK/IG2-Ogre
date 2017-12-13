#include "FlotadorMan.h"


using namespace Ogre;

namespace OgreBites {

	FlotadorMan::FlotadorMan(Ogre::SceneNode* nod, Ogre::Vector3 pos) : ObjectMan(nod, pos)
	{
		ent = node->getCreator()->createEntity("entKnot", "Sinbad.mesh");

		setObjMan(ent);// Examples.material -> "2 – Default " -> MtlPlat2.jpg

		//...

		animation = node->getCreator()->createAnimation("animKnot", duracion);//Duracion total de la animación
		track = animation->createNodeTrack(0);//Camino 0
		track->setAssociatedNode(node);

		duracion = 2;
		float tamDesplazamiento = 5;

		Real longitudPaso = duracion / 4.0;
		TransformKeyFrame * kf;; // 5 keyFrames: origen(0), arriba, origen, abajo, origen(4)
		kf = track->createNodeKeyFrame(longitudPaso * 0); // Keyframe 0: origen.
		Vector3 keyframePos;
		kf->setTranslate(keyframePos); // Origen: Vector3

		//... ->
		AnimationState * animationState = node->getCreator()->createAnimationState("animKnot");
		animationState->setLoop(true);
		animationState->setEnabled(true);		kf = track->createNodeKeyFrame(longitudPaso * 1); // Keyframe 1: arriba.
		keyframePos += Ogre::Vector3::UNIT_Y * tamDesplazamiento;
		kf->setTranslate(keyframePos); // Arriba

		// Keyframe 2: origen. ….

		kf = track->createNodeKeyFrame(longitudPaso * 3); // Keyframe 3: abajo.
		keyframePos += Ogre::Vector3::UNIT_Y * tamDesplazamiento * -2;
		kf->setTranslate(keyframePos); // Abajo

		kf = track->createNodeKeyFrame(longitudPaso * 4); // Keyframe 4: origen.
		keyframePos += Ogre::Vector3::UNIT_Y * tamDesplazamiento;
		kf->setTranslate(keyframePos); // Origen.

	}


	FlotadorMan::~FlotadorMan()
	{
	}

}