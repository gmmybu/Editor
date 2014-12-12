#include "stdafx.h"
#include "Editor.h"
#include "RenderPump.h"

#include "GameDoc.h"
#include "GameView.h"
#include "MaterialPreviewPage.h"
#include "OutputWnd.h"
#include "RenderView.h"
#include "ResourcePreview.h"
#include "Roaming.h"
#include "SceneDoc.h"
#include "SceneView.h"

#include "OgreConfigFile.h"
#include "OgreLogManager.h"
#include "OgreRenderWindow.h"
#include "OgreRoot.h"
#include "Overlay\OgreOverlaySystem.h"
#include "Overlay\OgreOverlayManager.h"
#include "SESkeletonManagerEx.h"

void LogListener::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool& skipThisMessage )
{
	COutputWnd::Current->Cache(message.c_str());
}

RenderPump *RenderPump::current = NULL;
RenderPump::RenderPump()
{
	current = this;
	initialized = false;
	root = NULL;
	overlaySystem = NULL;
	debugOverlay = NULL;
}

RenderPump::~RenderPump()
{
}

void RenderPump::initialize()
{
	LogListener *logListener = new LogListener;
	Ogre::LogManager *logManager = new Ogre::LogManager;
	logManager->createLog("tmp.log", true, false, true);
	logManager->getLog("tmp.log")->addListener(logListener);
	logManager->getLog("tmp.log")->setLogDetail(Ogre::LL_NORMAL);

	root = OGRE_NEW Ogre::Root("plugins_d.cfg", "ogre.cfg", "tmp.log");
	overlaySystem = OGRE_NEW Ogre::OverlaySystem();

	// 为了正确加载天龙八部的骨骼文件
	OGRE_DELETE Ogre::SkeletonManager::getSingletonPtr();
	OGRE_NEW SkeletonManagerEx;

	setupResources();
	loadConfig();
	//if(!mRoot->showConfigDialog())
	//{
	//	OGRE_DELETE mRoot;
	//	mRoot = NULL;
	//}
	if(!root->restoreConfig())
	{
		if(!root->showConfigDialog())
		{
			OGRE_DELETE root;
			root = NULL;
		}
		else
			root->saveConfig();
	}
	root->initialise(false);
}

void RenderPump::renderOneFrame()
{
	if(!initialized)
	{
		initialized = TRUE;
		roaming.initialize();
	}

	static unsigned long LastTime = root->getTimer()->getMilliseconds();
	unsigned long NowTime = root->getTimer()->getMilliseconds();
	float Elapsed = (NowTime-LastTime)*0.001f;
	LastTime = NowTime;

	roaming.process(Elapsed);
	root->renderOneFrame();

	CMaterialPreviewPage::Current->update(Elapsed);
	CResourcePreview::Current->update(Elapsed);
	if(SceneDoc::current != NULL)
	{
		if(debugOverlay && debugOverlay->isVisible())
		{
			static std::string currFps = "Current FPS: ";
			static std::string avgFps = "Average FPS: ";
			static std::string bestFps = "Best FPS: ";
			static std::string worstFps = "Worst FPS: ";
			static std::string tris = "Triangle Count: ";
			static std::string batches = "Batch Count: ";

			Ogre::OverlayElement *guiAvg = Ogre::OverlayManager::getSingletonPtr()->getOverlayElement("Core/AverageFps");
			Ogre::OverlayElement *guiCurr = Ogre::OverlayManager::getSingletonPtr()->getOverlayElement("Core/CurrFps");
			Ogre::OverlayElement *guiBest = Ogre::OverlayManager::getSingletonPtr()->getOverlayElement("Core/BestFps");
			Ogre::OverlayElement *guiWorst = Ogre::OverlayManager::getSingletonPtr()->getOverlayElement("Core/WorstFps");

			const Ogre::RenderTarget::FrameStats& stats = SceneDoc::current->getActiveView()->getRenderWindow()->getStatistics();
			guiAvg->setCaption(avgFps + Ogre::StringConverter::toString(stats.avgFPS));
			guiCurr->setCaption(currFps + Ogre::StringConverter::toString(stats.lastFPS));
			guiBest->setCaption(bestFps + Ogre::StringConverter::toString(stats.bestFPS)
				+" "+Ogre::StringConverter::toString(stats.bestFrameTime)+" ms");
			guiWorst->setCaption(worstFps + Ogre::StringConverter::toString(stats.worstFPS)
				+" "+Ogre::StringConverter::toString(stats.worstFrameTime)+" ms");

			Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingletonPtr()->getOverlayElement("Core/NumTris");
			guiTris->setCaption(tris + Ogre::StringConverter::toString(stats.triangleCount));

			Ogre::OverlayElement* guiBatches = Ogre::OverlayManager::getSingletonPtr()->getOverlayElement("Core/NumBatches");
			guiBatches->setCaption(batches + Ogre::StringConverter::toString(stats.batchCount));
		}
		SceneDoc::current->update(Elapsed);
	}
	if(CGameDoc::Current != NULL)
		CGameDoc::Current->update(Elapsed);
}

void RenderPump::showDebugOverlay(bool show)
{
	if(!debugOverlay)
		debugOverlay = Ogre::OverlayManager::getSingletonPtr()->getByName("Core/DebugOverlay");
	if (show)
		debugOverlay->show();
	else
		debugOverlay->hide();
}

void RenderPump::setupResources()
{
	Ogre::ConfigFile cf;
#if OGRE_DEBUG_MODE
    cf.load("resources_d.cfg");
#else
    cf.load("resources.cfg");
#endif
    Ogre::ConfigFile::SectionIterator secIter = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while(secIter.hasMoreElements())
    {
        secName = secIter.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = secIter.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for(i=settings->begin(); i!=settings->end(); i++)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}

void RenderPump::loadConfig()
{
	Ogre::ConfigFile cf;
#if OGRE_DEBUG_MODE
    cf.load("setting_d.cfg");
#else
    cf.load("setting.cfg");
#endif
    Ogre::ConfigFile::SectionIterator secIter = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while(secIter.hasMoreElements())
    {
        secName = secIter.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = secIter.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator iter;
        for(iter=settings->begin(); iter!=settings->end(); iter++)
        {
			theApp.addPath(iter->first, iter->second);
		}
    }
}
