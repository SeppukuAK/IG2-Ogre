#ifndef __KnotFlyMan_H__
#define __KnotFlyMan_H__

#include "ObjectMan.h"

namespace OgreBites
{
	class KnotFlyMan : public ObjectMan
	{
	public:
		KnotFlyMan(Ogre::SceneNode* nod, Ogre::Vector3 pos);
		~KnotFlyMan();

		virtual void frameRendered(const Ogre::FrameEvent & evt) {
			node->rotate(Ogre::Vector3(0.0f, 1.0f, 0.0f), Ogre::Radian(0.03f));
		};

	private:
		Ogre::Entity* ent;

	};

}
#endif