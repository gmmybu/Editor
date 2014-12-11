#pragma once

class CameraManager;

enum KCameraWnd
{
	CW_SPEED,
	CW_POLYGON_MODE,
	CW_XPOS,
	CW_YPOS,
	CW_ZPOS,
	CW_XDIR,
	CW_YDIR,
	CW_ZDIR,
	_CW
};

class CCameraWnd : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CCameraWnd)
public:
	CCameraWnd();
	virtual ~CCameraWnd();

	void update(CameraManager *CameraManager);

	static CCameraWnd *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);

	void AdjustLayout();

	CBCGPPropList mPropList;
	CBCGPProp *mProps[_CW];
	CameraManager *mCameraManager;
};
