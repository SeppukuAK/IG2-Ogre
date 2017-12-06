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
	};

}
#endif