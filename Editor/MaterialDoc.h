#pragma once

#include "ScintillaDocView.h"

class MaterialDoc : public Scintilla::CScintillaDoc
{
	DECLARE_DYNCREATE(MaterialDoc)
public:
	MaterialDoc();
	virtual ~MaterialDoc();

	CView *getActiveView() { return activeView; }
	void setActiveView(CView *activeView) { this->activeView = activeView; }

	DECLARE_MESSAGE_MAP()
	BOOL OnNewDocument();
	BOOL OnSaveDocument(LPCTSTR lpszPathName);

	CView *activeView;
};
