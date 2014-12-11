#pragma once

#include "ScintillaDocView.h"
#include <string>
#include <vector>

class MaterialView : public Scintilla::CScintillaView
{
	DECLARE_DYNCREATE(MaterialView)
public:
	MaterialView();
	virtual ~MaterialView();

	//BOOL IsEdited() { return mEdited; }
	//void SetEdited(BOOL Edited) { mEdited = Edited; }

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void OnInitialUpdate();
	void OnCharAdded(Scintilla::SCNotification* pSCNotification);
	void OnUpdateUI(Scintilla::SCNotification* pSCNotification);

	void parseFile();
	void parseMaterialFile();
	void parseCompositorFile();
	void setAStyle(int style, COLORREF fore, COLORREF back=RGB(0xff, 0xff, 0xff), int size=-1, const char *face=0);

	/* BOOL mEdited; */ bool timer; int lastChar;
	std::vector<std::string> materials;
};
