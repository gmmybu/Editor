#pragma once

#include "RibbonBarEx.h"

class CTexturePreviewPage : public CBCGPDialog
{
	DECLARE_DYNAMIC(CTexturePreviewPage)
public:
	CTexturePreviewPage(CWnd* pParent = NULL);
	virtual ~CTexturePreviewPage();

	enum { IDD = IDD_TEXTURE_PREVIEW };

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTextureBar();
	afx_msg void OnLoadTexture();
	afx_msg void OnClearTexture();

	void DoDataExchange(CDataExchange* pDX);
	CRibbonBarEx mTextureBar;
	CBCGPHotSpotImageCtrl mImage;
};
