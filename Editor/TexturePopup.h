#pragma once

class TextureProp;

class TexturePopup : public CMFCPopupMenu
{
public:
	TexturePopup(TextureProp *Owner, CString TexName);
	~TexturePopup();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM,LPARAM);
	afx_msg void OnPaint();

	BOOL PreTranslateMessage(MSG* pMsg);	// ¹Ø±Õµ¯³ö²Ëµ¥

	TextureProp *mOwner;
	CString mTexName;
	CStatic mPicture;
	CRect mButtonRect;
};

