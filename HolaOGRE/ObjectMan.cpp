#include "ObjectMan.h"


ObjectMan::ObjectMan(Ogre::SceneNode* scnNode, Ogre::Vector3 pos) : node(scnNode) {
	control = new UserControl(this);
	node->setPosition(pos);
}


ObjectMan::~ObjectMan() {
	UserControl* pCtrl = Ogre::any_cast<UserControl*>(
		node->getAttachedObject(0)->//Suponemos que solo puede tener controlador el primer objeto adjunto a un nodo
		getUserObjectBindings().getUserAny());
	delete pCtrl;
}

void ObjectMan::setObjMan(Ogre::MovableObject* mObj) {
	//comprobar que es primer objeto que se adjunta al nodo
	if (node->getAttachedObjects().size() == 0)
	{
		node->attachObject(mObj);
		node->getAttachedObject(0)->getUserObjectBindings().setUserAny(Ogre::Any(control));
	}
}