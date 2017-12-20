#ifndef __PanelMan_H__
#define __PanelMan_H__

#include "ObjectMan.h"

namespace OgreBites
{
	class PanelMan : public Ogre::RenderTargetListener, public ObjectMan
	{
	public:
		PanelMan(Ogre::SceneNode* nod, Ogre::Vector3 pos);
		~PanelMan();

		//EVENTOS
		virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);// ocultar el panel y poner luz ambiente
		virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);// restablecer los cambios
	private:
		Ogre::SceneManager* scnMgr;
		Ogre::RenderTexture* renderTexture;
	};

}
#endif