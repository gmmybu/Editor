#pragma once

class CRibbonPaletteButtonEx;
class CRibbonPaletteIconEx;

class CRibbonBarEx : public CBCGPRibbonBar
{
public:
	CRibbonBarEx(void);
	~CRibbonBarEx(void);

	void Create(const RECT &rect, CWnd *pParentWnd, UINT nID);
	void AddMenuItem(UINT nID, LPCTSTR lpszText, int nIndex = -1, BOOL bOnTop = FALSE);
	void AddImages(const CStringArray &Filenames);
	void ClearImages();

	int GetIconSize();
	void ResizeIcon(int Size);

	void FillPropertyWnd();
	CString GetSelectedItem();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void DoPaint(CDC* pDC);
	void RedrawElement(CBCGPBaseRibbonElement *Elmt);

	CRibbonPaletteButtonEx *mPaletteButton;
	CRibbonPaletteIconEx *mPaletteIcon; // ”√”⁄∏ﬂ¡¡
};
