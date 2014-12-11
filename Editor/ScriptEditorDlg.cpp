#include "stdafx.h"
#include "Editor.h"
#include "ScriptEditorDlg.h"
#include "ScriptEditorModifyDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CScriptEditorDlg, CBCGPDialog)

CScriptEditorDlg *CScriptEditorDlg::Current = NULL;
CScriptEditorDlg::CScriptEditorDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CScriptEditorDlg::IDD, pParent)
{
	Current = this;
	mScriptParent = NULL;
	mNumScriptActions = 0;
	mModifyScript = NULL;
}

CScriptEditorDlg::~CScriptEditorDlg()
{
}

void CScriptEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCRIPT, mScript);
	DDX_Control(pDX, IDC_SCRIPTLIST, mScriptList);
	DDX_Control(pDX, IDC_ACTIONS, mActions);
}

BEGIN_MESSAGE_MAP(CScriptEditorDlg, CBCGPDialog)
	ON_BN_CLICKED(IDC_LOAD_SCRIPT, &CScriptEditorDlg::OnBnClickedLoadScript)
	ON_BN_CLICKED(IDC_SAVE_SCRIPT, &CScriptEditorDlg::OnBnClickedSaveScript)
	ON_BN_CLICKED(IDC_DELETE, &CScriptEditorDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_EDIT, &CScriptEditorDlg::OnBnClickedEdit)
	ON_BN_CLICKED(IDC_MOVE_UP, &CScriptEditorDlg::OnBnClickedMoveUp)
	ON_BN_CLICKED(IDC_MOVE_DOWN, &CScriptEditorDlg::OnBnClickedMoveDown)
	ON_LBN_DBLCLK(IDC_ACTIONS, &CScriptEditorDlg::OnLbnDblclkActions)
	ON_BN_CLICKED(IDC_ADD_TO_SCRIPT, &CScriptEditorDlg::OnBnClickedAddToScript)
	ON_BN_CLICKED(IDC_INSERT_TO_SCRIPT, &CScriptEditorDlg::OnBnClickedInsertToScript)
	ON_BN_CLICKED(IDC_LOAD_ACTIONS, &CScriptEditorDlg::OnBnClickedLoadActions)
END_MESSAGE_MAP()

void CScriptEditorDlg::OnBnClickedLoadScript()
{
	FILE *fp;
	long i, j;
	char Text[1024];
	sScript *Script, *ScriptPtr = NULL;

	CString FilePathName;
	CFileDialog Dlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR, "MLS Script Files (*.mls)|*.mls|All Files (*.*)|*.*||", NULL);
	if(Dlg.DoModal()==IDOK)
		FilePathName = Dlg.GetPathName();

	// Open the file for input
	fopen_s(&fp, FilePathName, "rb");
	
	// Delete the current script
	delete mScriptParent;

	// Get # of script actions
	fread(&mNumScriptActions, 1, sizeof(long), fp);

	// Loop through each script action
	for(i=0;i<mNumScriptActions;i++) 
	{
		// Allocate a script structure and link in
		Script = new sScript();
		Script->Next = NULL;
		if(ScriptPtr == NULL)
			mScriptParent = Script;
		else
			ScriptPtr->Next = Script;
		ScriptPtr = Script;

		// Get type of action and # of entries
		fread(&Script->Type, 1, sizeof(long), fp);
		fread(&Script->NumEntries, 1, sizeof(long), fp);

		// Get entry data (if any)
		if(Script->NumEntries) 
		{
			// Allocate entry array
			Script->Entries = new sScriptEntry[Script->NumEntries]();

			// Load in each entry
			for(j=0;j<Script->NumEntries;j++) 
			{
				// Get entry type
				fread(&Script->Entries[j].Type, 1, sizeof(long), fp);
        
				// Get entry data
				fread(&Script->Entries[j].IOValue, 1, sizeof(long), fp);

				// Get text (if any)
				if(Script->Entries[j].Type == _TEXT && Script->Entries[j].Length) 
				{
					// Allocate a buffer and get string
					Script->Entries[j].Text = new char[Script->Entries[j].Length];
					fread(Script->Entries[j].Text, 1, Script->Entries[j].Length, fp);
				}
			}
		}
	}

	fclose(fp);

	// Clear the script and script list boxes
	mScript.ResetContent();
	mScriptList.ResetContent();

	// Add script actions to list
	ScriptPtr = mScriptParent;
	while(ScriptPtr != NULL) 
	{
		// Add script action text to list
		mActionTemplate.ExpandActionText(Text, 1024, ScriptPtr);
		mScript.AddString(Text);

		// Add script action pointer to list
		sprintf_s(Text, "%lu", ScriptPtr);
		mScriptList.AddString(Text);

		ScriptPtr = ScriptPtr->Next;
	}
}

