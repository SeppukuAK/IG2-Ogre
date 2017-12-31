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

		void createPatrolAnimation();
		void createRunAnimation();
		void initAnimations();
		void setTAfin(Ogre::TransformKeyFrame * kf, Ogre::Vector3 keyframePos, Ogre::Quaternion quat); //TRANSFORMACION,ROTACION,ESCALA
	};

}
#endif