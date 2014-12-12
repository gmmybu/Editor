#include "stdafx.h"
#include "GameView.h"
#include "GameDoc.h"
#include "OgreRenderWindow.h"

IMPLEMENT_DYNCREATE(CGameView, RenderView)

CGameView *CGameView::Current = NULL;
CGameView::CGameView()
{
	Current = this;
}

CGameView::~CGameView()
{
}

int CGameView::GetWidth()
{
	return renderWindow->getWidth();
}

int CGameView::GetHeight()
{
	return renderWindow->getHeight();
}

void CGameView::Roaming(OIS::Keyboard *Keyboard, OIS::Mouse *Mouse, float Elapsed)
{
	CGameDoc* GameDoc = (CGameDoc*)GetDocument();
	if(!cameraManager)
	{
		//IDirect3DDevice9 *Device = NULL;
		//mRenderWindow->getCustomAttribute("D3DDEVICE", (void*)&Device);
		setCameraManager(GameDoc->getCameraManager());
	}

	CPoint screenPoint;
	GetCursorPos(&screenPoint);
	ScreenToClient(&screenPoint);

	if(	screenPoint.x < 0 || screenPoint.x > GetWidth() || 
		screenPoint.y < 0 || screenPoint.y > GetHeight())
	{
		return;
	}

	RenderView::roaming(Keyboard, Mouse, Elapsed);
}

BEGIN_MESSAGE_MAP(CGameView, RenderView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CGameView::OnDestroy()
{
	RenderView::OnDestroy();
	CGameView::Current = NULL;

	((CGameDoc*)GetDocument())->Destroy();
	CGameDoc::Current = NULL;
}
