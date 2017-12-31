#ifndef __BombMan_H__
#define __BombMan_H__

#include "ObjectMan.h"
#include "Sinbad.h"


namespace OgreBites
{
	//Bomba que explota si haces click y hace que Sinbad vaya a por ella
	class BombMan : public ObjectMan
	{
	public:
		BombMan(Ogre::SceneNode* nod, Ogre::Vector3 pos, Sinbad * pSin);
		~BombMan();

		virtual void frameRendered(const Ogre::FrameEvent & evt) {
			animationState->addTime(evt.timeSinceLastFrame);
		};

		virtual bool mousePicking(const OgreBites::MouseButtonEvent& evt){
			//Si hago click en la bomba
			if (!boom)
			{
				boom = true;
				pSys->setEmitting(true);//Emito particulas
				pSinbad->runToBomb(); //Sinbad corre hacia ella
			}
			return true;
		}

	private:
		//Entidad
		Ogre::Entity* ent;		
		Ogre::AnimationState * animationState;

		//Humo
		Ogre::ParticleSystem* pSys;

		//Referencias
		Ogre::SceneManager* scnMgr;
		Sinbad * pSinbad;

		//Booleana que indica si la bomba ha explotado
		bool boom;

		void initAnimation();
		void setTAfinKF(Ogre::TransformKeyFrame * kf, Ogre::Vector3 keyframePos); //Translacion, escala

	};

}
#endif