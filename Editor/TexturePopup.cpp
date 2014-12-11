#include "stdafx.h"
#include "TexturePopup.h"
#include "TextureProp.h"
#include "MiscUtils.h"

TexturePopup::TexturePopup(TextureProp *Owner, CString TexName):
	mOwner(Owner),
	mTexName(TexName),
	mButtonRect(0, 65, 65, 80)
{
}


TexturePopup::~TexturePopup()
{
}

BEGIN_MESSAGE_MAP(TexturePopup, CMFCPopupMenu)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

int TexturePopup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCToolBar::IsCustomizeMode ())
	{
		// Don't show color popup in customization mode
		return -1;
	}
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD toolbarStyle = AFX_DEFAULT_TOOLBAR_STYLE;
	if (GetAnimationType () != NO_ANIMATION && !CMFCToolBar::IsCustomizeMode ())
	{
		toolbarStyle &= ~WS_VISIBLE;
	}

	mPicture.Create("", WS_VISIBLE|WS_CHILD|SS_BITMAP, CRect(0, 0, 64, 64), this);
	mPicture.SetBitmap(MiscUtils::loadBitmap(mTexName, 64));
	
	SetCapture();
	SetWindowPos(NULL, 0, 0, 70, 90, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	return 0;
}

void TexturePopup::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMFCPopupMenu::OnLButtonDown(nFlags, point);
	if(mButtonRect.PtInRect(point))
	{
		mOwner->ShowFileDlg();
		OnClose();
	}
}

void TexturePopup::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (!m_bAnimationIsDone && !CMFCToolBar::IsCustomizeMode ())
	{
		DrawFade (&dc);
	}
	else
	{
		DoPaint (&dc);
	}
	dc.FillSolidRect(mButtonRect, RGB(245,245,245));
	// ±ß¿ò
	CBrush brBtnShadow(GetSysColor(COLOR_BTNSHADOW));
	dc.FrameRect(mButtonRect, &brBtnShadow);
	// ÎÄ×Ö
 	CFont font;
 	font.Attach(::GetStockObject(DEFAULT_GUI_FONT));
 	dc.SelectObject(font);
  	dc.TextOut(20, 67, "Ìæ»»");
}

BOOL TexturePopup::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			CRect rect;
			GetClientRect (rect);
			CPoint pt (AFX_GET_X_LPARAM (pMsg->lParam), AFX_GET_Y_LPARAM (pMsg->lParam));
			if (!rect.PtInRect (pt))
			{
				OnClose();
				return TRUE;
			}
		}
		break;

	case WM_SYSKEYDOWN:
	case WM_CONTEXTMENU:
		OnClose();
		return TRUE;
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_ESCAPE)
		{
			OnClose();
			return TRUE;
		}
	}
	return CMFCPopupMenu::PreTranslateMessage(pMsg);
}
