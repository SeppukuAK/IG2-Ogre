#ifndef __BombMan_H__
#define __BombMan_H__

#include "ObjectMan.h"
#include "Sinbad.h"
namespace OgreBites
{
	class BombMan : public ObjectMan
	{
	public:
		BombMan(Ogre::SceneNode* nod, Ogre::Vector3 pos);
		~BombMan();

		virtual void frameRendered(const Ogre::FrameEvent & evt) {
			animationState->addTime(evt.timeSinceLastFrame);
		};

		virtual bool mousePicking(const OgreBites::MouseButtonEvent& evt){
			pSys->setEmitting(true);
			static_cast<Sinbad*>(node->getCreator()->getEntity("entSinbad"))->run();
			return true;
		}

	private:
		Ogre::Entity* ent;
		Ogre::Animation * animation;
		Ogre::NodeAnimationTrack * track;
		Ogre::AnimationState * animationState;

		Ogre::ParticleSystem* pSys;
	};

}
#endif