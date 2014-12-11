#include "stdafx.h"
#include "Editor.h"
#include "SceneDoc.h"
#include "PropertyWnd.h"
#include "SceneResourceTree.h"
#include "SceneObject.h"

IMPLEMENT_DYNAMIC(CPropertyWnd, CBCGPDockingControlBar)

CPropertyWnd *CPropertyWnd::Current = NULL;
CPropertyWnd::CPropertyWnd()
{
	Current = this;
	mListener = NULL;
}

CPropertyWnd::~CPropertyWnd()
{
}

void CPropertyWnd::FirePropertyChanged()
{
	if(mListener != NULL)
		mListener->FirePropertyChanged();
}

BEGIN_MESSAGE_MAP(CPropertyWnd, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

int CPropertyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	mPropList.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER, rectDummy, this, 0);
	mPropList.EnableHeaderCtrl(FALSE);
	mPropList.SetVSDotNetLook();
	AdjustLayout();

	return 0;
}

void CPropertyWnd::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertyWnd::AdjustLayout()
{
	CRect rectClient;
	GetClientRect(rectClient);
	mPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

LRESULT CPropertyWnd::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
{
	if(mListener != NULL)
		mListener->OnPropertyChanged((CBCGPProp*)lparam);
	return 0;
}
