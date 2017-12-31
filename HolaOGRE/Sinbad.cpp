#include "Sinbad.h"

using namespace Ogre;

namespace OgreBites {

	Sinbad::Sinbad(Ogre::SceneNode* nod, Ogre::Vector3 pos) : ObjectMan(nod, pos)
	{
		//TAfin del nodo
		node->setScale(Ogre::Vector3(4.0f));

		//Entidad
		ent = node->getCreator()->createEntity("entSinbad", "Sinbad.mesh");
		setObjMan(ent);

		//Flags del ratón
		ent->setQueryFlags(MY_QUERY_MASK);

		//Inicializacion de estados
		lastState = Idle;
		state = Patrol;

		//Entidades hijas: Espadas
		swordEntL = node->getCreator()->createEntity("entEspada1", "Sword.mesh");
		swordEntR = node->getCreator()->createEntity("entEspada2", "Sword.mesh");

		ent->attachObjectToBone("Sheath.L", swordEntL);	//Espada izquierda en la funda
		ent->attachObjectToBone("Handle.R", swordEntR);//Espada derecha en la mano

		//Iniciamos animaciones
		initAnimations();
	}

	Sinbad::~Sinbad()
	{
	}

	void Sinbad::initAnimations()
	{
		//Se inicializan las animaciones de Sinbad
		initSinbadAnim();

		//Se inicializan las animaciones propias (De nodos)
		initOwnAnim();

		/*Animaciones que se pueden hacer
		"Dance" "DrawSwords" "HandsClosed" "HandsRelaxed"
		"IdleBase" "IdleTop" "JumpEnd" "JumpLoop" "JumpStart"
		"RunBase" "RunTop" "SliceHorizontal" "SliceVertical"

		Codigo para ver las animaciones que tiene el modelo
		Ogre::AnimationStateSet * aux = ent->getAllAnimationStates();
		auto it = aux->getAnimationStateIterator().begin();
		while (it != aux->getAnimationStateIterator().end())
		{
		auto s = it->first;
		++it;
		}
		*/
	}

	void Sinbad::initSinbadAnim(){
		//Correr de piernas
		animRunBase = ent->getAnimationState("RunBase");
		animRunBase->setLoop(true);
		animRunBase->setEnabled(true);

		//Correr de torso
		animRunTop = ent->getAnimationState("RunTop");
		animRunTop->setLoop(true);
		animRunTop->setEnabled(true);

		//Sacar espadas
		animSacarEspadas = ent->getAnimationState("DrawSwords");
		animSacarEspadas->setLoop(false);
		animSacarEspadas->setEnabled(false);

		//Blandir espadas
		animBlandirEspadas = ent->getAnimationState("SliceHorizontal");
		animBlandirEspadas->setLoop(true);
		animBlandirEspadas->setEnabled(false);

		//Cerramos las manos
		ent->getAnimationState("HandsClosed")->setEnabled(true);
	}

	void Sinbad::initOwnAnim(){

		createPatrolAnimation();

		//Creación de la animación de Patrol
		animationPatrol = node->getCreator()->createAnimationState("animSinbad");
		animationPatrol->setLoop(true);
		animationPatrol->setEnabled(true);
	}

	void Sinbad::createPatrolAnimation()
	{
		//Atributos animacion
		int duracion = 20;
		float tamDesplazamiento = 120;
		Real longitudPaso = duracion / 5.0;
		Ogre::Vector3 src(0, 0, 1);//Donde mira en un inicio

		Ogre::Animation * animation = node->getCreator()->createAnimation("animSinbad", duracion);//Duracion total de la animación
		Ogre::NodeAnimationTrack * track = animation->createNodeTrack(0);//Camino 0
		track->setAssociatedNode(node);

		//Atributos rotacion
		Ogre::Vector3 dest(1, 0, 0);
		Ogre::Quaternion quat = src.getRotationTo(dest);

		//KEYFRAME ORIGEN
		TransformKeyFrame * kf = track->createNodeKeyFrame(0); // Keyframe 0: origen.
		Vector3 keyframePos(-tamDesplazamiento / 2, 20, -tamDesplazamiento / 2);
		setTAfin(kf, keyframePos, quat);

		//1
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
		kf = track->createNodeKeyFrame(longitudPaso * 4.75);
		keyframePos += Ogre::Vector3::UNIT_Z * tamDesplazamiento * -1;
		setTAfin(kf, keyframePos, quat);

		//8 - rotar	
		kf = track->createNodeKeyFrame(longitudPaso * 5.0);
		dest = Ogre::Vector3::UNIT_X;
		quat = src.getRotationTo(dest);
		setTAfin(kf, keyframePos, quat);

	}

	void Sinbad::frameRendered(const Ogre::FrameEvent & evt) {

		switch (state)
		{
			//Movimiento por cuadrado
		case Patrol:
			animationPatrol->addTime(evt.timeSinceLastFrame);
			animRunBase->addTime(evt.timeSinceLastFrame);
			animRunTop->addTime(evt.timeSinceLastFrame);
			break;

			//Movimiento de correr hacia la bomba
		case Run:
			animationRunToBomb->addTime(evt.timeSinceLastFrame);
			animRunBase->addTime(evt.timeSinceLastFrame);

			if (animSacarEspadas->getEnabled())
				animSacarEspadas->addTime(evt.timeSinceLastEvent);

			if (animBlandirEspadas->getEnabled())
				animBlandirEspadas->addTime(evt.timeSinceLastEvent);

			if (animSacarEspadas->getEnabled() && animSacarEspadas->hasEnded())
			{
				animSacarEspadas->setEnabled(false);
				animBlandirEspadas->setEnabled(true);
			}

			if (animationRunToBomb->hasEnded())
				runToDead();

			break;

		case Dead:
			node->translate(Ogre::Vector3(0.03f, 0.0f, 0.0f));
			break;

		case Idle:
			break;
		}
	};

