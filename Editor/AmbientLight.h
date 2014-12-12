#pragma once

#include "OgreSimpleRenderable.h"

// Renderable for rendering Ambient component and also to
// establish the depths

// Just instantiation is sufficient
// Note that instantiation is necessary to at least establish the depths
// even if the current ambient colour is 0

// its ambient colour is same as the scene's ambient colour

// XXX Could make this a singleton/make it private to the DeferredShadingSystem e.g.

class AmbientLight : public Ogre::SimpleRenderable

{
public:
	AmbientLight();
	~AmbientLight();

	/** @copydoc MovableObject::getBoundingRadius */
	virtual Ogre::Real getBoundingRadius(void) const;
	/** @copydoc Renderable::getSquaredViewDepth */
	virtual Ogre::Real getSquaredViewDepth(const Ogre::Camera*) const;
	/** @copydoc Renderable::getMaterial */
	virtual const Ogre::MaterialPtr& getMaterial(void) const;

	virtual void getWorldTransforms(Ogre::Matrix4* xform) const;

	void updateFromCamera(Ogre::Camera* camera);
protected:
	Ogre::Real mRadius;
	Ogre::MaterialPtr mMatPtr;
};
