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
		//6666666666
		virtual void frameRendered(const Ogre::FrameEvent & evt);

		//Metodo en el que Sinbad se acerca a la bomba blandiendo las espadas
		void runToBomb();

	private:
		Ogre::Entity* ent;
		Ogre::Entity* espadaEnt1;
		Ogre::Entity* espadaEnt2;

		//Animaciones del modelo
		Ogre::AnimationState* animRunBase;
		Ogre::AnimationState* animRunTop;


		Ogre::AnimationState* animSacarEspadas;
		Ogre::AnimationState* animBlandirEspadas;



		//Animaciones propias
		Ogre::AnimationState * animationPatrol;
		Ogre::AnimationState * animationRun;

		State state;
		State lastState;

		//Cambios de estado
		void patrolToIdle();
		void runToIdle();
		void anyStateToIdle();
		void idleToPatrol();
		void idleToRun();
		void runToDead();

		void createPatrolAnimation(); //666666
			void createRunAnimation(); //66666666
			void initAnimations();
		void initAnimPatrol();
		void initAnimEspadas();
		void setTAfin(Ogre::TransformKeyFrame * kf, Ogre::Vector3 keyframePos, Ogre::Quaternion quat); //TRANSFORMACION,ROTACION,ESCALA
	};

}
#endif