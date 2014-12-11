#pragma once

#include "ResourceManagerTree.h"

class CResourceManager : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CResourceManager)
public:
	CResourceManager();
	virtual ~CResourceManager();

	static CResourceManager *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void AdjustLayout();

	CResourceManagerTree mResourceManagerTree;
};
