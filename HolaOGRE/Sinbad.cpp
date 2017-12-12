#include "Sinbad.h"

namespace OgreBites {

	Sinbad::Sinbad(Ogre::SceneNode* nod, Ogre::Vector3 pos) : ObjectMan(nod,pos)
	{
		node->scale(5, 5, 5);

		//Si fuese una luz o cámara sería diferente
		ent = node->getCreator()->createEntity("entSinbad", "Sinbad.mesh");

		//Configuramos entidades con marcas(flags)
		ent->setQueryFlags(MY_QUERY_MASK);
		//ent->addQueryFlags(0_QUERY_MASK); // |= 0_QUERY_MASK
		setObjMan(ent);


		//animacion
		//RunBase, RunTop, IdleTop, IdleBase
		
		animRunBase = ent->getAnimationState("RunBase");
		animRunBase->setLoop(true);
		animRunBase->setEnabled(true);
		
		animRunTop = ent->getAnimationState("RunTop");
		animRunTop->setLoop(true);
		animRunTop->setEnabled(true);
		
		
		//Espadas
		espadaEnt1 = node->getCreator()->createEntity("entEspada1", "Sword.mesh");
		espadaEnt2 = node->getCreator()->createEntity("entEspada2", "Sword.mesh");

		//"Handle.L" "Sheath.R"
		ent->attachObjectToBone("Sheath.L", espadaEnt1);
		ent->attachObjectToBone("Sheath.R", espadaEnt2);

		/*"Dance" "DrawSwords" "HandsClosed" "HandsRelaxed"
		"IdleBase" "IdleTop" "JumpEnd" "JumpLoop" "JumpStart"
		"RunBase" "RunTop" "SliceHorizontal" "SliceVertical"*/
		//"Dance",
		Ogre::AnimationStateSet * aux = ent->getAllAnimationStates();
		auto it = aux->getAnimationStateIterator().begin();
		while (it != aux->getAnimationStateIterator().end())
		{
			auto s = it->first;
			++it;
		}

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

		if (key == 'q')
		{
			//node->getAttachedObject(0)->detachObjectFromBone(espadaEnt2);
			ent->detachObjectFromBone(espadaEnt1);
			ent->detachObjectFromBone(espadaEnt2);

		}

		return InputListener::keyPressed(evt);
	}
	 bool Sinbad::mousePicking(const OgreBites::MouseButtonEvent& evt)
	{
		node->showBoundingBox(true);

		return true;
	}
}
