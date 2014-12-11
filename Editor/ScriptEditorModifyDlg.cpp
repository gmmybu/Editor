#include "stdafx.h"
#include "Editor.h"
#include "ScriptEditorDlg.h"
#include "ScriptEditorModifyDlg.h"
#include "afxdialogex.h"

#include "Action.h"

IMPLEMENT_DYNAMIC(CScriptEditorModifyDlg, CBCGPDialog)

CScriptEditorModifyDlg::CScriptEditorModifyDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CScriptEditorModifyDlg::IDD, pParent)
{

}

CScriptEditorModifyDlg::~CScriptEditorModifyDlg()
{
}

BOOL CScriptEditorModifyDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	mEntryNum = 0;
	SetModifyDialogText();

	return TRUE;
}

void CScriptEditorModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACTION_TEXT, mActionText);
	DDX_Control(pDX, IDC_NUM, mNum);
	DDX_Control(pDX, IDC_PREV, mPrev);
	DDX_Control(pDX, IDC_NEXT, mNext);
	DDX_Control(pDX, IDC_FRAME, mFrame);
	DDX_Control(pDX, IDC_STATIC1, mStatic1);
	DDX_Control(pDX, IDC_STATIC2, mStatic2);
	DDX_Control(pDX, IDC_STATIC3, mStatic3);
	DDX_Control(pDX, IDC_MIN, mMin);
	DDX_Control(pDX, IDC_MAX, mMax);
	DDX_Control(pDX, IDC_VALUE, mValue);
	DDX_Control(pDX, IDC_CHOICE, mChoice);
	DDX_Control(pDX, IDC_TEXT, mText);
}


BEGIN_MESSAGE_MAP(CScriptEditorModifyDlg, CBCGPDialog)
	ON_BN_CLICKED(IDC_PREV, OnBnClickedPrev)
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

void CScriptEditorModifyDlg::OnBnClickedPrev()
{
	if(mEntryNum > 0) 
	{
		// Apply changes
		UpdateEntry();

		// Go to previous entry
		mEntryNum--;
		SetModifyDialogText();
	}
}

void CScriptEditorModifyDlg::OnBnClickedNext()
{
	sScript *Script = CScriptEditorDlg::Current->GetModifyScript();

	// Only bother if not last entry
	if(mEntryNum < Script->NumEntries-1) 
	{
		// Apply changes
		UpdateEntry();

		// Go to next entry
		mEntryNum++;
		SetModifyDialogText();
	}
}

void CScriptEditorModifyDlg::OnBnClickedOk()
{
	UpdateEntry();
	CBCGPDialog::OnOK();
}

void CScriptEditorModifyDlg::SetModifyDialogText()
{
	sAction *ActionPtr;
	char Text[2048];
	long i;

	cActionTemplate *Actions = CScriptEditorDlg::Current->GetActionTemplate();
	sScript *Script = CScriptEditorDlg::Current->GetModifyScript();
	ActionPtr = Actions->GetAction(Script->Type);
	Actions->ExpandActionText(Text, 2048, Script);

	mActionText.SetWindowText(Text);

	mPrev.ShowWindow(FALSE);
	mNext.ShowWindow(FALSE);
	mText.ShowWindow(FALSE);
	mChoice.ShowWindow(FALSE);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_TRUE),    FALSE);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_FALSE),   FALSE);
	mMin.ShowWindow(FALSE);
	mMax.ShowWindow(FALSE);
	mValue.ShowWindow(FALSE);
	mStatic1.ShowWindow(FALSE);
	mStatic2.ShowWindow(FALSE);
	mStatic3.ShowWindow(FALSE);

	mNum.SetWindowText("");
	mFrame.SetWindowText("No Entries");

	if(!ActionPtr->NumEntries)
		return;

	if(ActionPtr->NumEntries > 1)
	{
		mPrev.ShowWindow(TRUE);
		mNext.ShowWindow(TRUE);
	}
	sprintf_s(Text, "%lu of %lu", mEntryNum+1, ActionPtr->NumEntries);
	mNum.SetWindowText(Text);

	// Enable and set specific fields based on type
	switch(Script->Entries[mEntryNum].Type) 
	{
	case _TEXT:
		mFrame.SetWindowText(" Text Entry ");
		if(Script->Entries[mEntryNum].Text != NULL)
			mText.SetWindowText(Script->Entries[mEntryNum].Text);
		mText.ShowWindow(TRUE);
		break;

	case _BOOL:
		mFrame.SetWindowText(" Boolean Entry ");
		if(Script->Entries[mEntryNum].bValue == TRUE) 
		{
			::SendMessage(::GetDlgItem(m_hWnd, IDC_TRUE), BM_SETCHECK,  BST_CHECKED, 0);
			::SendMessage(::GetDlgItem(m_hWnd, IDC_FALSE), BM_SETCHECK, BST_UNCHECKED, 0);
		} else 
		{
			::SendMessage(::GetDlgItem(m_hWnd, IDC_TRUE), BM_SETCHECK,  BST_UNCHECKED, 0);
			::SendMessage(::GetDlgItem(m_hWnd, IDC_FALSE), BM_SETCHECK, BST_CHECKED, 0);
		}
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_TRUE),  TRUE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_FALSE), TRUE);
		break;

	case _INT:
		mFrame.SetWindowText(" Integer Entry ");
		sprintf_s(Text, "%lu", ActionPtr->Entries[mEntryNum].lMin);
		mMin.SetWindowText(Text);
		sprintf_s(Text, "%lu", ActionPtr->Entries[mEntryNum].lMax);
		mMax.SetWindowText(Text);
		sprintf_s(Text, "%lu", Script->Entries[mEntryNum].lValue);
		mValue.SetWindowText(Text);
		mMin.ShowWindow(TRUE);
		mMax.ShowWindow(TRUE);
		mValue.ShowWindow(TRUE);
		mStatic1.ShowWindow(TRUE);
		mStatic2.ShowWindow(TRUE);
		mStatic3.ShowWindow(TRUE);
		break;

	case _FLOAT:
		mFrame.SetWindowText(" Float Entry ");
		sprintf_s(Text, "%lf", ActionPtr->Entries[mEntryNum].fMin);
		mMin.SetWindowText(Text);
		sprintf_s(Text, "%lf", ActionPtr->Entries[mEntryNum].fMax);
		mMax.SetWindowText(Text);
		sprintf_s(Text, "%lf", Script->Entries[mEntryNum].fValue);
		mValue.SetWindowText(Text);
		mMin.ShowWindow(TRUE);
		mMax.ShowWindow(TRUE);
		mValue.ShowWindow(TRUE);
		mStatic1.ShowWindow(TRUE);
		mStatic2.ShowWindow(TRUE);
		mStatic3.ShowWindow(TRUE);
		break;

	case _CHOICE:
		mFrame.SetWindowText(" Choice Entry ");
		mChoice.ResetContent();
		if(ActionPtr->Entries[mEntryNum].NumChoices) 
		{
			for(i=0;i<ActionPtr->Entries[mEntryNum].NumChoices;i++) 
				mChoice.AddString(ActionPtr->Entries[mEntryNum].Choices[i]);
			mChoice.SetCurSel(Script->Entries[mEntryNum].Selection);
			mChoice.ShowWindow(TRUE);
		}
		break;
	}
}

