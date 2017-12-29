#include "Sinbad.h"

using namespace Ogre;

namespace OgreBites {

	Sinbad::Sinbad(Ogre::SceneNode* nod, Ogre::Vector3 pos) : ObjectMan(nod, pos)
	{
		//Si fuese una luz o cámara sería diferente
		ent = node->getCreator()->createEntity("entSinbad", "Sinbad.mesh");

		//Configuramos entidades con marcas(flags)
		ent->setQueryFlags(MY_QUERY_MASK);
		//ent->addQueryFlags(0_QUERY_MASK); // |= 0_QUERY_MASK
		setObjMan(ent);

		createPatrolAnimation();

		initAnimations();
	}

	Sinbad::~Sinbad()
	{
	}

	void Sinbad::initAnimations()
	{
		//RunBase, RunTop, IdleTop, IdleBase
		espadasSacadas = false;

		//----------Animaciones de correr----------
		animRunBase = ent->getAnimationState("RunBase");
		animRunBase->setLoop(true);
		animRunBase->setEnabled(true);


		animRunTop = ent->getAnimationState("RunTop");
		animRunTop->setLoop(true);
		animRunTop->setEnabled(true);

		//----------Animaciones de correr----------

		animSacarEspadas = ent->getAnimationState("DrawSwords");
		animSacarEspadas->setLoop(false);
		animSacarEspadas->setEnabled(false);

		animCerrarManitas = ent->getAnimationState("HandsClosed");
		animCerrarManitas->setLoop(false);
		animCerrarManitas->setEnabled(false);

		animAbrirManitas = ent->getAnimationState("HandsRelaxed");
		animAbrirManitas->setLoop(false);
		animAbrirManitas->setEnabled(true);

		//Espadas
		espadaEnt1 = node->getCreator()->createEntity("entEspada1", "Sword.mesh");
		espadaEnt2 = node->getCreator()->createEntity("entEspada2", "Sword.mesh");

		//"Handle.L" "Sheath.R"
		ent->attachObjectToBone("Sheath.L", espadaEnt1);
		ent->attachObjectToBone("Sheath.R", espadaEnt2);

		/*"Dance" "DrawSwords" "HandsClosed" "HandsRelaxed"
		"IdleBase" "IdleTop" "JumpEnd" "JumpLoop" "JumpStart"
		"RunBase" "RunTop" "SliceHorizontal" "SliceVertical"*/
		//"Dance",
		/*
		Ogre::AnimationStateSet * aux = ent->getAllAnimationStates();
		auto it = aux->getAnimationStateIterator().begin();
		while (it != aux->getAnimationStateIterator().end())
		{
		auto s = it->first;
		++it;
		}
		*/

		//Asignamos la animaciones creada
		animationState = node->getCreator()->createAnimationState("animSinbad");
		animationState->setLoop(true);
		animationState->setEnabled(true);


	}


	void Sinbad::createPatrolAnimation()
	{
		int duracion = 20;

		Ogre::Animation * animation = node->getCreator()->createAnimation("animSinbad", duracion);//Duracion total de la animación
		Ogre::NodeAnimationTrack * track = animation->createNodeTrack(0);//Camino 0
		track->setAssociatedNode(node);

		float tamDesplazamiento = 120;

		Ogre::Vector3 src(0, 0, 1);
		Ogre::Vector3 dest(1, 0, 0);
		Ogre::Quaternion quat = src.getRotationTo(dest);

		//KEYFRAME ORIGEN
		TransformKeyFrame * kf = track->createNodeKeyFrame(0); // Keyframe 0: origen.
		Vector3 keyframePos(-tamDesplazamiento / 2, 20, -tamDesplazamiento / 2);
		setTAfin(kf, keyframePos, quat);

		//1
		Real longitudPaso = duracion / 5.0;
		kf = track->createNodeKeyFrame(longitudPaso * 1);
		keyframePos += Ogre::Vector3::UNIT_X * tamDesplazamiento;
		setTAfin(kf, keyframePos, quat);

		//2 - rotar
		kf = track->createNodeKeyFrame(longitudPaso * 1.25);
		dest = Ogre::Vector3::UNIT_Z;
		quat = src.getRotationTo(dest);
		setTAfin(kf, keyframePos, quat);

		//3
		kf = track->createNodeKeyFrame(longitudPaso * 2.25); 
		keyframePos += Ogre::Vector3::UNIT_Z * tamDesplazamiento;
		setTAfin(kf, keyframePos, quat);


		//4 - rotar	
		kf = track->createNodeKeyFrame(longitudPaso * 2.50);
		dest = Ogre::Vector3::UNIT_X * -1;
		quat = src.getRotationTo(dest);
		setTAfin(kf, keyframePos, quat);

		//5
		kf = track->createNodeKeyFrame(longitudPaso * 3.50); 
		keyframePos += Ogre::Vector3::UNIT_X * tamDesplazamiento * -1;
		setTAfin(kf, keyframePos, quat);

		//6 - rotar	
		kf = track->createNodeKeyFrame(longitudPaso * 3.75);
		dest = Ogre::Vector3::UNIT_Z * -1;
		quat = src.getRotationTo(dest);
		setTAfin(kf, keyframePos, quat);

		//7
		kf = track->createNodeKeyFrame(longitudPaso * 4.75); // Keyframe 4: origen.
		keyframePos += Ogre::Vector3::UNIT_Z * tamDesplazamiento * -1;
		setTAfin(kf, keyframePos, quat);


		//8 - rotar	
		kf = track->createNodeKeyFrame(longitudPaso * 5.0);
		dest = Ogre::Vector3::UNIT_X;
		quat = src.getRotationTo(dest);
		setTAfin(kf, keyframePos, quat);

	}

