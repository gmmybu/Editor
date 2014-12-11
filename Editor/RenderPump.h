#pragma once

#include "Roaming.h"
#include "OgreLog.h"

class RenderView;

class LogListener : public Ogre::LogListener
{
public:
	void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool& skipThisMessage );
};

class RenderPump
{
public:
	RenderPump();
	~RenderPump();

	bool isInitialized() { return initialized; }
	void setInitialized(bool initialized) { this->initialized = initialized; }

	RenderView *getActiveView() { return roaming.getActiveView(); }
	void setActiveView(RenderView *activeView) { roaming.setActiveView(activeView); }

	void initialize();
	void renderOneFrame();

	static RenderPump *current;

private:
	void setupResources();
	void loadConfig();

	bool initialized;
	Ogre::Root *root;
	Roaming roaming;
};
