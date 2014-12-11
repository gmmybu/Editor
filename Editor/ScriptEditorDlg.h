#pragma once

#include "afxwin.h"
#include "Action.h"

class CScriptEditorDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CScriptEditorDlg)
public:
	CScriptEditorDlg(CWnd* pParent = NULL);
	virtual ~CScriptEditorDlg();
	enum { IDD = IDD_SCRIPT_EDITOR };
	
	cActionTemplate *GetActionTemplate() { return &mActionTemplate; }
	sScript *GetModifyScript() { return mModifyScript; }

	void DoDataExchange(CDataExchange* pDX);

	static CScriptEditorDlg *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedLoadScript();
	afx_msg void OnBnClickedSaveScript();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedEdit();
	afx_msg void OnBnClickedMoveUp();
	afx_msg void OnBnClickedMoveDown();
	afx_msg void OnLbnDblclkActions();
	afx_msg void OnBnClickedAddToScript();
	afx_msg void OnBnClickedInsertToScript();
	afx_msg void OnBnClickedLoadActions();

	sScript *mScriptParent;
	CListBox mScript, mScriptList;
	long mNumScriptActions;

	cActionTemplate mActionTemplate;
	CListBox mActions;
	sScript *mModifyScript;
};
