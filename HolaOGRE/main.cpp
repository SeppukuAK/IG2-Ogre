#include "HolaApp.h"

//Main siempre es igual
int main(int argc, char *argv[])     
{
	//En lugar de HolaApp sería otro modulo
  HolaApp app;//Constructora
  try {
    app.initApp();
    app.getRoot()->startRendering();//Empieza bucle de renderizado
  }
  catch (Ogre::Exception& e) {
    Ogre::LogManager::getSingleton().logMessage("An exception has occured: " + e.getFullDescription() + "\n");
  }
  app.closeApp();
  return 0;
}