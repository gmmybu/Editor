#pragma once

class CPropertyListener
{
public:
	/** 诸如ObjectEidtHandler等修改了PropertyWnd所引用的资源时，
		应主动使PropertyWnd更新其中的数据。
	*/
	virtual void FirePropertyChanged() = 0;

	/** 当修改了PropertyWnd中的属性，将会更新其所引用的资源。
	*/
	virtual void OnPropertyChanged(CBCGPProp *Prop) = 0;
};

class CPropertyWnd : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CPropertyWnd)
public:
	CPropertyWnd();
	virtual ~CPropertyWnd();

	CBCGPPropList *GetPropList() { return &mPropList; }
	void SetListener(CPropertyListener *Listener) { mListener = Listener; }
	void FirePropertyChanged();

	static CPropertyWnd *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);

	void AdjustLayout();
	CBCGPPropList mPropList;
	CPropertyListener *mListener;
};
