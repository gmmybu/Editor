#pragma once

#include "ResourceManager.h"
#include "SceneResource.h"
#include "MiscEditWnd.h"
#include "ResourcePreview.h"
#include "PropertyWnd.h"
#include "CameraWnd.h"
#include "OutputWnd.h"
#include "RenderPump.h"

class CMainFrame : public CBCGPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CBCGPToolBar          mObjectEditToolBar;
	CBCGPStatusBar        mStatusBar;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void CreateToolBar();
	void CreateStatusBar();
	void CreateDockingWindows();

	void InitScintilla();
	void InitFreeImage();
	void InitOGRE();

	CResourceManager	mResourceManager;
	CSceneResource		mSceneResource;
	CMiscEditWnd		mMiscEditWnd;
	CResourcePreview	mResourcePreview;
	CPropertyWnd		mPropertyWnd;
	CCameraWnd			mCameraWnd;
	COutputWnd			mOutputWnd;

	RenderPump mRenderPump;
};
