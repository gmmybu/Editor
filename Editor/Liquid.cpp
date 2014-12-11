#include "stdafx.h"
#include "Liquid.h"

#include "OgreSceneManager.h"
#include "OgreEntity.h"

Liquid::Liquid(Ogre::SceneManager *sceneManager):
	SceneObject(sceneManager, SceneObject::LIQUID)
{
	entity = NULL;
}

void Liquid::create(std::string name, std::string path)
{
	SceneObject::create(name, path);
	entity = sceneManager->createEntity(name, path);
	entity->setUserAny(Ogre::Any((SceneObject*)this));
	sceneNode->attachObject(entity);
}

void Liquid::destroy()
{
	if(entity != NULL)
		sceneManager->destroyEntity(entity);
	SceneObject::destroy();
}

Ogre::Entity *Liquid::getEntity()
{
	return entity;
}
