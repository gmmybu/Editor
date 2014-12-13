#include "stdafx.h"
#include "Editor.h"

#include "ChildFrm.h"
#include "GameDoc.h"
#include "GameView.h"
#include "MainFrm.h"
#include "MaterialDoc.h"
#include "MaterialView.h"
#include "NewSceneDlg.h"
#include "OgreXMLConverterDlg.h"
#include "OutputWnd.h"
#include "SceneDoc.h"
#include "SceneView.h"
#include "ScriptEditorDlg.h"
#include "ImageCombinator.h"

CEditorApp theApp;

CEditorApp::CEditorApp()
{
	SetAppID(_T("Editor.AppID.NoVersion"));
	m_bSaveState = FALSE;
}

BOOL CEditorApp::InitInstance()
{
	CBCGPWinApp::InitInstance();
	EnableTaskbarInteraction(FALSE);

	docTemplates["Scene"] = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(SceneDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(SceneView));
	if (!docTemplates["Scene"])
		return FALSE;
	AddDocTemplate(docTemplates["Scene"]);

	docTemplates["Game"] = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CGameDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CGameView));
	if (!docTemplates["Game"])
		return FALSE;
	AddDocTemplate(docTemplates["Game"]);

	docTemplates["Material"] = new CMultiDocTemplate(IDR_MATERIAL,
		RUNTIME_CLASS(MaterialDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(MaterialView));
	if (!docTemplates["Material"])
		return FALSE;
	AddDocTemplate(docTemplates["Material"]);

	//////////////////////////////////////////////////
	// MFC
	//////////////////////////////////////////////////

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if(!ProcessShellCommand(cmdInfo))
		return FALSE;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CEditorApp::ExitInstance()
{
	return CBCGPWinApp::ExitInstance();
}

CMultiDocTemplate *CEditorApp::getDocTemplate(std::string name)
{
	return docTemplates[name];
}

void CEditorApp::addDocTemplate(std::string name, CMultiDocTemplate *docTemplate)
{
	docTemplates[name] = docTemplate;
}

std::string CEditorApp::getPath(std::string name)
{
	return paths[name];
}

void CEditorApp::addPath(std::string name, std::string path)
{
	paths[name] = path;
}

BEGIN_MESSAGE_MAP(CEditorApp, CBCGPWinApp)
	ON_COMMAND(ID_NEW_SCENE, &CEditorApp::OnNewScene)
	ON_COMMAND(ID_OPEN_SCENE, &CEditorApp::OnOpenScene)
	ON_COMMAND(ID_NEW_GAME, &CEditorApp::OnNewGame)
	ON_COMMAND(ID_XML_CONVERTER, &CEditorApp::OnOgreXmlConverter)
	ON_COMMAND(ID_SCRIPT_EDITOR, &CEditorApp::OnScriptEditor)
	ON_COMMAND(ID_CLEAR_LOG, &CEditorApp::OnClearLog)
	ON_COMMAND(ID_APP_ABOUT, &CEditorApp::OnAppAbout)
	ON_UPDATE_COMMAND_UI_RANGE(ID_LOAD_TEXTURE, ID_RESIZE_TEXTURE, OnUpdateTextureMenu)
	ON_COMMAND(ID_IMAGE_COMBINATOR, &CEditorApp::OnImageCombinator)
END_MESSAGE_MAP()

void CEditorApp::OnNewScene()
{
	NewSceneDlg Dlg;
	if(Dlg.DoModal() == IDOK)
	{
		docTemplates["Scene"]->OpenDocumentFile(NULL);
		SceneDoc::current->initialize(&Dlg);
	}
}

void CEditorApp::OnOpenScene()
{
	CFileDialog Dlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR, 
		"Scene config (*.scene)|*.scene|\
		All Files (*.*)|*.*||", NULL);
	if(Dlg.DoModal() == IDOK)
	{
		docTemplates["Scene"]->OpenDocumentFile(NULL);
		SceneDoc::current->initialize(Dlg.GetPathName());
	}
}

void CEditorApp::OnNewGame()
{
	docTemplates["Game"]->OpenDocumentFile(NULL);
	CGameDoc::Current->Initialize();
}

void CEditorApp::OnOgreXmlConverter()
{
	COgreXMLConverterDlg Dlg;
	Dlg.DoModal();
}

void CEditorApp::OnScriptEditor()
{
	CScriptEditorDlg Dlg;
	Dlg.DoModal();
}

void CEditorApp::OnClearLog()
{
	COutputWnd::Current->Clear();
}

void CEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CEditorApp::OnUpdateTextureMenu(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


void CEditorApp::OnImageCombinator()
{
	ImageCombinator imageCombinator;
	imageCombinator.DoModal();
}

IDirect3DDevice9 *CEditorApp::getDevice()
{
	return device;
}

void CEditorApp::setDevice(IDirect3DDevice9 *device)
{
	this->device = device;
}
