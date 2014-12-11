#include "stdafx.h"
#include "Editor.h"
#include "OutputWnd.h"

IMPLEMENT_DYNAMIC(COutputWnd, CBCGPDockingControlBar)

COutputWnd *COutputWnd::Current = NULL;
COutputWnd::COutputWnd()
{
	Current = this;
}

COutputWnd::~COutputWnd()
{
}

void COutputWnd::Cache(CString Str)
{
	mCache.push_back(Str);
}

void COutputWnd::Flush()
{
	if(!mCache.empty())
	{
		CString Tmp;
		mMessageList.GetWindowText(Tmp);

		for(int i=0; i<mCache.size(); i++)
		{
			Tmp += "1>" + mCache[i] + "\n";
		}
		Tmp.Replace("\n", "\r\n");

		mCache.clear();
		mMessageList.SetWindowText(Tmp);
	}
}

void COutputWnd::Clear()
{
	mCache.clear();
	mMessageList.SetWindowText("");
}

BEGIN_MESSAGE_MAP(COutputWnd, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	strcpy_s(lf.lfFaceName, "Consolas");
	lf.lfHeight = 14;
	//lf.lfWeight = FW_BOLD;

	mFont.CreateFontIndirect(&lf);

	mMessageList.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|ES_AUTOVSCROLL|ES_AUTOHSCROLL|ES_MULTILINE|ES_WANTRETURN, rectDummy, this, ID_MESSAGE_LIST);
	mMessageList.SetFont(&mFont);

	SetTimer(1, 1000, NULL);
	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	CRect rectClient;
	GetClientRect(rectClient);

	mMessageList.SetWindowPos(NULL, 0, 0, rectClient.Width(), rectClient.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
}

void COutputWnd::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		Flush();
	}
}
