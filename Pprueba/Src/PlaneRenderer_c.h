#ifndef _H_PLANERENDERER_H_
#define  _H_PLANERENDERER_H_
#include "Component.h"
#include <Ogre.h>
#include <string>

class GraphicsManager;

class PlaneRenderer_c:
	public Component
{
public:
	PlaneRenderer_c(std::string meshName, std::string textName);
	~PlaneRenderer_c();

private:
	Ogre::SceneNode* plane_;
};

#endif //!_H_PLANERENDERER_H_

