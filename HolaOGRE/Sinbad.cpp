#include "Sinbad.h"

using namespace Ogre;

namespace OgreBites {

	Sinbad::Sinbad(Ogre::SceneNode* nod, Ogre::Vector3 pos) : ObjectMan(nod, pos)
	{
		ent = node->getCreator()->createEntity("entSinbad", "Sinbad.mesh");

		//Configuramos entidades con marcas(flags)
		ent->setQueryFlags(MY_QUERY_MASK);

		setObjMan(ent);

		node->setScale(Ogre::Vector3(4.0f));

		//Se inicializa el estado a Patrol
		lastState = Idle;
		state = Patrol;



		initAnimations();
	}

	Sinbad::~Sinbad()
	{
	}

	void Sinbad::initAnimPatrol(){
		//Movimiento de brazos y piernas
		animRunBase = ent->getAnimationState("RunBase");//Piernas
		animRunBase->setLoop(true);
		animRunBase->setEnabled(true);

		animRunTop = ent->getAnimationState("RunTop");//Brazos
		animRunTop->setLoop(true);
		animRunTop->setEnabled(true);

		//Creación de la animación de Patrol
		animationPatrol = node->getCreator()->createAnimationState("animSinbad");
		animationPatrol->setLoop(true);
		animationPatrol->setEnabled(true);


	}

	void Sinbad::initAnimEspadas(){
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

		//Espadas (entidad)
		espadaEnt1 = node->getCreator()->createEntity("entEspada1", "Sword.mesh");
		espadaEnt2 = node->getCreator()->createEntity("entEspada2", "Sword.mesh");

		//Espada izquierda en la funda
		ent->attachObjectToBone("Sheath.L", espadaEnt1);

		//Espada derecha en la mano
		ent->attachObjectToBone("Handle.R", espadaEnt2);
	}

	void Sinbad::initAnimations()
	{
		//Se inicializa la animación de patrulla
		createPatrolAnimation();
		initAnimPatrol();

		//Se inicializa la animación de las espadas
		initAnimEspadas();

		/*Animaciones que se pueden hacer

		/*"Dance" "DrawSwords" "HandsClosed" "HandsRelaxed"
		"IdleBase" "IdleTop" "JumpEnd" "JumpLoop" "JumpStart"
		"RunBase" "RunTop" "SliceHorizontal" "SliceVertical"*/

		/* Codigo para ver las animaciones que tiene el modelo
		Ogre::AnimationStateSet * aux = ent->getAllAnimationStates();
		auto it = aux->getAnimationStateIterator().begin();
		while (it != aux->getAnimationStateIterator().end())
		{
		auto s = it->first;
		++it;
		}
		*/

	}
	void Sinbad::frameRendered(const Ogre::FrameEvent & evt) {
		//Movimiento por cuadrado

		switch (state)
		{
		case Patrol:
			animationPatrol->addTime(evt.timeSinceLastFrame);
			animRunBase->addTime(evt.timeSinceLastFrame);
			animRunTop->addTime(evt.timeSinceLastFrame);
			break;

		case Run:

			animationRun->addTime(evt.timeSinceLastFrame);
			animRunBase->addTime(evt.timeSinceLastFrame);

			if (animSacarEspadas->hasEnded())
			{
				animSacarEspadas->setEnabled(false);
				animBlandirEspadas->setEnabled(true);
			}

			if (animSacarEspadas->getEnabled())
				animSacarEspadas->addTime(evt.timeSinceLastEvent);

			if (animBlandirEspadas->getEnabled())
				animBlandirEspadas->addTime(evt.timeSinceLastEvent);

			if (animationRun->hasEnded())
				runToDead();

			break;

		case Dead:
			node->translate(Ogre::Vector3(0.03f, 0.0f, 0.0f));
			break;

		case Idle:
			break;
		}
	};

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
		zBomb = nodeBomb->getPosition().z;

		//Este es en función de la bomba
		Ogre::Vector3 dest(xBomb - xSinbad, 0, zBomb - zSinbad);
		dest.normalise();

		Ogre::Quaternion quat = src.getRotationTo(dest);

		//KEYFRAME ORIGEN
		TransformKeyFrame * kf = track->createNodeKeyFrame(0); // Keyframe 0: origen.
		Vector3 keyframePos(node->getPosition());
		setTAfin(kf, keyframePos, quat);

		//1

		kf = track->createNodeKeyFrame(duracion);
		keyframePos = Ogre::Vector3(nodeBomb->getPosition().x, 20, nodeBomb->getPosition().z);
		setTAfin(kf, keyframePos, quat);
	}

	//Método que gestiona las transformaciones afines (rotación, traslación y escala)
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

		return InputListener::keyPressed(evt);
	}


	bool Sinbad::mousePicking(const OgreBites::MouseButtonEvent& evt)
	{
		node->showBoundingBox(true);//Se muestra la caja de colisión

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

	void Sinbad::runToBomb()
	{
		//Se desactivan las otras animaciones
		animationPatrol->setEnabled(false);
		animRunTop->setEnabled(false);
		animRunBase->setEnabled(true);

		//Se crea la animación de correr hacia la bomba
		createRunAnimation();

		//Sinbad saca la espada izquierda
		ent->detachObjectFromBone(espadaEnt1);
		ent->attachObjectToBone("Handle.L", espadaEnt1);

		//Sinbad blande las espadas
		animSacarEspadas->setEnabled(true);

		animationRun = node->getCreator()->createAnimationState("animBomb");
		animationRun->setEnabled(true);
		animationRun->setLoop(false);

		lastState = Patrol;
		state = Run;
	}

	//------------------MÉTODOS DE CAMBIOS DE ESTADO-------------------

	//Desactiva las animaciones de correr con los brazos y con las piernas
	void Sinbad::anyStateToIdle(){
		animRunBase->setEnabled(false);
		animRunTop->setEnabled(false);
	}

	//Pasa del estado Patrol a Idle
	void Sinbad::patrolToIdle(){
		animationPatrol->setEnabled(false);
		anyStateToIdle();
		state = Idle;
		lastState = Patrol;
	}

	//Pasa del estado Run a Idle
	void Sinbad::runToIdle(){
		animationRun->setEnabled(false);
		anyStateToIdle();
		state = Idle;
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

		//Blandir Espadas
		animBlandirEspadas->setEnabled(true);
		animationRun->setEnabled(true);
		animRunBase->setEnabled(true);

		state = Run;
		lastState = Idle;
	}

	void Sinbad::runToDead(){
		animBlandirEspadas->setEnabled(false);
		animRunBase->setEnabled(false);
		animationRun->setEnabled(false);
		state = Dead;
		lastState = Run;

		node->rotate(Ogre::Vector3(1.0f, 0.0f, 0.0f), Ogre::Radian(3.14 / 2));
		node->rotate(Ogre::Vector3(0.0f, 1.0f, 0.0f), Ogre::Radian(3.14));
		node->translate(Ogre::Vector3(0.0f, -20.0f, 0.0f));
	}
	//------------------METODOS DE CAMBIOS DE ESTADO-------------------
}
