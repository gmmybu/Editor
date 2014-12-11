#include "stdafx.h"
#include "Editor.h"
#include "MaterialDoc.h"
#include "MaterialView.h"

IMPLEMENT_DYNCREATE(MaterialDoc, Scintilla::CScintillaDoc)

MaterialDoc::MaterialDoc()
{
}


MaterialDoc::~MaterialDoc()
{
}

BEGIN_MESSAGE_MAP(MaterialDoc, Scintilla::CScintillaDoc)
END_MESSAGE_MAP()

BOOL MaterialDoc::OnNewDocument()
{
	if (!Scintilla::CScintillaDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

BOOL MaterialDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if(!Scintilla::CScintillaDoc::OnSaveDocument(lpszPathName))
		return FALSE;
	// ((MaterialView*)activeView)->SetEdited(FALSE);
	return TRUE;
}
