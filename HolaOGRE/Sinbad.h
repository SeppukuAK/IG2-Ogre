#ifndef __Sinbad_H__
#define __Sinbad_H__

#include "ObjectMan.h"
namespace OgreBites
{
	class Sinbad : public InputListener, public ObjectMan
	{
	public:
		Sinbad(Ogre::SceneNode* nod, Ogre::Vector3 pos);
		~Sinbad();

		//EVENTOS
		bool keyPressed(const KeyboardEvent& evt);
		virtual bool mousePicking(const OgreBites::MouseButtonEvent& evt);
		
		virtual void frameRendered(const Ogre::FrameEvent & evt) {
			//Movimiento por cuadrado
			animationState->addTime(evt.timeSinceLastFrame);

			animRunBase->addTime(evt.timeSinceLastFrame);

			animRunTop->addTime(evt.timeSinceLastFrame);

			animSacarEspadas->addTime(evt.timeSinceLastFrame);
			if (animSacarEspadas->hasEnded())
			{
				animSacarEspadas->setEnabled(false);
				animRunTop->setEnabled(true);
			}
			animCerrarManitas->addTime(evt.timeSinceLastFrame);
			if (animCerrarManitas->hasEnded())
				animCerrarManitas->setEnabled(false);
			
		};
		
		void run();
	private:
		Ogre::Entity* ent;
		Ogre::Entity* espadaEnt1;
		Ogre::Entity* espadaEnt2;

		//Animaciones del modelo
		Ogre::AnimationState* animRunBase;
		Ogre::AnimationState* animRunTop;
		Ogre::AnimationState* animSacarEspadas;
		Ogre::AnimationState* animCerrarManitas;
		Ogre::AnimationState* animAbrirManitas;

		bool espadasSacadas;

		//Animaciones propias
		Ogre::AnimationState * animationState;
		Ogre::AnimationState * animationBomb;

		void createPatrolAnimation();
		void createRunAnimation();
		void initAnimations();
		void setTAfin(Ogre::TransformKeyFrame * kf, Ogre::Vector3 keyframePos, Ogre::Quaternion quat); //TRANSFORMACION,ROTACION,ESCALA
	};

}
#endif