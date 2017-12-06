#include "Sinbad.h"

namespace OgreBites {

	Sinbad::Sinbad(Ogre::SceneNode* nod, Ogre::Vector3 pos) : ObjectMan(nod,pos)
	{
		node->scale(5, 5, 5);

		//Si fuese una luz o cámara sería diferente
		Ogre::Entity* ent = node->getCreator()->createEntity("entSinbad", "Sinbad.mesh");

		//Configuramos entidades con marcas(flags)
		ent->setQueryFlags(MY_QUERY_MASK);
		//ent->addQueryFlags(0_QUERY_MASK); // |= 0_QUERY_MASK
		setObjMan(ent);
	}


	Sinbad::~Sinbad()
	{
	}

	bool Sinbad::keyPressed(const KeyboardEvent &evt)
	{
		Keycode key = evt.keysym.sym;

		if (key == 'r')
			node->rotate(Ogre::Vector3(0, 1,0), Ogre::Radian(Ogre::Degree(10.0f)));
		if (key == 'w')
			node->translate(0.0f,0.0f,1.0f);
		if (key == 's')
			node->translate(0.0f, 0.0f, -1.0f);
		if (key == 'a')
			node->translate(1.0f, 0.0f, 0.0f);
		if (key == 'd')
			node->translate(-1.0f, 0.0f, 0.0f);

		return InputListener::keyPressed(evt);
	}
	 bool Sinbad::mousePicking(const OgreBites::MouseButtonEvent& evt)
	{
		node->showBoundingBox(true);

		return true;
	}
}
