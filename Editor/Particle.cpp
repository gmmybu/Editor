#include "stdafx.h"
#include "Particle.h"

#include "OgreSceneManager.h"
#include "OgreParticleSystem.h"

Particle::Particle(Ogre::SceneManager *sceneManager)
	:SceneObject(sceneManager, SceneObject::PARTICLE)
{
	particleSystem = NULL;
}

void Particle::create(std::string name, std::string path)
{
	SceneObject::create(name, path);
	particleSystem = sceneManager->createParticleSystem(name, path);
	particleSystem->_notifyOrigin(path);
	particleSystem->setUserAny(Ogre::Any((SceneObject*)this));
	sceneNode->attachObject(particleSystem);
}

void Particle::destroy()
{
	if(particleSystem != NULL)
		sceneManager->destroyParticleSystem(particleSystem);
	SceneObject::destroy();
}

Ogre::ParticleSystem *Particle::getParticleSystem()
{
	return particleSystem;
}

float Particle::getBoundingRadius()
{
	if(particleSystem != NULL)
		return particleSystem->getBoundingRadius();
	return SceneObject::getBoundingRadius();
}
