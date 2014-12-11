#pragma once

#include "SceneObject.h"
#include <map>
#include <vector>

namespace Ogre
{
	class Bone;
	class Entity;
	class AnimationState;
}

/** 用于加载天龙的.model
*/
class DynamicModel : public SceneObject
{
public:
	DynamicModel(Ogre::SceneManager *sceneManager);

	void create(std::string name, std::string path);
	void destroy();
	void update(float elapsed);
	float getBoundingRadius();

private:
	void cloneBone(Ogre::Bone *Bone, std::map<std::string, Ogre::SceneNode*> *BoneMap);
	void _cloneBone(Ogre::Bone *Bone, Ogre::SceneNode *SceneNode, 
		std::map<std::string, Ogre::SceneNode*> *BoneMap);

	float _getBoundingRadius(Ogre::SceneNode *SceneNode);

	std::vector<Ogre::Entity*> mEntities;
	std::vector<Ogre::AnimationState*> mAnimStates;
};
