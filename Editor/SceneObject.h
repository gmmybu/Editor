#pragma once

#include "OgreAny.h"
#include "tinyxml.h"

class SceneObject
{
public:
	enum Type
	{
		STATIC_MESH,
		DYNAMIC_MODEL,
		LIGHT,
		LIQUID,
		PARTICLE
	};

public:
	Ogre::SceneManager *getSceneManager();
	void setSceneManager(Ogre::SceneManager *sceneManager);

	Ogre::SceneNode *getSceneNode();
	void setSceneNode(Ogre::SceneNode *sceneNode);

	Type getType();
	void setType(Type type);

	std::string getName();
	void setName(std::string name);

	std::string getPath();
	void setPath(std::string path);

	void setUserAny(const Ogre::Any &any);
	const Ogre::Any &getUserAny();

	virtual void create(std::string name, std::string path);
	virtual void destroy();
	virtual void update(float elapsed);

	virtual Ogre::Entity *getEntity();
	virtual float getBoundingRadius();

protected:
	SceneObject(Ogre::SceneManager *sceneManager, Type type);

	Ogre::SceneManager *sceneManager; Ogre::SceneNode *sceneNode;
	Type type; std::string name; std::string path;
};
