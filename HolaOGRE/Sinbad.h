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

			animRunBase->addTime(evt.timeSinceLastFrame);

			if (animRunTop->getEnabled())
				animRunTop->addTime(evt.timeSinceLastFrame);

			if (animSacarEspadas->getEnabled())
			{
				animSacarEspadas->addTime(evt.timeSinceLastFrame);
				if (animSacarEspadas->hasEnded())
				{
					animSacarEspadas->setEnabled(false);
					animRunTop->setEnabled(true);
				}
			}

			if (animCerrarManitas->getEnabled())
			{
				animCerrarManitas->addTime(evt.timeSinceLastFrame);
				if (animCerrarManitas->hasEnded())
					animCerrarManitas->setEnabled(false);
			}
			
		};
		
	private:
		Ogre::Entity* ent;
		Ogre::Entity* espadaEnt1;
		Ogre::Entity* espadaEnt2;

		Ogre::AnimationState* animRunBase;
		Ogre::AnimationState* animRunTop;
		Ogre::AnimationState* animSacarEspadas;
		Ogre::AnimationState* animCerrarManitas;
		Ogre::AnimationState* animAbrirManitas;

		bool espadasSacadas;

	};

}
#endif