#pragma once

#include "RenderView.h"
#include "OgreVector3.h"

class SceneObject;

class SceneView : public RenderView
{
	DECLARE_DYNCREATE(SceneView)
public:
	SceneView();           // 动态创建所使用的受保护的构造函数
	virtual ~SceneView();

	int getWidth();
	int getHeight();

	void roaming(OIS::Keyboard *keyboard, OIS::Mouse *mouse, float elapsed);

	static SceneView *current;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	void setupView();
};
