#include "stdafx.h"
#include "SceneDoc.h"
#include "SceneView.h"

#include "TerrainEditHandler.h"
#include "TerrainEditPage.h"
#include "ObjectEditHandler.h"
#include "LiquidEditHandler.h"
#include "CharacterController.h"
#include "DeferredShading.h"

#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"

#include "OIS\OISMouse.h"

IMPLEMENT_DYNCREATE(SceneView, RenderView)

SceneView *SceneView::current = NULL;
SceneView::SceneView()
{
}

SceneView::~SceneView()
{
}

int SceneView::getWidth()
{
	return renderWindow->getWidth();
}

int SceneView::getHeight()
{
	return renderWindow->getHeight();
}

void SceneView::roaming(OIS::Keyboard *keyboard, OIS::Mouse *mouse, float elapsed)
{
	SceneDoc *sceneDoc = (SceneDoc*)GetDocument();
	if(!cameraManager)
		setCameraManager(sceneDoc->getCameraManager());

	CPoint screenPoint;
	GetCursorPos(&screenPoint);
	ScreenToClient(&screenPoint);

	if(	screenPoint.x < 0 || screenPoint.x > getWidth() || 
		screenPoint.y < 0 || screenPoint.y > getHeight())
	{
		return;
	}

	sceneDoc->roaming(screenPoint, elapsed);
	RenderView::roaming(keyboard, mouse, elapsed);
}

BEGIN_MESSAGE_MAP(SceneView, RenderView)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

void SceneView::OnDestroy()
{
	RenderView::OnDestroy();
	SceneView::current = NULL;

	((SceneDoc*)GetDocument())->destroy();
	SceneDoc::current = NULL;
}

void SceneView::OnLButtonDown(UINT nFlags, CPoint point)
{
	((SceneDoc*)GetDocument())->leftDown(nFlags, point);
	RenderView::OnLButtonDown(nFlags, point);
}

void SceneView::OnLButtonUp(UINT nFlags, CPoint point)
{
	((SceneDoc*)GetDocument())->leftUp(nFlags, point);
	RenderView::OnLButtonUp(nFlags, point);
}

void SceneView::OnSetFocus(CWnd* pOldWnd)
{
	RenderView::OnSetFocus(pOldWnd);
	SceneView::current = this;

	((SceneDoc*)GetDocument())->setActiveView(this);
	SceneDoc::current = (SceneDoc*)GetDocument();
}

void SceneView::setupView()
{
	RenderView::setupView();
	SceneDoc *Doc = (SceneDoc*)GetDocument();
	Doc->setDeferredShadingSystem(new DeferredShadingSystem(viewport, Doc->getSceneManager(), Doc->getCamera()));
	Doc->getDeferredShadingSystem()->initialize();
}
