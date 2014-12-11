#pragma once

#include "SceneObject.h"

class Light : public SceneObject
{
public:
	Light(Ogre::SceneManager *sceneManager);

	void create(std::string name, std::string path);
	void destroy();

	Ogre::Light *getLight();
	void showIndicator();

private:
	Ogre::Light *light;
};
