#include "stdafx.h"
#include "resource.h"
#include "MainFrm.h"
#include "RenderPump.h"

#include "FreeImage.h"

IMPLEMENT_DYNAMIC(CMainFrame, CBCGPMDIFrameWnd)

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBCGPMDITabParams mdiTabParams;
	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	//////////////////////////////////////////////////
	// 第三方
	//////////////////////////////////////////////////

	InitScintilla();
	InitFreeImage();
	InitOGRE();

	CreateToolBar();
	CreateStatusBar();
	CreateDockingWindows();

	//CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager2013));
	//CBCGPVisualManager2013::SetStyle(CBCGPVisualManager2013::Style::Office2013_Gray);
	CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
	SetTimer(1, 30, NULL);

	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		mRenderPump.renderOneFrame();
	}
}

void CMainFrame::CreateToolBar()
{
	mObjectEditToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	mObjectEditToolBar.LoadToolBar(IDR_OBJECT_EDIT);

	mObjectEditToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&mObjectEditToolBar);

	CBCGPToolBar::ResetAllImages();
	mObjectEditToolBar.LoadBitmap(IDB_OBJECT_EDIT);
}

void CMainFrame::CreateStatusBar()
{
	static UINT Indicators[] =
	{
		ID_SEPARATOR,           
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
	};

	mStatusBar.Create(this);
	mStatusBar.SetIndicators(Indicators, sizeof(Indicators)/sizeof(UINT));
}

void CMainFrame::CreateDockingWindows()
{
	CBCGPToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize(CSize(16, 16));
	imagesWorkspace.SetTransparentColor(RGB(192, 192, 192));
	imagesWorkspace.Load(IDB_DOCKING_WINDOW);

	// 创建资源管理器窗口
	mResourceManager.Create("资源管理", this, CRect(0, 0, 200, 200), TRUE, ID_RESOURCE_MANAGER, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	mResourceManager.SetIcon(imagesWorkspace.ExtractIcon(0), FALSE);
	mResourceManager.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&mResourceManager);

	// 创建场景资源窗口
	mSceneResource.Create("场景资源", this, CRect(0, 0, 200, 200), TRUE, ID_SCENE_RESOURCE, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	mSceneResource.SetIcon(imagesWorkspace.ExtractIcon(1), FALSE);
	mSceneResource.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&mSceneResource);

	// 创建混合编辑窗口
	mMiscEditWnd.Create("混合编辑", this, CRect(0, 0, 200, 200), TRUE, ID_MISC_EDIT_WND,
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	mMiscEditWnd.EnableDocking(CBRS_ALIGN_ANY);
	mMiscEditWnd.SetIcon(imagesWorkspace.ExtractIcon(2), FALSE);
	DockControlBar(&mMiscEditWnd);

	mSceneResource.AttachToTabWnd(&mResourceManager, BCGP_DM_STANDARD, FALSE, NULL);
	mMiscEditWnd.AttachToTabWnd(&mResourceManager, BCGP_DM_STANDARD, FALSE, NULL);

	// 创建属性窗口
	mPropertyWnd.Create("属性", this, CRect(0, 0, 200, 200), TRUE, ID_PROPERTY_WND, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_RIGHT|CBRS_FLOAT_MULTI);
	mPropertyWnd.SetIcon(imagesWorkspace.ExtractIcon(4), FALSE);
	mPropertyWnd.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&mPropertyWnd);

	// 创建相机窗口
	mCameraWnd.Create("相机", this, CRect(0, 0, 200, 200), TRUE, ID_CAMERA_WND, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_RIGHT|CBRS_FLOAT_MULTI);
	mCameraWnd.SetIcon(imagesWorkspace.ExtractIcon(3), FALSE);
	mCameraWnd.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&mCameraWnd);

	// 创建资源预览窗口
	mResourcePreview.Create("资源预览", this, CRect(0, 0, 200, 200), TRUE, ID_RESOURCE_PREVIEW, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	mResourcePreview.SetIcon(imagesWorkspace.ExtractIcon(5), FALSE);
	mResourcePreview.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&mResourcePreview);

	mResourcePreview.DockToWindow(&mPropertyWnd, CBRS_ALIGN_BOTTOM);
	mCameraWnd.DockToWindow(&mPropertyWnd, CBRS_ALIGN_BOTTOM);

	// 创建输出窗口
	mOutputWnd.Create("输出", this, CRect(0, 0, 200, 200), TRUE, ID_RESOURCE_PREVIEW, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_BOTTOM|CBRS_FLOAT_MULTI);
	mOutputWnd.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&mOutputWnd);
}

void CMainFrame::InitScintilla()
{
	LoadLibrary("SciLexer.dll");
}

void CMainFrame::InitFreeImage()
{
	FreeImage_Initialise();
}

void CMainFrame::InitOGRE()
{
	mRenderPump.initialize();
}
