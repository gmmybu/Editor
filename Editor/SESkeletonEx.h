#pragma once
#include "OgreSkeleton.h"
class SkeletonEx : public Ogre::Skeleton
{
public:
 	SkeletonEx(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
		const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0);

protected:
	/** @copydoc Resource::loadImpl
	*/
	void loadImpl(void);
};
