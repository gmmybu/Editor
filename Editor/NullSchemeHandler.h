#pragma once

#include "OgreMaterialManager.h"

/** Class for skipping materials which do not have the scheme defined
 */
class NullSchemeHandler : public Ogre::MaterialManager::Listener
{
public:
	/** @copydoc MaterialManager::Listener::handleSchemeNotFound */
	virtual Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex, 
		const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex, 
		const Ogre::Renderable* rend)
	{
		//Creating a technique so the handler only gets called once per material
		Ogre::Technique* emptyTech = originalMaterial->createTechnique();
		emptyTech->removeAllPasses();
		emptyTech->setSchemeName(schemeName);
		return emptyTech;
	}
};
