#pragma once

#include "SceneObject.h"

namespace Ogre
{
	class SceneManager;
	class Entity;
}

class Liquid : public SceneObject
{
public:
	Liquid(Ogre::SceneManager *sceneManager);

	void create(std::string name, std::string path);
	void destroy();

	Ogre::Entity *getEntity();

private:
	Ogre::Entity *entity;
};
