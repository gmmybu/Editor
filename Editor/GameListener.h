#pragma once

#include "OgreRenderQueueListener.h"

class CGameListener : public Ogre::RenderQueueListener
{
public:
	void preRenderQueues();

	void postRenderQueues();

    void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, 
		bool& skipThisInvocation);

    void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, 
		bool& repeatThisInvocation);
};