	bool Sinbad::mousePicking(const OgreBites::MouseButtonEvent& evt)
	{
		//node->showBoundingBox(true);//Se muestra la caja de colisión

		switch (state)
		{
		case Patrol: //Patrol->Idle
			patrolToIdle();
			break;

		case Run: //Run->Idle		
			runToIdle();
			break;

		case Idle: //Distingo en función del estado anterior
			switch (lastState)
			{
			case Patrol: //Idle->Patrol
				idleToPatrol();
				break;

			case Run: //Idle->Run
				idleToRun();
				break;
			}
			break;
		}

		return true;
	}

	bool Sinbad::keyPressed(const KeyboardEvent &evt)
	{
		Keycode key = evt.keysym.sym;

		/*
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
		*/

		return InputListener::keyPressed(evt);
	}

	void Sinbad::runToBomb()
	{
		//Se desactivan las otras animaciones
		animationPatrol->setEnabled(false);
		animRunTop->setEnabled(false);
		animRunBase->setEnabled(true);


		//Se crea la animación de correr hacia la bomba
		createRunAnimation();

		//Se inicializa el animationState
		animationRunToBomb = node->getCreator()->createAnimationState("animBomb");
		animationRunToBomb->setEnabled(true);
		animationRunToBomb->setLoop(false);


		//Sinbad saca la espada izquierda
		ent->detachObjectFromBone(swordEntL);
		ent->attachObjectToBone("Handle.L", swordEntL);

		//Sinbad blande las espadas
		animSacarEspadas->setEnabled(true);

		//Actualización de estados
		lastState = Patrol;
		state = Run;
	}

	void Sinbad::createRunAnimation()
	{
		//Atributos animacion
		int duracion = 3;
		Ogre::Vector3 src(0, 0, 1);//Hacia donde mira Sinbad

		Ogre::Animation * animatio = node->getCreator()->createAnimation("animBomb", duracion);//Duracion total de la animación
		Ogre::NodeAnimationTrack * track = animatio->createNodeTrack(0);//Camino 0
		track->setAssociatedNode(node);

		//Atributos rotacion
		float xSinbad, zSinbad, xBomb, zBomb;
		xSinbad = node->getPosition().x;
		zSinbad = node->getPosition().z;
		SceneNode * nodeBomb = node->getCreator()->getEntity("entBomb")->getParentSceneNode();
		xBomb = nodeBomb->getPosition().x;
		zBomb = nodeBomb->getPosition().z;

		//Hacia donde tengo que mirar es en función de la bomba
		Ogre::Vector3 dest(xBomb - xSinbad, 0, zBomb - zSinbad);
		dest.normalise();

		Ogre::Quaternion quat = src.getRotationTo(dest);

		// Keyframe 0: origen.
		TransformKeyFrame * kf = track->createNodeKeyFrame(0);
		Vector3 keyframePos(node->getPosition());
		setTAfin(kf, keyframePos, quat);

		//Keyframe 1: bomba.
		kf = track->createNodeKeyFrame(duracion);
		keyframePos = Ogre::Vector3(nodeBomb->getPosition().x, 20, nodeBomb->getPosition().z);
		setTAfin(kf, keyframePos, quat);
	}

	//------------------MÉTODOS DE CAMBIOS DE ESTADO-------------------

	//Desactiva las animaciones de correr con los brazos y con las piernas
	void Sinbad::anyStateToIdle(){
		animRunBase->setEnabled(false);
		animRunTop->setEnabled(false);
		state = Idle;
	}

	//Pasa del estado Patrol a Idle
	void Sinbad::patrolToIdle(){
		animationPatrol->setEnabled(false);
		anyStateToIdle();
		lastState = Patrol;
	}

	//Pasa del estado Run a Idle
	void Sinbad::runToIdle(){
		animationRunToBomb->setEnabled(false);
		anyStateToIdle();
		lastState = Run;
	}

	//Pasa del estado Idle a Patrol
	void Sinbad::idleToPatrol(){
		animationPatrol->setEnabled(true);
		animRunBase->setEnabled(true);
		animRunTop->setEnabled(true);

		state = Patrol;
		lastState = Idle;
	}

	//Pasa del estado Idle a Run
	void Sinbad::idleToRun(){
		animRunTop->setEnabled(false);

		animRunBase->setEnabled(true);
		animBlandirEspadas->setEnabled(true);
		animationRunToBomb->setEnabled(true);

		state = Run;
		lastState = Idle;
	}

	void Sinbad::runToDead(){
		animRunBase->setEnabled(false);
		animBlandirEspadas->setEnabled(false);
		animationRunToBomb->setEnabled(false);
		state = Dead;
		lastState = Run;

		//Tumbamos a Sinbad
		node->rotate(Ogre::Vector3(1.0f, 0.0f, 0.0f), Ogre::Radian(3.14 / 2));
		node->rotate(Ogre::Vector3(0.0f, 1.0f, 0.0f), Ogre::Radian(3.14));
		node->translate(Ogre::Vector3(0.0f, -20.0f, 0.0f));
	}
	//------------------METODOS DE CAMBIOS DE ESTADO-------------------


	//Método que gestiona las transformaciones afines de KF (rotación, traslación y escala)
	void Sinbad::setTAfin(TransformKeyFrame * kf, Vector3 keyframePos, Quaternion quat){
		kf->setTranslate(keyframePos); // Arriba
		kf->setRotation(quat);
		kf->setScale(Ogre::Vector3(4.0f));
	}

}
