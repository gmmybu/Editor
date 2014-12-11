#include "stdafx.h"
#include "SceneObject.h"

#include "OgreSceneManager.h"
#include "OgreSceneNode.h"

Ogre::SceneManager *SceneObject::getSceneManager()
{
	return sceneManager;
}

void SceneObject::setSceneManager(Ogre::SceneManager *sceneManager)
{
	this->sceneManager = sceneManager;
}

Ogre::SceneNode *SceneObject::getSceneNode()
{
	return sceneNode;
}

void SceneObject::setSceneNode(Ogre::SceneNode *sceneNode)
{
	this->sceneNode = sceneNode;
}


SceneObject::Type SceneObject::getType()
{
	return type;
}

void SceneObject::setType(Type type)
{
	this->type = type;
}

std::string SceneObject::getName()
{
	return name;
}

void SceneObject::setName(std::string name)
{
	this->name = name;
}

std::string SceneObject::getPath()
{
	return path;
}

void SceneObject::setPath(std::string path)
{
	this->path = path;
}

void SceneObject::setUserAny(const Ogre::Any &any)
{
	sceneNode->setUserAny(any);
}

const Ogre::Any &SceneObject::getUserAny()
{
	return sceneNode->getUserAny();
}

void SceneObject::create(std::string name, std::string path)
{
	this->name = name; this->path = path;
	sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(name);
}

void SceneObject::destroy()
{
	if(sceneNode != NULL)
	{
		sceneManager->getRootSceneNode()->removeAndDestroyChild(sceneNode->getName());
	}
}

void SceneObject::update(float elapsed)
{
}

Ogre::Entity *SceneObject::getEntity()
{
	return NULL;
}

float SceneObject::getBoundingRadius()
{
	return 1.0f;
}

SceneObject::SceneObject(Ogre::SceneManager *sceneManager, Type type)
{
	this->sceneManager = sceneManager;
	this->type = type;
}
