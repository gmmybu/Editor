#include "stdafx.h"
#include "StaticMesh.h"

#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreSubEntity.h"

StaticMesh::StaticMesh(Ogre::SceneManager *sceneManager)
	:SceneObject(sceneManager, SceneObject::STATIC_MESH)
{
	entity = NULL;
}

void StaticMesh::create(std::string name, std::string path)
{
	SceneObject::create(name, path);
	entity = sceneManager->createEntity(name, path);
	entity->setUserAny(Ogre::Any((SceneObject*)this));
	sceneNode->attachObject(entity);
}

void StaticMesh::destroy()
{
	if(entity != NULL)
		sceneManager->destroyEntity(entity);
	SceneObject::destroy();
}

Ogre::Entity *StaticMesh::getEntity()
{
	return entity;
}

float StaticMesh::getBoundingRadius()
{
	if(entity != NULL)
	{
		return entity->getBoundingRadius();
	}
	return SceneObject::getBoundingRadius();
}
