#pragma once
#include "OgreSkeletonSerializer.h"
class SkeletonSerializerEx : public Ogre::SkeletonSerializer
{
public:
	SkeletonSerializerEx(void);
	~SkeletonSerializerEx(void);
	void importSkeleton(Ogre::DataStreamPtr& stream, Ogre::Skeleton* pDest);

protected:
	void readAnimation(Ogre::DataStreamPtr& stream, Ogre::Skeleton* pSkel);
	// Modified
	void readAnimationTrack(Ogre::DataStreamPtr& stream, Ogre::Animation* anim, Ogre::Skeleton* pSkel);
};