void CScriptEditorModifyDlg::UpdateEntry()
{
  sAction *ActionPtr;
  char Text[2048];

  	cActionTemplate *Actions = CScriptEditorDlg::Current->GetActionTemplate();
	sScript *Script = CScriptEditorDlg::Current->GetModifyScript();

  // Get pointer to action
  ActionPtr = Actions->GetAction(Script->Type);

  // Return if no entries to update
  if(!ActionPtr->NumEntries)
    return;

  // Return on incorrect Entry #
  if(mEntryNum >= Script->NumEntries)
    return;

  // Update fields based on type
  switch(Script->Entries[mEntryNum].Type) {
    case _TEXT:
      // Delete old text
      delete [] Script->Entries[mEntryNum].Text;
      Script->Entries[mEntryNum].Text = NULL;
      Script->Entries[mEntryNum].Length = 0;

      // Get new text
      mText.GetWindowText(Text, 2048);
      if(Text[0]) {
        Script->Entries[mEntryNum].Length = strlen(Text) + 1;
        Script->Entries[mEntryNum].Text = new char[Script->Entries[mEntryNum].Length];
        strcpy_s(Script->Entries[mEntryNum].Text, Script->Entries[mEntryNum].Length, Text);
      }
      break;

    case _BOOL:
		// Choose TRUE or FALSE from radio buttons
		if(::SendMessage(::GetDlgItem(m_hWnd, IDC_TRUE), BM_GETCHECK, 0, 0) == BST_CHECKED)
			Script->Entries[mEntryNum].bValue = TRUE;
		else
			Script->Entries[mEntryNum].bValue = FALSE;
		break;

    case _INT:
      // Get int value and bounds check with min/max
      mValue.GetWindowText(Text, 2048);
      Script->Entries[mEntryNum].lValue = atol(Text);
      if(Script->Entries[mEntryNum].lValue < ActionPtr->Entries[mEntryNum].lMin)
        Script->Entries[mEntryNum].lValue = ActionPtr->Entries[mEntryNum].lMin;
      if(Script->Entries[mEntryNum].lValue > ActionPtr->Entries[mEntryNum].lMax)
        Script->Entries[mEntryNum].lValue = ActionPtr->Entries[mEntryNum].lMax;
      break;

    case _FLOAT:
      // Get float value and bounds check with min/max
      mValue.GetWindowText(Text, 2048);
      Script->Entries[mEntryNum].fValue = (float)atof(Text);
      if(Script->Entries[mEntryNum].fValue < ActionPtr->Entries[mEntryNum].fMin)
        Script->Entries[mEntryNum].fValue = ActionPtr->Entries[mEntryNum].fMin;
      if(Script->Entries[mEntryNum].fValue > ActionPtr->Entries[mEntryNum].fMax)
        Script->Entries[mEntryNum].fValue = ActionPtr->Entries[mEntryNum].fMax;
      break;

    case _CHOICE:
      // Store choice selection
		Script->Entries[mEntryNum].Selection = mChoice.GetCurSel();
      break;
  }
}
