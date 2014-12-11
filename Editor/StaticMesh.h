#pragma once
#include "SceneObject.h"

namespace Ogre
{
	class Entity;
}

class StaticMesh : public SceneObject
{
public:
	StaticMesh(Ogre::SceneManager *sceneManager);

	void create(std::string name, std::string path);
	void destroy();

	Ogre::Entity *getEntity();
	float getBoundingRadius();

private:
	Ogre::Entity *entity;
};