	void Sinbad::createRunAnimation()
	{
		int duracion = 3;

		Ogre::Animation * animatio = node->getCreator()->createAnimation("animBomb", duracion);//Duracion total de la animación
		Ogre::NodeAnimationTrack * track = animatio->createNodeTrack(0);//Camino 0
		track->setAssociatedNode(node);

		Ogre::Vector3 src(0, 0, 1);//ESTE SIEMPRE ES ESTE

		float xSinbad, zSinbad, xBomb, zBomb;
		xSinbad = node->getPosition().x;
		zSinbad = node->getPosition().z;
		SceneNode * nodeBomb = node->getCreator()->getEntity("entBomb")->getParentSceneNode();
		xBomb = nodeBomb->getPosition().x;
		zBomb = nodeBomb->getPosition().x;

		//Este es en función de la bomba
		Ogre::Vector3 dest(xSinbad- xBomb, 0, xBomb-zBomb);
		dest.normalise();

		Ogre::Quaternion quat = src.getRotationTo(dest);

		//KEYFRAME ORIGEN
		TransformKeyFrame * kf = track->createNodeKeyFrame(0); // Keyframe 0: origen.
		Vector3 keyframePos(node->getPosition());
		setTAfin(kf, keyframePos, quat);

		//1

		kf = track->createNodeKeyFrame(duracion);
		keyframePos = nodeBomb->getPosition();
		setTAfin(kf, keyframePos, quat);
	}
	//TRANSFORMACION,ROTACION,ESCALA
	void Sinbad::setTAfin(TransformKeyFrame * kf, Vector3 keyframePos, Quaternion quat){
		kf->setTranslate(keyframePos); // Arriba
		kf->setRotation(quat);
		kf->setScale(Ogre::Vector3(4.0f));
	}


	bool Sinbad::keyPressed(const KeyboardEvent &evt)
	{
		Keycode key = evt.keysym.sym;

		if (key == 'r')
			node->rotate(Ogre::Vector3(0, 1, 0), Ogre::Radian(Ogre::Degree(10.0f)));
		if (key == 'w')
			node->translate(0.0f, 0.0f, 1.0f);
		if (key == 's')
			node->translate(0.0f, 0.0f, -1.0f);
		if (key == 'a')
			node->translate(1.0f, 0.0f, 0.0f);
		if (key == 'd')
			node->translate(-1.0f, 0.0f, 0.0f);

		if (key == 'q')
		{
			if (!espadasSacadas)
			{
				animRunTop->setEnabled(false);
				animSacarEspadas->setEnabled(true);
				animCerrarManitas->setEnabled(true);
				//node->getAttachedObject(0)->detachObjectFromBone(espadaEnt2);
				ent->detachObjectFromBone(espadaEnt1);
				ent->detachObjectFromBone(espadaEnt2);
				ent->attachObjectToBone("Handle.L", espadaEnt1);
				ent->attachObjectToBone("Handle.R", espadaEnt2);

				espadasSacadas = true;
			}
			else
			{
				ent->detachObjectFromBone(espadaEnt1);
				ent->detachObjectFromBone(espadaEnt2);
				ent->attachObjectToBone("Sheath.L", espadaEnt1);
				ent->attachObjectToBone("Sheath.R", espadaEnt2);

				espadasSacadas = false;
			}
		}

		return InputListener::keyPressed(evt);
	}
	bool Sinbad::mousePicking(const OgreBites::MouseButtonEvent& evt)
	{
		node->showBoundingBox(true);

		if (animationState->getEnabled())
		{
			animationState->setEnabled(false);
			animRunBase->setEnabled(false);
			animRunTop->setEnabled(false);
		}
		else
		{
			animationState->setEnabled(true);
			animRunBase->setEnabled(true);
			animRunTop->setEnabled(true);
		}

		return true;
	}

	void Sinbad::run()
	{
		createRunAnimation();

		//Asignamos la animaciones creada
		animationBomb = node->getCreator()->createAnimationState("animBomb");
		animationBomb->setLoop(false);
		animationBomb->setEnabled(true);

		animationState->setEnabled(false);
	}
}
