#pragma once

#include "RenderScene.h"

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

	static CGameDoc *Current;

	DECLARE_MESSAGE_MAP()
};
