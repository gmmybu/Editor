#pragma once

#include "CameraManager.h"

namespace Ogre
{
	class SceneManager;
	class Camera;
}

class RenderScene : public CDocument
{
	DECLARE_DYNCREATE(RenderScene)
public:
	RenderScene();
	~RenderScene();

	Ogre::SceneManager *getSceneManager() { return sceneManager; }
	Ogre::Camera *getCamera() { return camera; } CameraManager *getCameraManager() { return &cameraManager; }
	
	DECLARE_MESSAGE_MAP()
	Ogre::SceneManager *sceneManager;
	Ogre::Camera *camera; CameraManager cameraManager;
};
