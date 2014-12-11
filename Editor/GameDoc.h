#pragma once

#include "RenderScene.h"

class DeferredShadingSystem;

namespace Ogre
{
	class SceneManager;
	class Camera;
	class SceneNode;
}

class CGameDoc : public RenderScene
{
	DECLARE_DYNCREATE(CGameDoc)
public:
	CGameDoc();
	virtual ~CGameDoc();

	void Initialize();
	void Destroy();

	void update(float Elapsed);

	DeferredShadingSystem *System;
	static CGameDoc *Current;

	DECLARE_MESSAGE_MAP()

	void createAtheneScene(Ogre::SceneNode *rootNode);
	void createSampleLights();
	void createKnotScene(Ogre::SceneNode* rootNode);
	void createObjects(Ogre::SceneNode* rootNode);
};
