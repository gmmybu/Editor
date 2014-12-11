#pragma once

#include "MiscEditSheet.h"

class CMiscEditWnd : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CMiscEditWnd)
public:
	CMiscEditWnd();
	virtual ~CMiscEditWnd();

	static CMiscEditWnd *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void AdjustLayout();

	CMiscEditSheet mSheet;
};
