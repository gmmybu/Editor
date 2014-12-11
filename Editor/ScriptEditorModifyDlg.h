#pragma once
#include "afxwin.h"

class CScriptEditorModifyDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CScriptEditorModifyDlg)

public:
	CScriptEditorModifyDlg(CWnd* pParent = NULL);
	virtual ~CScriptEditorModifyDlg();
	enum { IDD = IDD_SCRIPT_EDITOR_MODIFY };

	BOOL OnInitDialog();
	void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedPrev();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedOk();

	void SetModifyDialogText();
	void UpdateEntry();

	long mEntryNum;

	CStatic mActionText;
	CStatic mNum;
	CButton mPrev;
	CButton mNext;
	CStatic mFrame;
	CStatic mStatic1;
	CStatic mStatic2;
	CStatic mStatic3;
	CStatic mMin;
	CStatic mMax;
	CEdit mValue;
	CComboBox mChoice;
	CEdit mText;
};
