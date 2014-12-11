// TerrainEditWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "OgreSE.h"
#include "SceneDoc.h"

#include "TerrainEditWnd.h"
#include "TerrainEditHandler.h"
#include "TerrainBrush.h"

// CTerrainEditWnd

IMPLEMENT_DYNAMIC(CTerrainEditWnd, CBCGPDockingControlBar)

CTerrainEditWnd::CTerrainEditWnd():
	mBrushText("Brush"),
	mSizeText("Size"),
	mPowerText("Power"),
	mTexText("Texture")
{
}

CTerrainEditWnd::~CTerrainEditWnd()
{
}

BEGIN_MESSAGE_MAP(CTerrainEditWnd, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_RELEASEDCAPTURE, ID_SIZE_SLIDER, OnReleasedcaptureSlider) 
	ON_NOTIFY(NM_RELEASEDCAPTURE, ID_POWER_SLIDER, OnReleasedcaptureSlider)
END_MESSAGE_MAP()

// CTerrainEditWnd 消息处理程序

int CTerrainEditWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	rect.SetRectEmpty();

	mBrushBar.Create(rect, this, 123);
	mBrushBar.AddMenuItem(1234, "载入画刷...");
	mBrushBar.AddMenuItem(1, "清除画刷");
	mBrushBar.AddMenuItem(1, "加载画刷组...");
	mBrushBar.AddMenuItem(1, "保存画刷组...");
	mBrushBar.AddMenuItem(1, "切换图标尺寸");

	mSizeSlider.Create(WS_CHILD | WS_VISIBLE, rect, this, ID_SIZE_SLIDER);
	mSizeSlider.SetRange(1, 100);
	mSizeSlider.SetPageSize(10);
	mSizeSlider.SetLineSize(1);
	mSizeSlider.SetPos(gDefaultTerrainBrushSize);
	mSizeSlider.m_bVisualManagerStyle = TRUE;

	mPowerSlider.Create(WS_CHILD | WS_VISIBLE, rect, this, ID_POWER_SLIDER);
	mPowerSlider.SetRange(1, 100);
	mPowerSlider.SetPageSize(10);
	mPowerSlider.SetLineSize(1);
	mPowerSlider.SetPos(gDefaultTerrainBrushPower);
	mPowerSlider.m_bVisualManagerStyle = TRUE;

	mTexBar.Create(rect, this, 1);
	mTexBar.AddMenuItem(1, "载入着色图...");
	mTexBar.AddMenuItem(1, "清除着色图");
	mTexBar.AddMenuItem(1, "加载着色图...");
	mTexBar.AddMenuItem(1, "保存着色图...");
	mTexBar.AddMenuItem(1, "切换图标尺寸");

	return 0;
}


void CTerrainEditWnd::OnPaint()
{
	CPaintDC PaintDC(this); // device context for painting
	CBCGPMemDC MemDC(PaintDC, this);
	CDC *DC = &MemDC.GetDC();

	CRect Client;
	GetClientRect(Client);

	CBCGPVisualManager::GetInstance ()->OnFillBarBackground(DC, this, Client, Client);
	DC->SetBkMode(TRANSPARENT); 

	CFont Font;
	Font.Attach(::GetStockObject(DEFAULT_GUI_FONT));
	CFont *OldFont = DC->SelectObject(&Font);

	DC->SetTextColor(RGB(0, 0, 0));
	DC->DrawText(mBrushText, &mBrushBox.ConvertToRect(), 0);
	DC->DrawText(mSizeText, &mSizeBox.ConvertToRect(), 0);
	DC->DrawText(mPowerText, &mPowerBox.ConvertToRect(), 0);
	DC->DrawText(mTexText, &mTexBox.ConvertToRect(), 0);
	DC->SelectObject(OldFont);
	CBCGPDockingControlBar::OnPaint();
}


void CTerrainEditWnd::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	if (cx < cy)
	{
		int BarHeight = (cy>>1)-70;

		mBrushBox.left = 5;
		mBrushBox.top = 5;
		mBrushBox.width = cx-10;
		mBrushBox.height = 20;

		CBox BrushBarBox;
		BrushBarBox.left = 5;
		BrushBarBox.top = mBrushBox.bottom()+3;
		BrushBarBox.width = cx-10;
		BrushBarBox.height = BarHeight;
		mBrushBar.SetWindowPos(NULL, BrushBarBox.left, BrushBarBox.top, BrushBarBox.width, BrushBarBox.height, SWP_NOACTIVATE|SWP_NOZORDER);

		mSizeBox.left = 5;
		mSizeBox.top = BrushBarBox.bottom()+3;
		mSizeBox.width = 55;
		mSizeBox.height = 20;

		CBox SizeSliderBox;
		SizeSliderBox.left = mSizeBox.right()+5;
		SizeSliderBox.top = mSizeBox.top;
		SizeSliderBox.width = cx-SizeSliderBox.left-10;
		SizeSliderBox.height = 20;
		mSizeSlider.SetWindowPos(NULL, SizeSliderBox.left, SizeSliderBox.top, SizeSliderBox.width, SizeSliderBox.height, SWP_NOACTIVATE | SWP_NOZORDER);

		mPowerBox.left = 5;
		mPowerBox.top = SizeSliderBox.bottom()+3;
		mPowerBox.width = 55;
		mPowerBox.height = 20;

		CBox PowerSliderBox;
		PowerSliderBox.left = mPowerBox.right()+5;
		PowerSliderBox.top = mPowerBox.top;
		PowerSliderBox.width = cx-PowerSliderBox.left-10;
		PowerSliderBox.height = 20;
		mPowerSlider.SetWindowPos(NULL, PowerSliderBox.left, PowerSliderBox.top, PowerSliderBox.width, PowerSliderBox.height, SWP_NOACTIVATE | SWP_NOZORDER);

		mTexBox.left = 5;
		mTexBox.top = PowerSliderBox.bottom()+3;
		mTexBox.width = cx-10;
		mTexBox.height = 20;

		CBox TexBarBox;
		TexBarBox.left = 5;
		TexBarBox.top = mTexBox.bottom()+3;
		TexBarBox.width = cx-10;
		TexBarBox.height = BarHeight;
		mTexBar.SetWindowPos(NULL, TexBarBox.left, TexBarBox.top, TexBarBox.width, TexBarBox.height, SWP_NOACTIVATE|SWP_NOZORDER);
	}
}

void CTerrainEditWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch(pScrollBar->GetDlgCtrlID())   
	{   
	case ID_SIZE_SLIDER:
		{ 
			mSizeText.Format("尺寸 %d", mSizeSlider.GetPos());
			InvalidateRect(&mSizeBox.ConvertToRect());
		} 
		break;
	case ID_POWER_SLIDER:
		{
			mPowerText.Format("强度 %d", mPowerSlider.GetPos());
			InvalidateRect(&mPowerBox.ConvertToRect());
		}
	}
	CBCGPDockingControlBar::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTerrainEditWnd::OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
	*pResult = 0;
	if(theApp.mSceneDoc == NULL)
		return;
	switch (pNMHDR->idFrom)
	{
	case ID_SIZE_SLIDER:
		if(theApp.mSceneDoc->GetTerrainEditHandler() != NULL)
			theApp.mSceneDoc->GetTerrainEditHandler()->SetBrushSize(mSizeSlider.GetPos());
		break;
	}
}
