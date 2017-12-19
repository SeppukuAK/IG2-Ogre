#ifndef __FlotadorMan_H__
#define __FlotadorMan_H__

#include "ObjectMan.h"
namespace OgreBites
{
	class FlotadorMan : public ObjectMan
	{
	public:
		FlotadorMan(Ogre::SceneNode* nod, Ogre::Vector3 pos);
		~FlotadorMan();

		virtual void frameRendered(const Ogre::FrameEvent & evt) {
			animationState->addTime(evt.timeSinceLastFrame);
		};

	private:
		Ogre::Entity* ent;
		Ogre::Animation * animation;
		Ogre::NodeAnimationTrack * track;
		Ogre::AnimationState * animationState;
		float duracion;
	};

}
#endif