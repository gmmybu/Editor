#pragma once

#include "ListenerFactoryLogic.h"

class SSAOLogic : public ListenerFactoryLogic
{
protected:
	/** @copydoc ListenerFactoryLogic::createListener */
	virtual Ogre::CompositorInstance::Listener *createListener(Ogre::CompositorInstance *instance);
};
