#ifndef __ObjectMan_H__
#define __ObjectMan_H__

#include "MyApplicationContext.h"

class ObjectMan; // declaración adelantada

class UserControl { // clase recubridora
public:
	ObjectMan* getControl() { return object; };
	friend class ObjectMan;
protected:
	UserControl(ObjectMan * obj) : object(obj) { };
	~UserControl() { };
	ObjectMan * object; // ObjectMan: clase raíz de una jerarquía
};

//Clase abstracta para implementar controladores para los objetos
class ObjectMan {
public:
	static const Ogre::uint32 MY_QUERY_MASK = 1; // << 0;
	static const Ogre::uint32 O_QUERY_MASK = 0;

	//...

	virtual bool mousePicking(const OgreBites::MouseButtonEvent& evt)
	{
		return true;
	};
	virtual void frameRendered(const Ogre::FrameEvent & evt) { };
	virtual void setObjMan(Ogre::MovableObject* mObj);
	virtual ~ObjectMan();
protected:
	ObjectMan(Ogre::SceneNode* scnNode, Ogre::Vector3 pos);
	Ogre::SceneNode* node = nullptr;
	UserControl* control = nullptr;

};
#endif