void CScriptEditorDlg::OnBnClickedSaveScript()
{
	FILE *fp;
	long i, j;
	char Text[1024];
	sScript *ScriptPtr;

	// Make sure there's some script actions
	if(!mNumScriptActions)
		return;

	CString FilePathName;
	CFileDialog Dlg(FALSE, NULL, NULL, OFN_NOCHANGEDIR, "MLS Script Files (*.mls)|*.mls|All Files (*.*)|*.*||", NULL);
	if(Dlg.DoModal()==IDOK)
		FilePathName = Dlg.GetPathName();

	// Open the file for output
	fopen_s(&fp, FilePathName, "wb");

	// Output # of script actions
	fwrite(&mNumScriptActions, 1, sizeof(long), fp);

	// Loop through each script action
	for(i=0;i<mNumScriptActions;i++) 
	{
		// Get a pointer to the script structure (don't depend on list)
		mScriptList.GetText(i, Text);
		ScriptPtr = (sScript*)atol(Text);

		// Output type of action and # of entries
		fwrite(&ScriptPtr->Type, 1, sizeof(long), fp);
		fwrite(&ScriptPtr->NumEntries, 1, sizeof(long), fp);

		// Output entry data (if any)
		if(ScriptPtr->NumEntries) 
		{
			for(j=0;j<ScriptPtr->NumEntries;j++) 
			{
				// Write entry type
				fwrite(&ScriptPtr->Entries[j].Type, 1, sizeof(long), fp);

				// Write entry data
				fwrite(&ScriptPtr->Entries[j].IOValue, 1, sizeof(long), fp);

				// Write text (if any)
				if(ScriptPtr->Entries[j].Type == _TEXT && ScriptPtr->Entries[j].Text != NULL)
					fwrite(ScriptPtr->Entries[j].Text, 1, ScriptPtr->Entries[j].Length, fp);
			}
		}
	}
	
	fclose(fp);
}

void CScriptEditorDlg::OnBnClickedDelete()
{
	int CurSel = mScript.GetCurSel();
	if(CurSel == LB_ERR)
		return;

	CString Text;
	mScriptList.GetText(CurSel, Text);
	
	sScript *Script = (sScript*)atol(Text);
	if(Script->Prev != NULL)
		Script->Prev->Next = Script->Next;
	else
		mScriptParent = Script->Next;
	if(Script->Next != NULL)
		Script->Next->Prev = Script->Prev;
	Script->Next = NULL; // So whole list isn't deleted
	delete Script;

	mScript.DeleteString(CurSel);
	mScriptList.DeleteString(CurSel);

	mNumScriptActions--;
}


void CScriptEditorDlg::OnBnClickedEdit()
{
	int CurSel = mScript.GetCurSel();
	if(CurSel == LB_ERR)
		return;

	char Text[1024];
	mScriptList.GetText(CurSel, Text);
	sScript *Script = (sScript*)atol(Text);

	mModifyScript = Script;
	CScriptEditorModifyDlg Dlg;
	Dlg.DoModal();

	mActionTemplate.ExpandActionText(Text, 1024, Script);
	mScript.DeleteString(CurSel);
	mScript.InsertString(CurSel, Text);
}

