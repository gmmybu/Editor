#include "stdafx.h"
#include "Editor.h"
#include "MiscEditWnd.h"

IMPLEMENT_DYNAMIC(CMiscEditWnd, CBCGPDockingControlBar)

CMiscEditWnd *CMiscEditWnd::Current = NULL;
CMiscEditWnd::CMiscEditWnd()
{
	Current = this;
}

CMiscEditWnd::~CMiscEditWnd()
{
}

BEGIN_MESSAGE_MAP(CMiscEditWnd, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CMiscEditWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	mSheet.Create(CBCGPTabWnd::STYLE_FLAT, rectDummy, this, ID_MISC_EDIT_TAB, CBCGPBaseTabWnd::LOCATION_TOP);
	mSheet.EnableTabSwap(FALSE);
	AdjustLayout();

	return 0;
}


void CMiscEditWnd::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CMiscEditWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	
	mSheet.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE|SWP_NOZORDER);
}
