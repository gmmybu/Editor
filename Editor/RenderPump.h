#pragma once

#include "Roaming.h"
#include "OgreLog.h"

class RenderView;

namespace Ogre
{
	class OverlaySystem;
	class Overlay;
}

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

	Ogre::OverlaySystem *getOverlaySystem() { return overlaySystem; }
	
	void initialize();
	void renderOneFrame();
	void showDebugOverlay(bool show);

	RenderView *getActiveView() { return roaming.getActiveView(); }
	void setActiveView(RenderView *activeView) { roaming.setActiveView(activeView); }

	static RenderPump *current;

private:
	void setupResources();
	void loadConfig();

	bool initialized;
	Roaming roaming;

	Ogre::Root *root;
	Ogre::OverlaySystem *overlaySystem;
	Ogre::Overlay *debugOverlay;
};
