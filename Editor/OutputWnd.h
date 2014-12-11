#pragma once

#include <vector>

class COutputWnd : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(COutputWnd)
public:
	COutputWnd();
	virtual ~COutputWnd();

	void Cache(CString Str);
	void Flush();
	void Clear();

	static COutputWnd *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CBCGPEdit mMessageList; CFont mFont;
	std::vector<CString> mCache;
};
