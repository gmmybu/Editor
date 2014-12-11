#include "stdafx.h"
#include "Editor.h"
#include "SceneResource.h"

IMPLEMENT_DYNAMIC(CSceneResource, CBCGPDockingControlBar)

CSceneResource *CSceneResource::Current = NULL;
CSceneResource::CSceneResource()
{
	Current = this;
}

CSceneResource::~CSceneResource()
{
}

BEGIN_MESSAGE_MAP(CSceneResource, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CSceneResource::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
	mSceneResourceTree.Create(dwViewStyle, rectDummy, this, ID_SCENE_RESOURCE_TREE);

	mSceneResourceTree.ResetImageList();
	mSceneResourceTree.RebuildTree();
	AdjustLayout();

	return 0;
}

void CSceneResource::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CSceneResource::AdjustLayout()
{
	CRect rectClient;
	GetClientRect(rectClient);
	mSceneResourceTree.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}
