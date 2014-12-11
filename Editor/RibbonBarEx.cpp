#include "stdafx.h"
#include "Editor.h"
#include "RibbonBarEx.h"
#include "RibbonPaletteButtonEx.h"
#include "RibbonPaletteIconEx.h"
#include "PropertyWnd.h"
#include "MiscUtils.h"
#include "StringUtils.h"

#include "FreeImage.h"

CRibbonBarEx::CRibbonBarEx(void):
	mPaletteButton(NULL),
	mPaletteIcon(NULL)
{
}

CRibbonBarEx::~CRibbonBarEx(void)
{
}

void CRibbonBarEx::Create(const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	CSize ImageSize(32, 32);
	HBITMAP hBmp = MiscUtils::loadBitmap(NULL, 32);

	CBCGPToolBarImages images;
 	images.SetImageSize(ImageSize);
 	images.AddImage(hBmp);
 	::DeleteObject(hBmp);

	mPaletteButton = new CRibbonPaletteButtonEx(nID, NULL, -1, -1, images);
	mPaletteButton->EnableMenuResize();
	mPaletteButton->m_pRibbonBar = (CBCGPRibbonBar*)this;
	mPaletteButton->m_nPanelColumns = 4;
	mPaletteButton->m_arToolTips.SetSize(1);

	CBCGPControlBar::Create(NULL, WS_CHILD|WS_VISIBLE|CBRS_HIDE_INPLACE, rect, pParentWnd, nID, 0);
	SetOwner(pParentWnd);
}

void CRibbonBarEx::AddMenuItem(UINT nID, LPCTSTR lpszText, int nIndex, BOOL bOnTop)
{
	CBCGPRibbonButton *MenuItem = new CBCGPRibbonButton(nID, lpszText, -1, -1);
	mPaletteButton->AddSubItem(MenuItem, nIndex, bOnTop);
	((CRibbonPaletteIconEx*)MenuItem)->SetParentRibbonBar(this);
}

void CRibbonBarEx::AddImages(const CStringArray &Filenames)
{
	SIZE ImageSize = mPaletteButton->m_imagesPalette.GetImageSize();
	for(int i=0; i<Filenames.GetSize(); i++)
	{
		HBITMAP hBmp = MiscUtils::loadBitmap(Filenames[i], ImageSize.cx);
		mPaletteButton->m_imagesPalette.AddImage(hBmp);
		mPaletteButton->SetItemToolTip(mPaletteButton->m_nIcons + i, Filenames[i]);
		::DeleteObject(hBmp);
	}
	if (mPaletteButton->m_imagesPalette.GetCount() == 0)
	{
		return;
	}
	mPaletteButton->m_nIcons = mPaletteButton->m_imagesPalette.GetCount();
	mPaletteButton->m_arToolTips.SetSize(mPaletteButton->m_nIcons);
	mPaletteButton->RemoveAll();
	mPaletteButton->CreateIcons();

	CDC* pDC = GetDC();
	mPaletteButton->OnAfterChangeRect(pDC);
	mPaletteButton->Redraw();

	mPaletteButton->SelectItem(0);
	GetParent()->SendMessage(WM_COMMAND, mPaletteButton->GetID());		
}

void CRibbonBarEx::ClearImages()
{
	SIZE ImageSize = {32, 32};
	mPaletteButton->m_imagesPalette.Clear();
	mPaletteButton->m_imagesPalette.SetImageSize(ImageSize);
	mPaletteButton->m_arToolTips.RemoveAll();

	HBITMAP hBmp = MiscUtils::loadBitmap(NULL, 32);
	mPaletteButton->m_imagesPalette.AddImage(hBmp);
	::DeleteObject(hBmp);
	mPaletteButton->m_nIcons = mPaletteButton->m_imagesPalette.GetCount();
	mPaletteButton->m_arToolTips.SetSize(mPaletteButton->m_nIcons);
	mPaletteButton->RemoveAll();
	mPaletteButton->CreateIcons();

	CDC* pDC = GetDC();
	mPaletteButton->OnAfterChangeRect(pDC);
	mPaletteButton->Redraw();

	mPaletteButton->SelectItem(0);
	GetParent()->SendMessage(WM_COMMAND, mPaletteButton->GetID());	
}

