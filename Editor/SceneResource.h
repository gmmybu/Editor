#pragma once

#include "SceneResourceTree.h"

class CSceneResource : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CSceneResource)
public:
	CSceneResource();
	virtual ~CSceneResource();

	static CSceneResource *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void AdjustLayout();
	CSceneResourceTree mSceneResourceTree;
};
