#include "stdafx.h"
#include "resource.h"
#include "ResourceManager.h"

#include "MaterialPreviewPage.h"
#include "ResourcePreview.h"

IMPLEMENT_DYNAMIC(CResourceManager, CBCGPDockingControlBar)

CResourceManager *CResourceManager::Current = NULL;
CResourceManager::CResourceManager()
{
	Current = this;
}

CResourceManager::~CResourceManager()
{
}

BEGIN_MESSAGE_MAP(CResourceManager, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CResourceManager::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
	mResourceManagerTree.Create(dwViewStyle, rectDummy, this, ID_RESOURCE_MANAGER_TREE);

	mResourceManagerTree.ResetImageList();
	mResourceManagerTree.RebuildTree();
	AdjustLayout();

	return 0;
}

void CResourceManager::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

BOOL CResourceManager::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	*pResult = 0;
	NMHDR* pNMHDR = (NMHDR*)lParam;
	if ( pNMHDR->hwndFrom == mResourceManagerTree.m_hWnd )
	{
 		switch( pNMHDR->code )
 		{
	  		case TVN_SELCHANGED:
  			{
				LPNMTREEVIEW pNMTV = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
				HTREEITEM hItem = pNMTV->itemNew.hItem;
				if ( NULL == hItem )
  					break;
				TiXmlElement *treeNodeInfo;
				treeNodeInfo = (TiXmlElement*)mResourceManagerTree.GetItemData(hItem);
				if(treeNodeInfo != NULL)
				{
					CMaterialPreviewPage::Current->AfterSelectResource(treeNodeInfo);
					CResourcePreview::Current->AfterSelectResource(treeNodeInfo);
				}
				return FALSE;
  			}
		}
	}
	return CBCGPDockingControlBar::OnNotify(wParam, lParam, pResult);
}

void CResourceManager::AdjustLayout()
{
	CRect rectClient;
	GetClientRect(rectClient);
	mResourceManagerTree.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}
