#include "stdafx.h"
#include "RenderPump.h"

#include "RenderView.h"
#include "CameraManager.h"
#include "CustomWindowMessage.h"

#include "OgreRoot.h"
#include "OgreCamera.h"
#include "OgreRenderWindow.h"
#include "OgreStringConverter.h"
#include "d3d9.h"
#include "Editor.h"

IMPLEMENT_DYNCREATE(RenderView, CView)

RenderView::RenderView()
{
	renderWindow = NULL;
}

RenderView::~RenderView()
{
}

CameraManager *RenderView::getCameraManager()
{
	return cameraManager;
}

void RenderView::setCameraManager(CameraManager *cameraManager)
{
	if(!renderWindow)
		setupView();
	renderWindow->getViewport(0)->setCamera(cameraManager->getCamera());
	this->cameraManager = cameraManager;
}

Ogre::RenderWindow *RenderView::getRenderWindow()
{
	return renderWindow;
}

Ogre::Viewport *RenderView::getViewport()
{
	return viewport;
}

void RenderView::roaming(OIS::Keyboard *keyboard, OIS::Mouse *mouse, float elapsed)
{
	POINT CursorPos;
	GetCursorPos(&CursorPos);
	ScreenToClient(&CursorPos);

	CRect rectClient;
	GetClientRect(rectClient);

	if(	CursorPos.x < 0 || CursorPos.x > rectClient.Width() ||
		CursorPos.y < 0 || CursorPos.y > rectClient.Height())
		return;

	if(cameraManager)
		cameraManager->roaming(keyboard, mouse, elapsed);
}

BEGIN_MESSAGE_MAP(RenderView, CView)
	ON_REGISTERED_MESSAGE(WM_CUSTOM_AFTER_CREATE, &RenderView::OnCustomAfterCreate)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

int RenderView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	PostMessage(WM_CUSTOM_AFTER_CREATE, 0, 0);
	return 0;
}

afx_msg LRESULT RenderView::OnCustomAfterCreate(WPARAM wParam, LPARAM lParam)
{
	if(!renderWindow)
		setupView();
	return 0;
}

void RenderView::OnDestroy()
{
	if(renderWindow && !renderWindow->isPrimary())
	{
		Ogre::Root::getSingleton().getRenderSystem()->destroyRenderWindow(renderWindow->getName());
	}
	RenderPump::current->setActiveView(NULL);
}

void RenderView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if(cameraManager)
	{
		renderWindow->resize(cx, cy);
		renderWindow->windowMovedOrResized();
		cameraManager->getCamera()->setAspectRatio(Ogre::Real(cx)/Ogre::Real(cy));
	}
}

BOOL RenderView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void RenderView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
	RenderPump::current->setActiveView(this);
}

void RenderView::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
}

void RenderView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	if(renderWindow)
		renderWindow->update();
}

void RenderView::setupView()
{
	Ogre::NameValuePairList miscParams;
	miscParams["externalWindowHandle"] = 
		Ogre::StringConverter::toString((size_t)m_hWnd);
	CRect Client;
	GetClientRect(Client);
	renderWindow = Ogre::Root::getSingleton().createRenderWindow(
		Ogre::StringConverter::toString(Ogre::Math::RangeRandom(0, 1000)) + ".RenderWindow", 
		Client.Width(), Client.Height(), false, &miscParams);
	renderWindow->setAutoUpdated(false);

	viewport = renderWindow->addViewport(NULL);
	viewport->setBackgroundColour(Ogre::ColourValue::Black);
	if(renderWindow->isPrimary())
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		IDirect3DDevice9 *device = NULL;
		renderWindow->getCustomAttribute("D3DDEVICE", &device);
		theApp.setDevice(device);
	}
}
