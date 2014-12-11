#pragma once

enum KMiscEditSheet
{
	MES_TEXTURE_PREVIEW,
	MES_MATERIAL_PREVIEW,
	MES_TERRAIN_EDIT,
	_MET
};

class CMiscEditSheet : public CBCGPTabWnd
{
	DECLARE_DYNAMIC(CMiscEditSheet)
public:
	CMiscEditSheet();
	virtual ~CMiscEditSheet();

	void ShowPage(KMiscEditSheet Page);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CBCGPDialog *mTabs[_MET];
};
