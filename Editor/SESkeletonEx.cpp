#include "stdafx.h"
#include "SESkeletonEx.h"
#include "SESkeletonSerializerEx.h"
#include "OgreLogManager.h"
#include "OgreResourceGroupManager.h"
#include "OgreSkeletonManager.h"

SkeletonEx::SkeletonEx(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
 					const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader)
					:Skeleton(creator, name, handle, group, isManual, loader)
{
}

void SkeletonEx::loadImpl(void)
{
	// Modified
	SkeletonSerializerEx serializer;
	Ogre::LogManager::getSingleton().stream()
		<< "Skeleton: Loading " << mName;

	Ogre::DataStreamPtr stream = 
		Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);

	serializer.importSkeleton(stream, this);

	// Load any linked skeletons
	LinkedSkeletonAnimSourceList::iterator i;
	for (i = mLinkedSkeletonAnimSourceList.begin(); 
		i != mLinkedSkeletonAnimSourceList.end(); ++i)
	{
		i->pSkeleton = Ogre::SkeletonManager::getSingleton().load(i->skeletonName, mGroup).staticCast<Ogre::Skeleton>();
	}
}