void CScriptEditorDlg::OnBnClickedMoveUp()
{
	int CurSel = mScript.GetCurSel();
	if(CurSel == LB_ERR)
		return;

	char Text[1024];
	mScript.GetText(CurSel, Text);
	mScript.DeleteString(CurSel);
	mScript.InsertString(CurSel-1, Text);

	mScriptList.GetText(CurSel, Text);
	mScriptList.DeleteString(CurSel);
	mScriptList.InsertString(CurSel-1, Text);

	mScript.SetCurSel(CurSel-1);
}


void CScriptEditorDlg::OnBnClickedMoveDown()
{
	int CurSel = mScript.GetCurSel();
	if(CurSel == LB_ERR)
		return;

	char Text[1024];
	mScript.GetText(CurSel, Text);
	mScript.DeleteString(CurSel);
	mScript.InsertString(CurSel+1, Text);

	mScriptList.GetText(CurSel, Text);
	mScriptList.DeleteString(CurSel);
	mScriptList.InsertString(CurSel+1, Text);

	mScript.SetCurSel(CurSel+1);
}

void CScriptEditorDlg::OnLbnDblclkActions()
{
	OnBnClickedAddToScript();
}

void CScriptEditorDlg::OnBnClickedAddToScript()
{
	int CurSel;
	char Text[1024];
	sScript *Script;
	sAction *ActionPtr;

	CurSel = mActions.GetCurSel();
	if(CurSel == LB_ERR)
		return;
	
	Script = mActionTemplate.CreateScriptAction(CurSel);
	mModifyScript = Script;

	CScriptEditorModifyDlg Dlg;
	if(Dlg.DoModal() == IDOK)
	{
		// Add the script action to script
		Script->Next = mScriptParent;
		if(mScriptParent != NULL)
			mScriptParent->Prev = Script;
		mScriptParent = Script;

		// Display script action text
		ActionPtr = mActionTemplate.GetAction(CurSel);
		mActionTemplate.ExpandActionText(Text, 1024, Script);
		mScript.AddString(Text);

		// Add pointer to script list
		sprintf_s(Text, "%lu", Script);
		mScriptList.AddString(Text);

		// Increase count
		mNumScriptActions++;
	} 
	else 
	{
		delete Script;
	}
}

void CScriptEditorDlg::OnBnClickedInsertToScript()
{
	int Pos = mScript.GetCurSel();
	if(Pos == LB_ERR)
		return;

	int CurSel = mActions.GetCurSel();
	if(CurSel == LB_ERR)
		return;

	sScript *Script = mActionTemplate.CreateScriptAction(CurSel);
	mModifyScript = Script;

	CScriptEditorModifyDlg Dlg;
	if(Dlg.DoModal() == IDOK)
	{
		Script->Prev = NULL;
		Script->Next = mScriptParent;
		mScriptParent->Prev = Script;
		mScriptParent = Script;

		char Text[1024];
		mActionTemplate.ExpandActionText(Text, 1024, Script);
		mScript.InsertString(Pos, Text);

		sprintf_s(Text, "%lu", Script);
		mScriptList.InsertString(Pos, Text);

		mNumScriptActions++;
	}
	else
	{
		delete Script;
	}
}

void CScriptEditorDlg::OnBnClickedLoadActions()
{
	char Text[1024];

	CFileDialog Dlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR, "MLS Action Files (*.mla)|*.mla|All Files (*.*)|*.*||", NULL);
	if(Dlg.DoModal() == IDOK)
	{
		mActionTemplate.Load(Dlg.GetPathName());
		
		// Clear the list box
		mActions.ResetContent();

		// Get a pointer to the parent action
		sAction *Ptr = mActionTemplate.GetActionParent();

		// Loop through all actions
		for(long i=0;i<mActionTemplate.GetNumActions();i++) 
		{
			// Get expanded action text
			mActionTemplate.ExpandDefaultActionText(Text, 1024, Ptr);
			// Add action text to action list
			mActions.AddString(Text);
			// Go to next action
			Ptr = Ptr->Next;
		}
	}
}
