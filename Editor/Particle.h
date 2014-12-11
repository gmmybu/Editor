#pragma once

#include <SceneObject.h>

namespace Ogre
{
	class ParticleSystem;
};

class Particle : public SceneObject
{
public:
	Particle(Ogre::SceneManager *sceneManager);

	void create(std::string name, std::string path);
	void destroy();

	Ogre::ParticleSystem *getParticleSystem();
	float getBoundingRadius();

private:
	Ogre::ParticleSystem *particleSystem;
};
