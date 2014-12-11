#pragma once

#include "RenderView.h"

class CGameView : public RenderView
{
	DECLARE_DYNCREATE(CGameView)
public:
	CGameView();
	~CGameView();

	int GetWidth();
	int GetHeight();

	void Roaming(OIS::Keyboard *Keyboard, OIS::Mouse *Mouse, float Elapsed);

	static CGameView *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	void SetupView();
};
