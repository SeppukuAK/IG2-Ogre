#ifndef __Sinbad_H__
#define __Sinbad_H__

#include "ObjectMan.h"

namespace OgreBites
{
	enum State{
		Patrol,
		Run,
		Idle,
		Dead
	};

	class Sinbad : public InputListener, public ObjectMan
	{
	public:
		Sinbad(Ogre::SceneNode* nod, Ogre::Vector3 pos);
		~Sinbad();

		//EVENTOS
		bool keyPressed(const KeyboardEvent& evt);
		virtual bool mousePicking(const OgreBites::MouseButtonEvent& evt);

		virtual void frameRendered(const Ogre::FrameEvent & evt);

		//Metodo en el que Sinbad se acerca a la bomba blandiendo las espadas
		void runToBomb();

	private:
		//Entidades
		Ogre::Entity* ent;
		Ogre::Entity* swordEntL;
		Ogre::Entity* swordEntR;

		//Animaciones del modelo
		Ogre::AnimationState* animRunBase;
		Ogre::AnimationState* animRunTop;
		Ogre::AnimationState* animSacarEspadas;
		Ogre::AnimationState* animBlandirEspadas;

		//Animaciones propias
		Ogre::AnimationState * animationPatrol;
		Ogre::AnimationState * animationRunToBomb;

		//Estados
		State state;
		State lastState;

		//Inicializaci�n de animaciones
		void initAnimations();
		void initSinbadAnim();
		void initOwnAnim();

		//Creaci�n de animaciones propias
		void createPatrolAnimation(); //Crea animaci�n de dar vueltas por un cuadrado
		void createRunAnimation(); //Crea la animaci�n de moverse a la bomba

		//Cambios de estado
		void anyStateToIdle();
		void patrolToIdle();
		void runToIdle();
		void idleToPatrol();
		void idleToRun();
		void runToDead();

		//M�todo que gestiona las transformaciones afines de KF (rotaci�n, traslaci�n y escala)
		void setTAfin(Ogre::TransformKeyFrame * kf, Ogre::Vector3 keyframePos, Ogre::Quaternion quat);
	};

}
#endif