char *GetFormatString(FREE_IMAGE_FORMAT Format)
{
	switch(Format)
	{
	case FIF_UNKNOWN:
		return "FIF_UNKNOWN";
	case FIF_BMP:
		return "FIF_BMP";
	case FIF_JPEG:
		return "FIF_JPEG";
	case FIF_PNG:
		return "FIF_PNG";
	case FIF_TARGA:
		return "FIF_TARGA";
	case FIF_DDS:
		return "FIF_DDS";
	}
	return "FIF_UNKNOWN";
}

void CRibbonBarEx::FillPropertyWnd()
{
	CBCGPPropList *PropList = CPropertyWnd::Current->GetPropList();
	PropList->RemoveAll();

	const CString &Filename = mPaletteButton->m_arToolTips.GetAt(mPaletteButton->GetSelectedItem());
	if(!Filename.IsEmpty())
	{
		FREE_IMAGE_FORMAT ImageFormat = FreeImage_GetFileType(Filename); // 由文件头分析文件类型
		if(ImageFormat == FIF_UNKNOWN)
			ImageFormat = FreeImage_GetFIFFromFilename(Filename);

		FIBITMAP *Image = FreeImage_Load(ImageFormat, Filename);

		CBCGPProp *Prop = NULL;
		CBCGPProp *SubProp = NULL;

		Prop = new CBCGPProp("File name", Filename.GetString(), "");
		PropList->AddProperty(Prop);

		Prop = new CBCGPProp("Size");
		unsigned int ImageWidth = FreeImage_GetWidth(Image);
		SubProp = new CBCGPProp("Width", (_variant_t)ImageWidth, "");
		Prop->AddSubItem(SubProp);

		unsigned int ImageHeight = FreeImage_GetWidth(Image);
		SubProp = new CBCGPProp("Height", (_variant_t)ImageHeight, "");
		Prop->AddSubItem(SubProp);
		PropList->AddProperty(Prop);

		Prop = new CBCGPProp("Format", GetFormatString(ImageFormat), "");
		PropList->AddProperty(Prop);

		FreeImage_Unload(Image);
	}
}

CString CRibbonBarEx::GetSelectedItem()
{
	return mPaletteButton->m_arToolTips[mPaletteButton->GetSelectedItem()];
}

int CRibbonBarEx::GetIconSize()
{
	return mPaletteButton->m_imagesPalette.GetImageSize().cx;
}

void CRibbonBarEx::ResizeIcon(int Size)
{
	SIZE imageSize = {Size, Size};

	CStringArray SA;
	SA.Copy(mPaletteButton->m_arToolTips);
	if(SA.GetSize() != 1)
	{
		mPaletteButton->m_imagesPalette.Clear();
		mPaletteButton->m_imagesPalette.SetImageSize(imageSize);
		AddImages(SA);
	}
}

BEGIN_MESSAGE_MAP(CRibbonBarEx, CBCGPRibbonBar)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CRibbonBarEx::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
}

void CRibbonBarEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CBCGPRibbonBar::OnLButtonDblClk(nFlags, point);
}

void CRibbonBarEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBCGPControlBar::OnLButtonDown(nFlags, point);

	if(mPaletteIcon != NULL)
	{
		mPaletteIcon->m_bIsPressed = TRUE;
		RedrawElement (mPaletteIcon);
 		mPaletteIcon->OnLButtonDown(point);
	}
}

void CRibbonBarEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	CBCGPControlBar::OnLButtonUp(nFlags, point);

	if(mPaletteIcon != NULL)
	{
		mPaletteIcon->OnLButtonUp(point);
		mPaletteIcon->m_bIsPressed = FALSE;
		RedrawElement(mPaletteIcon);
	}
}

