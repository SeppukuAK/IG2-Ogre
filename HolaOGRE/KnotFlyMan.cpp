#include "KnotFlyMan.h"

using namespace Ogre;

namespace OgreBites {

	KnotFlyMan::KnotFlyMan(Ogre::SceneNode* nod, Ogre::Vector3 pos) : ObjectMan(nod, pos)
	{
		node->setScale(Vector3{ 0.01f, 0.01f, 0.01f });

		ent = node->getCreator()->createEntity("entKnot", "knot.mesh");

		ent->getSubEntity(0)->setMaterialName("KnotFly", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		setObjMan(ent);

		ent->setQueryFlags(MY_QUERY_MASK);
	}

	KnotFlyMan::~KnotFlyMan()
	{
	}

}