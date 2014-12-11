#include "stdafx.h"
#include "SESkeletonManagerEx.h"
#include "SESkeletonEx.h"

SkeletonManagerEx::SkeletonManagerEx(void)
{
}

Ogre::Resource* SkeletonManagerEx::createImpl(	const Ogre::String& name, Ogre::ResourceHandle handle, 
												const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
												const Ogre::NameValuePairList* createParams)
{
	return OGRE_NEW SkeletonEx(this, name, handle, group, isManual, loader);
}