void CRibbonBarEx::OnMouseMove(UINT nFlags, CPoint point)
{
	CBCGPControlBar::OnMouseMove(nFlags, point);

	if(!m_bTracked)
	{
		m_bTracked = TRUE;

		TRACKMOUSEEVENT trackmouseevent;
		trackmouseevent.cbSize = sizeof(trackmouseevent);
		trackmouseevent.dwFlags = TME_LEAVE;
		trackmouseevent.hwndTrack = GetSafeHwnd();
		trackmouseevent.dwHoverTime = HOVER_DEFAULT;
		m_bTracked = _TrackMouseEvent(&trackmouseevent);
	}

	CRibbonPaletteIconEx* Target = (CRibbonPaletteIconEx*)mPaletteButton->HitTest(point);

	if(Target != mPaletteIcon)
	{
		if(mPaletteIcon != NULL)
		{
			mPaletteIcon->m_bIsHighlighted = FALSE;
			mPaletteIcon->OnHighlight (FALSE);
			RedrawElement(mPaletteIcon);
			mPaletteIcon = NULL;
		}
	}

	if(Target != NULL)
	{
		mPaletteIcon = Target;
		if(!mPaletteIcon->m_bIsHighlighted)
		{
			mPaletteIcon->OnHighlight (TRUE);
			mPaletteIcon->m_bIsHighlighted = TRUE;
			RedrawElement (mPaletteIcon);
		}
	}
}

/**
	鼠标移出窗口，则取消控件的高亮
*/
LRESULT CRibbonBarEx::OnMouseLeave(WPARAM, LPARAM)
{
	m_bTracked = FALSE;
	if (mPaletteIcon)
	{
		mPaletteIcon->m_bIsHighlighted = FALSE;
		mPaletteIcon->OnHighlight (FALSE);
		RedrawElement(mPaletteIcon);
		mPaletteIcon = NULL;
	}
	return 0;
}

static CCriticalSection gCS;
void CRibbonBarEx::OnPaint()
{
	if(m_bMultiThreaded)
	{
		gCS.Lock ();
	}

	CPaintDC dc(this);

	// erase background now
	if(GetStyle() & WS_VISIBLE)
	{
		DoPaint(&dc);    
	}

	if(m_bMultiThreaded)
	{
		gCS.Unlock ();
	}
}

void CRibbonBarEx::OnSize(UINT nType, int cx, int cy)
{
	CBCGPRibbonBar::OnSize(nType, cx, cy);
	mPaletteButton->SetRect(CRect(0, 0, cx, cy));
	CDC* pDC = GetDC();
	mPaletteButton->OnAfterChangeRect(pDC);
	mPaletteButton->Redraw();
}

void CRibbonBarEx::DoPaint(CDC* pDC)
{
	CBCGPRibbonBar::DoPaint(pDC);
	ASSERT_VALID(pDC);

	CRect rect;
	GetClientRect(&rect);
	if (rect.IsRectEmpty ())
	{
		return;
	}

	CRect rectClip;
	pDC->GetClipBox (rectClip);

	CRect rectInter;

	if (!rectInter.IntersectRect (rect, rectClip))
	{
		return;
	}

	if(rectInter.IntersectRect(mPaletteButton->GetRect (), rectClip))
	{
		BOOL bIsHighlighted = mPaletteButton->m_bIsHighlighted;
		mPaletteButton->OnDraw (pDC);
		mPaletteButton->m_bIsHighlighted = bIsHighlighted;
	}
}

void CRibbonBarEx::RedrawElement(CBCGPBaseRibbonElement *Elmt)
{
	const CRect rectElmt = Elmt->GetRect();
	if (rectElmt.IsRectEmpty ())
	{
		return;
	}
	InvalidateRect(rectElmt);
	UpdateWindow();
}
