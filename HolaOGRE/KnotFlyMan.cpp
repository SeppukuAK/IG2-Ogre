#include "KnotFlyMan.h"

using namespace Ogre;

namespace OgreBites {

	KnotFlyMan::KnotFlyMan(Ogre::SceneNode* nod, Ogre::Vector3 pos) : ObjectMan(nod, pos)
	{
		node->setScale(Vector3{ 0.01f, 0.01f, 0.01f });

		//Creaci�n de entidad
		ent = node->getCreator()->createEntity("entKnot", "knot.mesh");
		ent->getSubEntity(0)->setMaterialName("KnotFly", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		setObjMan(ent);

		//Flags del rat�n
		ent->setQueryFlags(MY_QUERY_MASK);
	}

	KnotFlyMan::~KnotFlyMan()
	{
	}

}